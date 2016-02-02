#ifndef shape_h
#define shape_h
#include "vector.h"

typedef struct shapie{
	vector* crn;
	vector* crn_inset;
	int ncrn;
	int* conn;
	float* conn_t;
	float* conn_inset;
}shape;

shape* shape_new(int n);
void shape_destroy(shape* s);
#endif /*shape_h*/
