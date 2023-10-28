/**

*/

#include "buffer.h"
#include "ext.h"
#include "ext_buffer.h"
#include "ext_common.h"
#include "ext_globalsymbol.h"
#include "ext_obex.h"
#include "ext_systhread.h"



void* bug_class;

typedef struct _bug {
    t_object b_ob;
    t_buffer* buf;
    t_symbol* bufname; // buffer name
    void* out;
    int resize;
    long n;
    double freq;

    // new paprt
    short               busy;
    t_systhread         x_systhread;    // thread reference
    t_systhread_mutex   x_mutex;        // mutual exclusion lock for thread safety
    void                *x_qelem;
} t_bug;


void bug_set_buffer(t_bug* x, t_symbol* s);
void bug_fill(t_bug* x, long n, double freq);
void bug_resize(t_bug* x, long n);
void bug_do_fill(t_bug* x);
t_max_err bug_notify(t_bug* x, t_symbol* s, t_symbol* msg, void* sender, void* data);
void* bug_new(t_symbol* s);
void* bug_class;


t_symbol* ps_buffer;
t_symbol* ps_globalsymbol_binding;
t_symbol* ps_globalsymbol_unbinding;
t_symbol* ps_buffer_modified;

//--------------------------------------------------------------------------

int main(void)
{
    t_class* c;

    c = class_new("bug", (method)bug_new, 0, sizeof(t_bug), 0L, A_SYM, 0);
    class_addmethod(c, (method)bug_set_buffer, 	"set", 		A_SYM, 0L);
    class_addmethod(c, (method)bug_fill, 		"fill", 	A_LONG, A_FLOAT, 0);
    class_addmethod(c, (method)bug_notify, 		"notify", 	A_CANT, 0);
    class_addmethod(c, (method)bug_resize,      "resize",   A_LONG, 0);
    class_register(CLASS_BOX, c);
    bug_class = c;

    ps_buffer = gensym("buffer~");
    ps_globalsymbol_binding = gensym("globalsymbol_binding");
    ps_globalsymbol_unbinding = gensym("globalsymbol_unbinding");
    ps_buffer_modified = gensym("buffer_modified");

    return 0;
}


void bug_set_buffer(t_bug* x, t_symbol* s)
{
    if (s != x->bufname) {
        // Reference the buffer associated with the incoming name
        x->buf = (t_buffer*)globalsymbol_reference((t_object*)x, s->s_name,
                                                   "buffer~");
        post("buffer referenced: %s", s->s_name);
        if (x->bufname) {
            // If we were previously referencing another buffer, we should not
            // longer reference it.
            globalsymbol_dereference((t_object*)x, x->bufname->s_name,
                                     "buffer~");
        }
        x->bufname = s;
    }
}


void bug_resize(t_bug* x, long n)
{
    t_atom aa;
    atom_setlong(&aa, n);
    // typedmess((t_object*)x->buf, gensym("sizeinsamps"), 1, &aa);
    object_method_typed((t_object*)x->buf, gensym("sizeinsamps"), 1, &aa, NULL);

}


void bug_fill(t_bug* x, long n, double freq)
{

    long frames;
    t_buffer* b = NULL;

    // save private vars in obj struct, so we can recall
    x->n = n;
    x->freq = freq;

    b = x->buf;
    if (!b) {
        object_error((t_object*)x, "%s is no valid buffer", x->bufname);
        return;
    }

    ATOMIC_INCREMENT(&b->b_inuse);
    if (!b->b_valid) {
        ATOMIC_DECREMENT(&b->b_inuse);
        return;
    }

    frames = b->b_frames;
    ATOMIC_DECREMENT(&b->b_inuse);

    // n: number of samples we need. check if buffer is long enough
    if (n > frames) {
        object_warn((t_object*)x, "sorry have to resize first");
        x->resize = true; // set flag for notify method
        bug_resize(x, n); // resize buffer

        return;
    }

    object_post((t_object*)x, "all seems to be OK, start writing now...");

    bug_do_fill(x);
}


void bug_do_fill(t_bug* x)
{
    float* tab;
    long frames, nchnls, i;
    t_buffer* b = NULL;
    double incr;
    long n = x->n;
    double freq = x->freq;

    b = x->buf;
    if (!b) {
        object_error((t_object*)x, "%s is no valid buffer", x->bufname);
        return;
    }

    post("atomic increment?");
    ATOMIC_INCREMENT(&b->b_inuse);
    if (!b->b_valid) {
        ATOMIC_DECREMENT(&b->b_inuse);
        return;
    }
    post("atomic increment, yes!");
    tab = b->b_samples;
    frames = b->b_frames;
    nchnls = b->b_nchans;

    // check again if buffer is long enough
    if (n > frames) {
        object_error((t_object*)x, "there's definitely something wrong here!");
        ATOMIC_DECREMENT(&b->b_inuse);
        return;
    }

    object_post((t_object*)x, "starting to write to buffer now...");
    incr = 2 * M_PI * freq / n;
    for (i = 0; i < n; i++)
        tab[i * nchnls] = sin(i * incr);

    ATOMIC_DECREMENT(&b->b_inuse);

    object_method((t_object*)x->buf, gensym("dirty"));

    // bang when finished processing
    outlet_bang(x->out);
}


t_max_err bug_notify(t_bug* x, t_symbol* s, t_symbol* msg, void* sender,
                     void* data)
{
    if (msg == ps_globalsymbol_binding)
        x->buf = (t_buffer*)x->bufname->s_thing;
    else if (msg == ps_globalsymbol_unbinding) {
        x->buf = NULL;
    } else if (msg == ps_buffer_modified) {
        if (x->resize) {
            object_post((t_object*)x, "notify: resize complete!");
            x->resize = false;

            // bug_do_fill(x);		// this won't work
            // defer(x, (method)bug_do_fill, NULL,0, NULL);	// this won't work
            // either
            defer_low(x, (method)bug_do_fill, NULL, 0, NULL);
        }
    }

    return MAX_ERR_NONE;
}


//--------------------------------------------------------------------------

void* bug_new(t_symbol* s)
{

    t_bug* x;

    x = (t_bug*)object_alloc(bug_class);
    x->out = bangout(x);

    bug_set_buffer(x, s);

    x->resize = false;

    x->n = 10;
    x->freq = 2;

    return x;
}
