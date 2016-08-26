#ifndef make_shapes_h
#define make_shapes_h

//system dependencies
#include <math.h>
#include <stdlib.h>

//project dependencies
#include "../material.h"
#include "../struct/shape.h"
#include "../struct/vector.h"
#include "../ExpMesh.hh"

void make_shapes(ExpMesh &m, shape** shapes);

#endif /*make_shapes_h*/
