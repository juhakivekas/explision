#include "pack_shapes.h"
void pack_shapes_line(shape** shapes, transform* layout, int nshapes){
	int i;
	double max_h = 0;
	double posx = 0, posy = 0;
	for(i=0; i<nshapes; i++){
		if(posx + shapes[i]->crn[1].x > WIDTH){
			posx = 0;
			posy += max_h;
			max_h = 0;
		}
		layout[i].x = posx;
		layout[i].y = posy;
		layout[i].flip = 0;

		posx += shapes[i]->crn[1].x;
		if(shapes[i]->crn[2].y > max_h){
			max_h = shapes[i]->crn[2].y;
		}
	}
}

void pack_shapes(shape** shapes, transform* layout, int nshapes){
	pack_shapes_line(shapes, layout, nshapes);
}

/*
void pack_shapes_zigzag(shape** shapes, transform* layout, int nshapes){
	double posx, posy, theta;
	//start with the highest triangle
		//calculate posx and posy
		//calculate theta
		//try all triangles and select best fit
		//place the triangle and mark it used
	//restart when we hit the end of material (posx > WIDTH)
}

double angle_BAC(shape* s){
	
}
*/
