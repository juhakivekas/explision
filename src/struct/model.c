#include <malloc.h>
#include <math.h>

#include "model.h"

//------------------------------------------------FACES-----------------------
face* face_new(vertex* A, vertex* B, vertex* C){
	face* f = malloc(sizeof(face));
	f->verts  = malloc(3 * sizeof(vertex*));
	f->verts[0] = A;
	f->verts[1] = B;
	f->verts[2] = C;
	f->normal = malloc(sizeof(vertex));
	f->nverts = 3;
	return f;
};

void face_normal(face* f){
	//make vectors for edges AB and AC
	vertex AB = {
		f->verts[1]->x - f->verts[0]->x,
		f->verts[1]->y - f->verts[0]->y,
		f->verts[1]->z - f->verts[0]->z
	};
	vertex AC = {
		f->verts[2]->x - f->verts[0]->x,
		f->verts[2]->y - f->verts[0]->y,
		f->verts[2]->z - f->verts[0]->z
	};
	vertex_cross_product(&AB, &AC, f->normal);
	vertex_normalize(f->normal);
	return;
}

//------------------------------------------------VERTICES--------------------
vertex* vertex_new(double x, double y, double z){
	vertex* v = malloc(sizeof(vertex));
	v->x = x;
	v->y = y;
	v->z = z;
	return v;
};

double vertex_dot_product(vertex* a, vertex* b){
	return (
		a->x * b->x +
		a->y * b->y +
		a->z * b->z
	);
}
double vertex_distance(vertex* a, vertex* b){
	return sqrt(
		(a->x - b->x) * (a->x - b->x) +
		(a->y - b->y) * (a->y - b->y) +
		(a->z - b->z) * (a->z - b->z)
	);
}

double vertex_distance_origin(vertex* a){
	return sqrt(
		a->x * a->x +
		a->y * a->y +
		a->z * a->z
	);
}

void vertex_normalize(vertex* a){
	double distance = vertex_distance_origin(a);
	if(distance == 0) return;
	a->x /= distance;
	a->y /= distance;
	a->z /= distance;
}

//returns the vector product of a and b
void vertex_cross_product(vertex* a, vertex* b, vertex* result){
	result->x = a->y*b->z - a->z*b->y,
	result->y = a->z*b->x - a->x*b->z,
	result->z = a->x*b->y - a->y*b->x;
}

