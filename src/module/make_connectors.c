#include "make_connectors.h"

void print_connector_table(int nshapes, shape** shapes){
	FILE* fp = fopen("design_files/connections.txt", "w");

	int i, j;
	fprintf(fp, "Shape connective table\n");
	fprintf(fp, "Number of shapes: %d\n", nshapes);
	for(i=0; i<nshapes; i++){
		fprintf(fp, "%4d:", i);
		for(j=0; j<shapes[i]->ncrn; j++){
			fprintf(fp, "%6d, %6.2f",
					shapes[i]->conn[j],
				shapes[i]->conn_t[j]);
		}
		fprintf(fp, "\n");
	}
}

void conn_t1(FILE* fp, double theta){
	vector vu = {cos(theta), sin(theta)};
	vector vn = {vu.y     , -vu.x      };

	fprintf(fp, "M %.2f %.2f ", 0.0    , MAT );
	fprintf(fp, "l %.2f %.2f ", 0.0    , -MAT);
	fprintf(fp, "l %.2f %.2f ", MAT_OUT, 0.0 );
	fprintf(fp, "l %.2f %.2f ", 0.0    , MAT );
	fprintf(fp, "l %.2f %.2f ", MAT    , 0.0 );

	fprintf(fp, "l %.2f %.2f ", vu.x*MAT    , vu.y*MAT    );
	fprintf(fp, "l %.2f %.2f ", vn.x*MAT    , vn.y*MAT    );
	fprintf(fp, "l %.2f %.2f ", vu.x*MAT_OUT, vu.y*MAT_OUT);
	fprintf(fp, "l %.2f %.2f ",-vn.x*MAT    ,-vn.y*MAT    );

	fprintf(fp, "z");
}

void conn_t2(FILE* fp, double theta){
	vector vu = {cos(theta), sin(theta)};
	vector vn = {vu.y     , -vu.x      };

	fprintf(fp, "M %.2f %.2f ", 0.0    , 2*MAT );
	fprintf(fp, "l %.2f %.2f ", 0.0    ,-2*MAT );
	fprintf(fp, "l %.2f %.2f ", MAT_OUT, 0.0   );
	fprintf(fp, "l %.2f %.2f ", 0.0    , MAT   );
	fprintf(fp, "l %.2f %.2f ", MAT    , 0.0   );

	fprintf(fp, "l %.2f %.2f ", vu.x*MAT    , vu.y*MAT    );
	fprintf(fp, "l %.2f %.2f ", vn.x*MAT    , vn.y*MAT    );
	fprintf(fp, "l %.2f %.2f ", vu.x*MAT_OUT, vu.y*MAT_OUT);
	fprintf(fp, "l %.2f %.2f ",-2*vn.x*MAT  ,-2*vn.y*MAT  );

	fprintf(fp, "z");
}

void conn_t3(FILE* fp, double theta){
	vector vu = {cos(theta), sin(theta)};
	vector vn = {vu.y     , -vu.x      };

	fprintf(fp, "M %.2f %.2f ", MAT    , 2*MAT );
	fprintf(fp, "l %.2f %.2f ",-MAT    , 0.0   );
	fprintf(fp, "l %.2f %.2f ", 0.0    ,-2*MAT );
	fprintf(fp, "l %.2f %.2f ", MAT_OUT, 0.0   );
	fprintf(fp, "l %.2f %.2f ", 0.0    , MAT   );
	fprintf(fp, "l %.2f %.2f ", MAT    , 0.0   );

	fprintf(fp, "l %.2f %.2f ", vu.x*MAT    , vu.y*MAT    );
	fprintf(fp, "l %.2f %.2f ", vn.x*MAT    , vn.y*MAT    );
	fprintf(fp, "l %.2f %.2f ", vu.x*MAT_OUT, vu.y*MAT_OUT);
	fprintf(fp, "l %.2f %.2f ",-2*vn.x*MAT  ,-2*vn.y*MAT  );
	fprintf(fp, "l %.2f %.2f ",-vu.x*MAT    ,-vu.y*MAT    );

	fprintf(fp, "z");
}

void conn_t4(FILE* fp, double theta){
	vector vu = {cos(theta), sin(theta)};
	vector vn = {vu.y     , -vu.x      };

	fprintf(fp, "M %.2f %.2f ", 2*MAT  , 2*MAT );
	fprintf(fp, "l %.2f %.2f ",-2*MAT  , 0.0   );
	fprintf(fp, "l %.2f %.2f ", 0.0    ,-2*MAT );
	fprintf(fp, "l %.2f %.2f ", MAT_OUT, 0.0   );
	fprintf(fp, "l %.2f %.2f ", 0.0    , MAT   );
	fprintf(fp, "l %.2f %.2f ", MAT_IN , 0.0   );
	fprintf(fp, "l %.2f %.2f ", 0.0    ,-MAT   );

	fprintf(fp, "l %.2f %.2f ",-vn.x*MAT    ,-vn.y*MAT    );
	fprintf(fp, "l %.2f %.2f ", vu.x*MAT_IN , vu.y*MAT_IN );
	fprintf(fp, "l %.2f %.2f ", vn.x*MAT    , vn.y*MAT    );
	fprintf(fp, "l %.2f %.2f ", vu.x*MAT_OUT, vu.y*MAT_OUT);
	fprintf(fp, "l %.2f %.2f ",-2*vn.x*MAT  ,-2*vn.y*MAT  );
	fprintf(fp, "l %.2f %.2f ",-2*vu.x*MAT  ,-2*vu.y*MAT  );

	fprintf(fp, "z");
}

void make_connectors(int nshapes, shape** shapes){
	print_connector_table(nshapes, shapes);
	
	//count and make a list of connectors
	int i;
	int nconn = 0;
	for(i=0; i<nshapes; i++){
		nconn += shapes[i]->ncrn;
	}
	double connectors[nconn];
	int j, k=0;
	for(i=0; i<nshapes; i++){
		for(j=0; j<shapes[i]->ncrn; j++){
			connectors[k++] = shapes[i]->conn_t[j];
		}
	}
	//sort the connectors (bubble sort)
	double tmp;
	for(i=0; i<nconn; i++){
		for(j=0; j<nconn-i-1; j++){
			if(connectors[j] > connectors[j+1]){
				tmp = connectors[j];
				connectors[j] = connectors[j+1];
				connectors[j+1] = tmp;
			}
		}
	}

	//draw the connectors to an SVG
	FILE* fp = fopen("design_files/connectors.svg", "w");
	svg_head(fp, 200, 200);
	//make the outlines of each connector
	char text[10];
	fprintf(fp, "<g transform=\"scale(%f)\">", PT_TO_MM);
	for(i=0; i<nconn; i++){
		//start container group
		fprintf(fp, "<g fill=\"none\" stroke=\"black\" transform=\"translate(%.2f, %.2f)\">\n", 0.0, i*10.0);
		//start outline
		fprintf(fp, "<path  d=\"");
		//find out what connector type to use and print the path
		if(     connectors[i] > TWO_PI/3 ) conn_t1(fp, connectors[i]);
		else if(connectors[i] > TWO_PI/4 ) conn_t2(fp, connectors[i]);
		else if(connectors[i] > 0        ) conn_t3(fp, connectors[i]);
		else if(connectors[i] > -TWO_PI/4) conn_t4(fp, connectors[i]);
		//under -90 degree connections will be on the outside of the model
		else if(connectors[i] > -TWO_PI/3) conn_t2(fp, connectors[i]);
		else                               conn_t1(fp, connectors[i]);
		//finish outline path
		fprintf(fp, "\"/>\n");
		//add the angle as text
		sprintf(text, "%d", (int) ((connectors[i]/TWO_PI)*360));
		svg_puts(fp, text, 0.5, 5);
		//close container group
		fprintf(fp, "</g>\n");
	}
	fprintf(fp, "</g>\n");
	svg_foot(fp);
	fclose(fp);
}
