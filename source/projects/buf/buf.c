#include "ext.h"
#include "ext_obex.h"
#include "ext_buffer.h"


typedef struct _buf {
    t_object ob;
    void* outlet;
    t_symbol* buf_name;    // buffer name
    t_buffer_ref* buf_ref; // buffer ref
} t_buf;


// method prototypes
void* buf_new(t_symbol* s, long argc, t_atom* argv);
void buf_free(t_buf* x);
void buf_bang(t_buf* x);
void buf_float(t_buf* x, double f);
void buf_resize(t_buf* x, t_symbol* s, long argc, t_atom* argv);
void buf_do_resize(t_buf* x, t_symbol* s, long argc, t_atom* argv);
t_max_err buf_notify(t_buf* x, t_symbol* s, t_symbol* msg, void* sender,
                     void* data);
t_max_err buf_set_buf(t_buf* x, t_symbol* s);


// global class pointer variable
static t_class* buf_class = NULL;


void ext_main(void* r)
{
    t_class* c = class_new("buf", (method)buf_new, (method)buf_free,
                           (long)sizeof(t_buf), 0L, A_GIMME, 0);

    class_addmethod(c, (method)buf_bang,   "bang",  0);
    class_addmethod(c, (method)buf_resize, "resize", A_GIMME, 0);
    class_addmethod(c, (method)buf_notify, "notify", A_CANT, 0);

    class_register(CLASS_BOX, c);
    buf_class = c;
}


void* buf_new(t_symbol* s, long argc, t_atom* argv)
{
    t_buf* x = (t_buf*)object_alloc(buf_class);

    if (x) {
        x->outlet = outlet_new(x, NULL); // outlet
        x->buf_ref = NULL;
        x->buf_name = gensym("");
    }
    return (x);
}

void buf_free(t_buf* x)
{
    if (x->buf_ref)
        object_free(x->buf_ref);
}


void buf_bang(t_buf* x)
{
    outlet_bang(x->outlet);
}


t_max_err buf_notify(t_buf* x, t_symbol* s, t_symbol* msg, void* sender,
                     void* data)
{
    return buffer_ref_notify(x->buf_ref, s, msg, sender, data);
}


t_max_err buf_set_buf(t_buf* x, t_symbol* s)
{
    if (!x->buf_ref) {
        x->buf_ref = buffer_ref_new((t_object*)x, s);
    } else {
        buffer_ref_set(x->buf_ref, s);
    }

    if (buffer_ref_exists(x->buf_ref)) {
        object_post((t_object*)x, "found buffer with name '%s'", s->s_name);
        x->buf_name = s;
        return MAX_ERR_NONE;
    }
    object_error((t_object*)x, "buffer with name '%s' not found", s->s_name);
    return MAX_ERR_GENERIC;
}


void buf_resize(t_buf* x, t_symbol* s, long argc, t_atom* argv)
{
    defer_low(x, (method)buf_do_resize, s, argc, argv);
}


void buf_do_resize(t_buf* x, t_symbol* s, long argc, t_atom* argv)
{
    t_buffer_obj* buf_obj = NULL;
    t_symbol* buf_name = NULL;
    t_atom_long new_size = 0;
    t_max_err err = 0;

    if (argc < 2) {
        object_error((t_object*)x, "need at least 2 args to resize buffer");
        goto error;
    }

    if ((argv + 0)->a_type != A_SYM) {
        object_error((t_object*)x,
                     "1st arg of buf_resize needs to be a symbol name of the "
                     "target buffer");
        goto error;
    }

    // retrieve params
    buf_name = atom_getsym(argv);
    if (buf_name == NULL) {
        object_error((t_object*)x, "buffer name not provided");
        goto error;
    }

    new_size = atom_getlong(argv + 1);
    if (new_size == 0) {
        object_error((t_object*)x, "cannot resize buffer '%s' to 0 samples", buf_name->s_name);
        goto error;
    }

    err = buf_set_buf(x, buf_name);
    if (err) {
        object_error((t_object*)x, "cannot resize unknown buffer '%s'",
                     buf_name->s_name);
        goto error;
    }

    buf_obj = buffer_ref_getobject(x->buf_ref);

    object_post((t_object*)x, "to resize buffer '%s' from %d to %d samples", 
        buf_name->s_name, buffer_getframecount(buf_obj), new_size);

    err = buffer_edit_begin(buf_obj);
    if (err) {
        object_error((t_object*)x, "cannot start editing buffer '%s'",
                     buf_name->s_name);
        goto error;        
    }

    err = object_method_typed((t_object*)buf_obj,
        gensym("sizeinsamps"), 1, argv+1, NULL);

    if (err) {
        object_error((t_object*)x, "failed to resize buffer '%s'",
                     buf_name->s_name);
        goto error;
    }

    err = buffer_edit_end(buf_obj, 1); // second arg is 'valid' with positive=TRUE
    if (err) {
        object_error((t_object*)x, "cannot end editing buffer %s",
                     buf_name->s_name);
        goto error;        
    }

    buffer_setdirty(buf_obj);

    // success
    object_post((t_object*)x, "successfully resized buffer '%s' to %d samples",
        buf_name->s_name, new_size);
    buf_bang(x);
    goto cleanup;

error:
    object_error((t_object*)x, "buf_do_resize failed");

cleanup:
    if (x->buf_ref)
        object_free(x->buf_ref);
    x->buf_ref = NULL;
}
