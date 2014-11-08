#ifndef ply_io_h
#define ply_io_h

//system dependencies
#include <stdio.h>

//project dependencies
#include "../struct/model.h"

void ply_write_head(FILE* fp, int numverts, int numfaces);
void ply_write_model(model* m, char* filename);

model* ply_read_model(char* filename);

#endif /*ply_io_h*/
