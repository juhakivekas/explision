#include "shape.h"
#include <malloc.h>

shape* shape_new(int n){
	shape* s      = (shape*)  malloc(sizeof(shape));
	s->ncrn       = n;
	s->crn        = (vector*) malloc(n * sizeof(vector));
	s->crn_inset  = (vector*) malloc(n * sizeof(vector));
	s->conn       = (int*)    malloc(n * sizeof(int));
	s->conn_t     = (float*)  malloc(n * sizeof(float));
	s->conn_inset = (float*)  malloc(n * sizeof(float));
	return s;
}

void shape_destroy(shape* s){
	free(s->crn);
	free(s->crn_inset);
	free(s->conn);
	free(s->conn_t);
	free(s->conn_inset);
	free(s);
}
