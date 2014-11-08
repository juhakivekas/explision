#include "vector.h"
#include <math.h>

//adds u to v
void vector_add(vector* v, vector* u){
	v->x += u->x;
	v->y += u->y;
}

void vector_add_mul(vector* v, vector* u, double f){
	v->x += f * u->x;
	v->y += f * u->y;
}

//subtracts u from v
void vector_sub(vector* v, vector* u){
	v->x -= u->x;
	v->y -= u->y;
}

//multiplies the vector v by a real number
void vector_mul(vector* v, double f){
	v->x *= f;
	v->y *= f;
}

//sets v to have the value of u
void vector_set(vector* v, vector* u){
	v->x = u->x;
	v->y = u->y;
}

//rotates the vector a quarter CCW
void vector_normal(vector* v){
	double tmp = v->x;
	v->x = v->y;
	v->y = -tmp;
}

//returns the distance of the vector
double vector_distance(vector* v){
	return hypot(v->x, v->y);
}

//makes the vector a unit vector
void vector_unit(vector* v){
	vector_mul(v, 1/vector_distance(v));
}

double vector_dot_product(vector* a, vector* b){
	return a->x * b->x + a->y * b->y;
}
