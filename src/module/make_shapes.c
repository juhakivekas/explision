#include "make_shapes.h"	
#include <iostream>

void project_to(ExpMesh model, const OpenMesh::FaceHandle &fh, shape* s){
	//std::cout << fh.idx() << std::endl;
	//too compact to be understandable?
	s->crn[0].x = 0;
	s->crn[0].y = 0;
	s->crn[1].x = 100;//length of the first side
	s->crn[1].y = 0;
	s->crn[2].x = 50; //vertex_dot_product(&v, &u)/ s->crn[1].x;
	s->crn[2].y = 50; //sqrt(vertex_dot_product(&u, &u) - (s->crn[2].x * s->crn[2].x));
}
/*
void inset(shape* s){
	//TODO: seriouysly! write down the math somewhere!
	int i;
	double cos_t;
	vector a_unit, b_unit, b_norm;
	int curr_i, next_i, prev_i;
	vector *curr, *next, *prev;
	for(i=0; i<s->ncrn; i++){
		prev_i = (i+0)%s->ncrn;
		curr_i = (i+1)%s->ncrn;
		next_i = (i+2)%s->ncrn;
		prev = &(s->crn[prev_i]);
		curr = &(s->crn[curr_i]);
		next = &(s->crn[next_i]);

		a_unit.x = next->x - curr->x;
		a_unit.y = next->y - curr->y;
		vector_unit(&a_unit);
		b_unit.x = prev->x - curr->x;
		b_unit.y = prev->y - curr->y;
		vector_unit(&b_unit);

		b_norm.x = -b_unit.y;
		b_norm.y =  b_unit.x;

		cos_t = 1/vector_dot_product(&a_unit, &b_norm);
		
		s->crn_inset[curr_i].x = curr->x;
		s->crn_inset[curr_i].y = curr->y;
		vector_add_mul(&s->crn_inset[curr_i], &a_unit, -cos_t * s->conn_inset[prev_i] * MAT/2);
		vector_add_mul(&s->crn_inset[curr_i], &b_unit, -cos_t * s->conn_inset[curr_i] * MAT/2);
	}
}
*/
void make_shapes(ExpMesh &model, shape** shapes){
	shape* s;
	int j, i=0;
	for(auto fIt = model.faces_begin(); fIt != model.faces_end(); ++fIt, ++i){
		//----allocate space for the shape
		shapes[i] = shape_new(3);
		s = shapes[i];
		//----do the spacial projection
		project_to(model, *fIt, s);
		//----get adjacent faces and their angles
		j=0;
		for(auto feIt = model.fh_cwiter(*fIt); feIt.is_valid(); ++feIt, ++j){
			s->conn[j]   = model.opposite_face_handle(*feIt).idx();
			s->conn_t[j] = model.data(model.edge_handle(*feIt)).angle();
/*
			s->conn_inset[j] = tan(s->conn_t[j]/2);
			if(s->conn_inset[j] < 0){
				s->conn_inset[j] = -s->conn_inset[j];
			}
*/
		}
	}
}
