#ifndef svg_io_h
#define svg_io_h

//system dependencies
#include <stdio.h>

void svg_head(FILE* fp, int width, int height);
void svg_foot(FILE* fp);
void svg_puts(FILE* fp, char* text, double ox, double oy);

#endif /*svg_io_h*/
