#ifndef shape_h
#define shape_h
#include "vector.h"

typedef struct shapie{
	vector* crn;
	int ncrn;
	int* conn;
	float* conn_t;
}shape;

shape* shape_new(int n);
#endif /*shape_h*/
