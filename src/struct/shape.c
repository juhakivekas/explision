#include "shape.h"
#include <malloc.h>

shape* shape_new(int n){
	shape* s = malloc(sizeof(shape));
	s->ncrn = n;
	s->crn = malloc(n * sizeof(vector));
	s->crn_inset = malloc(n * sizeof(vector));
	s->conn = malloc(n * sizeof(int));
	s->conn_t = malloc(n * sizeof(float));
	s->conn_inset = malloc(n * sizeof(float));
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
