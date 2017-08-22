#ifndef material_h
#define material_h

//Material definitions
#define WIDTH 900
#define HEIGHT 610
//thickness
#define MAT	6.55

//kerf is the "thickness" of the lazzzerrrr, this should usually be beteeen 0.10 and 0.30
//adjusting this needs a lot of experimenting and changes drastically with different cutters and materials
#define KERF 0.16
#define MAT_IN  (MAT-KERF)
#define MAT_OUT (MAT+KERF)

//the number of connectors per side
#define NUM_HOLES 2
#endif /*material_h*/

