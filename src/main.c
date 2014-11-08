#include "main.h"
		#include <string.h>

int main(int argc, char* argv[]){
	model* m = ply_read_model(argv[1]);
	int i = 0;
	for(i=0; i<m->nfaces; i++){
		face_normal(m->faces[i]);
	}

	int nshapes = m->nfaces;
	shape* shapes[nshapes];			//allocate shape pointers
	make_shapes(m, shapes);				//create, project and inset shapes

	make_connectors(nshapes, shapes);	//output connectors to file

	transform layout[nshapes];			//layout is an array of transformations
	pack_shapes(shapes, layout, nshapes);	//calculate a good layout
	draw_shapes(shapes, layout, nshapes);	//draw shapes according to the layout
	return 0;	
}

