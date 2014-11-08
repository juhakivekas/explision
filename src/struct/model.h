#ifndef model_h
#define model_h

/* user's vertex and face definitions for a polygonal object */

typedef struct vertex {
	float x,y,z;             /* the usual 3-space position of a vertex */
} vertex;

typedef struct face {
	unsigned char nverts;	/* number of vertecies in face */
	vertex** verts;       	/* vertex pointers */
	vertex *normal;
} face;

typedef struct model {
	vertex** verts;
	int nverts;
	face** faces;
	int nfaces;
} model;

//construct and destruct
vertex* vertex_new(double x, double y, double z);
face* face_new(vertex* A, vertex* B, vertex* C);

//manipulating vertecies
void vertex_normalize(vertex* a);
double vertex_dot_product(vertex* a, vertex* b);
double vertex_distance_origin(vertex* a);
double vertex_distance(vertex* a, vertex* b);
void vertex_cross_product(vertex* a, vertex* b, vertex* result);

//manipulating faces
void face_normal(face* f);

#endif /*model_h*/
