#include "svg_io.h"

#define SPACE 0.5

const char* paths[14] = {
	"m 0 3 l 1.5 -3 l 0 3 l -1.5 0 l 0 -3 l 1.5 0 ",
	"m 0.75 0 l 0 3 m 0.75 -3 ",
	"l 1.5 0 l 0 1.5 l -1.5 0 l 0 1.5 l 1.5 0 m 0 -3 ",
	"l 1.5 0 l -1.5 1.5 l 1.5 0 l 0 1.5 l -1.5 0 m 1.5 -3 ",
	"m 1.5 1.5 l -1.5 0 l 1.5 -1.5 l 0 3 m 0 -3 ",
	"m 1.5 0 l -1.5 0 l 0 1.5 l 1.5 0 l 0 1.5 h -1.5 m 1.5 -3 ",
	"m 1.5 0 l -1.5 0 l 0 3 l 1.5 0 l 0 -1.5 l -1.5 0 m 1.5 -1.5 ",
	"l 1.5 0 l -1.5 3 m 1.5 -3 ",
	"m 0 1.5 l 1.5 0 l 0 1.5 l -1.5 0 l 0 -3 l 1.5 0 l 0 1.5 m 0 -1.5 ",
	"m 1.5 1.5 l -1.5 0 l 0 -1.5 l 1.5 0 l 0 3 l -1.5 0 m 1.5 -3 ",
	"m 1.5 0 ",
	"m 0 1.5 l 1.5 0 m 0 -1.5 ",
	"l 1.5 1.5 l -1.5 1.5 m 1.5 -3 ",
	"m 0.75 2.5 l 0 0.75 m 0.75 -3.25 "
};

void svg_puts(FILE* fp, char* text, double ox, double oy){
	const char* char_path;
	int line_char = 0;
	int i;
	fprintf(fp, "<path fill=\"none\" stroke=\"red\" stroke-width=\"0.3\" d=\"M %.2f %.2f ", ox, oy);
	for(i=0; text[i] != 0x00; i++){
		switch(text[i]){
		case '0':  char_path = paths[0]; break;
		case '1':  char_path = paths[1]; break;
		case '2':  char_path = paths[2]; break;
		case '3':  char_path = paths[3]; break;
		case '4':  char_path = paths[4]; break;
		case '5':  char_path = paths[5]; break;
		case '6':  char_path = paths[6]; break;
		case '7':  char_path = paths[7]; break;
		case '8':  char_path = paths[8]; break;
		case '9':  char_path = paths[9]; break;
		case ' ':  char_path = paths[10]; break;
		case '-':  char_path = paths[11]; break;
		case '>':  char_path = paths[12]; break;
		case '.':  char_path = paths[13]; break;
		case '\n': fprintf(fp, "m %.2f 4 ", -line_char * (1.5+SPACE)); line_char=0;break;
		}
		if(text[i] != '\n'){
			fprintf(fp, "%s m %.2f 0 ", char_path, SPACE);
			line_char++;
		}
	}
	fprintf(fp, "\"/>\n");
}

void svg_head(FILE* fp, int width, int height){
	fprintf(fp, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n");
	fprintf(fp, "<svg\n");
	fprintf(fp, "\tid=\"svg\"\n"); 
	fprintf(fp, "\tversion=\"1.1\"\n");
	fprintf(fp, "\txmlns=\"http://www.w3.org/2000/svg\"\n");	
	fprintf(fp, "\twidth=\"%d\" height=\"%d\"\n", width, height);
	fprintf(fp, ">\n");
}

void svg_foot(FILE* fp){
	fprintf(fp, "</svg>\n");
}

//test function
/*
int main(){
	FILE* fp = fopen("test.svg", "w");
	SVG_head(fp, 100, 100);
	svg_puts(fp, "01234\n56789\n --> ", 1, 10.0, 10.0);
	SVG_foot(fp);
	fclose(fp);
	return 0;
}
*/
