#include "ply_io.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>

//find the index of the first appearance of
//the element 'e' in array 'arr'
int indexof(void* e, void** arr, int arrlen){
	int i = 0;
	while(arr[i] != e && i<arrlen) i++;
	//if we can't find the element, then return 0
	if(i == arrlen) i=0;
	return i;
}


///////////////////  OUTPUT  ////////////////////////
void ply_write_head(FILE* fp, int numverts, int numfaces){
	fputs("ply\n", fp);
	fputs("format ascii 1.0\n", fp);
	fprintf(fp, "element vertex %d\n", numverts);
	fputs("property float x\n", fp);
	fputs("property float y\n", fp);
	fputs("property float z\n", fp);
	fprintf(fp, "element face %d\n", numfaces);
	fputs("property list uchar int vertex_index\n", fp);
	fputs("end_header\n", fp);
}

void ply_write_model(model* m, char* filename){
	
	vertex** verts = m->verts;
	int numverts = m->nverts;
	face** faces = m->faces;
	int numfaces = m->nfaces;

	FILE* fp = fopen(filename, "w");
	ply_write_head(fp, numverts, numfaces);
	int i;
	for(i=0; i<numverts; i++){
		fprintf(fp, "%.2f %.2f %.2f\n",
			verts[i]->x,
			verts[i]->y,
			verts[i]->z
		);
	}

	int index_A, index_B, index_C;
	for(i=0; i<numfaces; i++){
		index_A = indexof((void*)faces[i]->verts[0], (void**)verts, numverts);
		index_B = indexof((void*)faces[i]->verts[1], (void**)verts, numverts);
		index_C = indexof((void*)faces[i]->verts[2], (void**)verts, numverts);
		fprintf(fp, "3 %d %d %d\n",
			index_A,
			index_B,
			index_C
		);
	}
}

/////////////////////  INPUT  ////////////////////////

model* ply_read_model(char* filename){
	FILE* fp = fopen(filename, "r");
	char line[100];
	model* m = malloc(sizeof(model));

	fgets(line, 100, fp);
	if(strncmp(line, "ply", 3) != 0)
		fputs("ply_read_faces: file doesn't seem to be a .ply file\n", stderr);
	
	do fgets(line, 100, fp);
	while(strncmp(line, "element", 7) != 0);
	sscanf(line, "%*s %*s %d", &(m->nverts));

	do fgets(line, 100, fp);
	while(strncmp(line, "element", 7) != 0);
	sscanf(line, "%*s %*s %d", &(m->nfaces));

	do fgets(line, 100, fp);
	while(strncmp(line, "end_header", 7) != 0);

	m->verts = (vertex**) malloc(m->nverts * sizeof(vertex*));
	m->faces = (face**)   malloc(m->nfaces * sizeof(face*  ));

	double x, y, z;
	int i;
	for(i=0; i<m->nverts; i++){
		fgets(line, 100, fp);
		sscanf(line, "%lf %lf %lf", &x, &y, &z);
		m->verts[i] = vertex_new(x, y, z);
		//vertex_debug_print(m->verts[i]);
	}
	int A, B, C;
	for(i=0; i<m->nfaces; i++){
		fgets(line, 100, fp);
		sscanf(line, "%*d %d %d %d", &A, &B, &C);
		m->faces[i] = face_new(
			m->verts[A],
			m->verts[B],
			m->verts[C]
		);
	}
	return m;
}
