#include "shape.h"
#include <malloc.h>

shape* shape_new(int n){
	shape* s = malloc(sizeof(shape));
	s->ncrn = n;
	s->crn = malloc(n * sizeof(vector));
	s->conn = malloc(n * sizeof(int));
	s->conn_t = malloc(n * sizeof(float));
	return s;
}
