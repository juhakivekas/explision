#ifndef draw_shapes_h
#define draw_shapes_h

//system dependencies
#include <stdio.h>

//project dependencies
#include "../material.h"
#include "../magic.h"
#include "../io/svg_io.h"
#include "../struct/vector.h"
#include "../struct/shape.h"
#include "../struct/transform.h"

void draw_shapes(shape** shapes, transform* layout, int nshapes);

#endif /*draw_shapes_h*/
