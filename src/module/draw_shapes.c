#include "draw_shapes.h"

void draw_holes(FILE* fp, vector* a, vector* b, float inset){
	vector v = {b->x - a->x, b->y - a->y};	//original vector v
	vector vu = {v.x, v.y};				//unit vector vu
	vector_unit(&vu);
	vector vn = {-vu.y, vu.x};				//normal vector vn
	fprintf(fp, "M %.2f %.2f ", a->x, a->y);
	fprintf(fp, "m %.2f %.2f ",
		v.x/3 - vu.x*MAT_IN/2 + vn.x*(1+inset)*MAT_OUT,
		v.y/3 - vu.y*MAT_IN/2 + vn.y*(1+inset)*MAT_OUT);

	fprintf(fp, "l %.2f %.2f ",  vn.x*MAT_IN,  vn.y*MAT_IN); //sides 1
	fprintf(fp, "l %.2f %.2f ",  vu.x*MAT_IN,  vu.y*MAT_IN); //2
	fprintf(fp, "l %.2f %.2f ", -vn.x*MAT_IN, -vn.y*MAT_IN); //3
	fprintf(fp, "l %.2f %.2f ", -vu.x*MAT_IN, -vu.y*MAT_IN); //4

	fprintf(fp, "m %.2f %.2f ", v.x/3, v.y/3);

	fprintf(fp, "l %.2f %.2f ",  vn.x*MAT_IN,  vn.y*MAT_IN); //sides 1
	fprintf(fp, "l %.2f %.2f ",  vu.x*MAT_IN,  vu.y*MAT_IN); //2
	fprintf(fp, "l %.2f %.2f ", -vn.x*MAT_IN, -vn.y*MAT_IN); //3
	fprintf(fp, "l %.2f %.2f ", -vu.x*MAT_IN, -vu.y*MAT_IN); //4	
}

//HARDCODE: only triangles
void draw_shapes(shape** shapes, transform* layout, int nshapes){
	FILE* fp = fopen("design_files/shapes.svg", "w");
	int i, j;
	char info[100];
	shape* s;
	svg_head(fp,  WIDTH*PT_TO_MM, HEIGHT*PT_TO_MM);
	fprintf(fp, "<g id=\"file_style\" fill=\"none\" transform=\"scale(%f)\">", PT_TO_MM);
	for(i=0; i<nshapes; i++){
		s = shapes[i];
	
		fprintf(fp,										//start container
			"<g transform=\"translate(%.4f %.4f)scale(%d)\">",
			layout[i].x, layout[i].y,
			layout[i].flip==0 ? 1:-1
		);
	

		fprintf(fp, "<path stroke=\"black\" d=\"");		//start edges
		fprintf(fp, "M %.2f %.2f ", s->crn[0].x, s->crn[0].y);
		for(j=1; j<s->ncrn; j++){
			fprintf(fp, "L %.2f %.2f ",
				s->crn[j].x, s->crn[j].y
			);
		}
		fprintf(fp, "z\" />\n");						//close edges

		fprintf(fp, "<path stroke=\"blue\" d=\"");		//start holes
		for(j=0; j<s->ncrn; j++){
			draw_holes(fp, &s->crn[j], &s->crn[(j+1)%s->ncrn], s->conn_inset[j]);
		}
		fprintf(fp, "\" />\n");							//close holes

		sprintf(info, "%3d--->\n%3d%4.0f\n%3d%4.0f\n%3d%4.0f",
			i,
			s->conn[0], s->conn_t[0]/TWO_PI*360, 
			s->conn[1], s->conn_t[1]/TWO_PI*360, 
			s->conn[2], s->conn_t[2]/TWO_PI*360
		);
		svg_puts(fp, info, 
			(s->crn[0].x+s->crn[1].x)/2 - 3.5*2,
			(s->crn[0].y+s->crn[1].y)/2 + 2);
		fprintf(fp, "</g>\n");							//close container
	}
	fprintf(fp, "</g>\n");
	svg_foot(fp);
	fclose(fp);
}

