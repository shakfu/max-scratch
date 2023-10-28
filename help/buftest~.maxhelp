{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 449.0, 44.0, 702.0, 710.0 ],
		"bglocked" : 0,
		"defrect" : [ 449.0, 44.0, 702.0, 710.0 ],
		"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
		"openinpresentation" : 0,
		"default_fontsize" : 10.0,
		"default_fontface" : 0,
		"default_fontname" : "Verdana",
		"gridonopen" : 0,
		"gridsize" : [ 5.0, 5.0 ],
		"gridsnaponopen" : 0,
		"toolbarvisible" : 1,
		"boxanimatetime" : 200,
		"imprint" : 0,
		"enablehscroll" : 1,
		"enablevscroll" : 1,
		"devicewidth" : 0.0,
		"boxes" : [ 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "500",
					"numoutlets" : 1,
					"fontsize" : 10.0,
					"outlettype" : [ "" ],
					"patching_rect" : [ 126.0, 130.0, 32.5, 17.0 ],
					"id" : "obj-5",
					"fontname" : "Verdana",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "click here:\nthis should resize the buffer and fill 33 periods of a sinewave into it",
					"linecount" : 4,
					"numoutlets" : 0,
					"fontsize" : 10.0,
					"patching_rect" : [ 273.0, 62.0, 156.0, 55.0 ],
					"id" : "obj-3",
					"fontname" : "Verdana",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numoutlets" : 2,
					"fontsize" : 10.0,
					"outlettype" : [ "int", "bang" ],
					"patching_rect" : [ 126.0, 156.0, 53.0, 19.0 ],
					"id" : "obj-14",
					"fontname" : "Verdana",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "buffer~ uuu 10 1",
					"numoutlets" : 2,
					"fontsize" : 10.0,
					"outlettype" : [ "float", "bang" ],
					"patching_rect" : [ 126.0, 239.0, 103.0, 19.0 ],
					"id" : "obj-6",
					"fontname" : "Verdana",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "fill 2000 6",
					"numoutlets" : 1,
					"fontsize" : 10.0,
					"outlettype" : [ "" ],
					"patching_rect" : [ 306.0, 179.0, 68.0, 17.0 ],
					"id" : "obj-4",
					"fontname" : "Verdana",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "fill 10000 33",
					"numoutlets" : 1,
					"fontsize" : 10.0,
					"outlettype" : [ "" ],
					"patching_rect" : [ 275.0, 123.0, 82.0, 17.0 ],
					"id" : "obj-2",
					"fontname" : "Verdana",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 275.0, 242.0, 19.0, 19.0 ],
					"id" : "obj-29",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "sizeinsamps $1",
					"numoutlets" : 1,
					"fontsize" : 10.0,
					"outlettype" : [ "" ],
					"patching_rect" : [ 126.0, 195.0, 87.0, 17.0 ],
					"id" : "obj-38",
					"fontname" : "Verdana",
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "waveform~",
					"numoutlets" : 6,
					"selectioncolor" : [ 0.619608, 0.741176, 0.643137, 0.5 ],
					"setunit" : 1,
					"outlettype" : [ "float", "float", "float", "float", "list", "" ],
					"clipdraw" : 1,
					"setmode" : 3,
					"outmode" : 0,
					"bgcolor" : [ 0.619608, 0.741176, 0.643137, 1.0 ],
					"patching_rect" : [ 38.0, 341.0, 547.0, 186.0 ],
					"buffername" : "uuu",
					"tickmarkcolor" : [ 0.392157, 0.392157, 0.392157, 1.0 ],
					"id" : "obj-45",
					"numinlets" : 5,
					"grid" : 22.675737,
					"waveformcolor" : [ 0.129412, 0.0, 0.0, 1.0 ],
					"textcolor" : [  ],
					"vlabels" : 1,
					"labelbgcolor" : [ 0.572549, 0.701961, 0.85098, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "buftest~ uuu",
					"numoutlets" : 1,
					"fontsize" : 12.0,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 275.0, 213.0, 88.0, 21.0 ],
					"id" : "obj-47",
					"fontname" : "Verdana",
					"numinlets" : 1
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-14", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-38", 0 ],
					"destination" : [ "obj-6", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-47", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-4", 0 ],
					"destination" : [ "obj-47", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-47", 0 ],
					"destination" : [ "obj-29", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-14", 0 ],
					"destination" : [ "obj-38", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
