#include "make_shapes.h"	

void order_corners(face* f){
	//expects triangles
	double maxdist = 0;
	double dist;
	int i;
	int max_i = 0;
	for (i=0; i<3; i++){
		dist = vertex_distance(f->verts[i], f->verts[(i+1)%3]);
		if(dist > maxdist){
			maxdist = dist;
			max_i = i;
		}
	}

	//can be done with less tmp variables, but i wonder if its faster
	vertex* A = f->verts[(max_i+0)%3];
	vertex* B = f->verts[(max_i+1)%3];
	vertex* C = f->verts[(max_i+2)%3];

	f->verts[0] = A;
	f->verts[1] = B;
	f->verts[2] = C;
}

void project_to(face* f, shape* s){
	vertex v = {f->verts[1]->x -f->verts[0]->x, f->verts[1]->y -f->verts[0]->y, f->verts[1]->z -f->verts[0]->z};
	vertex u = {f->verts[2]->x -f->verts[0]->x, f->verts[2]->y -f->verts[0]->y, f->verts[2]->z -f->verts[0]->z};

	//too compact to be understandable?
	s->crn[0].x = 0;
	s->crn[0].y = 0;
	s->crn[1].x = sqrt(vertex_dot_product(&v, &v));
	s->crn[1].y = 0;
	s->crn[2].x = vertex_dot_product(&v, &u)/ s->crn[1].x;
	s->crn[2].y = sqrt(vertex_dot_product(&u, &u) - (s->crn[2].x * s->crn[2].x));
}

int find_neighbour(int num, face* f, model* m){
	vertex* v = f->verts[num];
	vertex* u = f->verts[(num+1)%f->nverts];
	int i;
	for(i=0; i<m->nfaces; i++){
		if((m->faces[i]->verts[0] == v ||
			m->faces[i]->verts[1] == v ||
			m->faces[i]->verts[2] == v)
			&&
			(m->faces[i]->verts[0] == u ||
			m->faces[i]->verts[1] == u ||
			m->faces[i]->verts[2] == u)){
			if(m->faces[i] != f)
				return i;
		}
	}
	return -1;
}

double angle(face* f, face* g){
	int common = 
		(g->verts[0] == f->verts[0] || g->verts[0] == f->verts[1] || g->verts[0] == f->verts[2])   +
		(g->verts[1] == f->verts[0] || g->verts[1] == f->verts[1] || g->verts[1] == f->verts[2])*2 +
		(g->verts[2] == f->verts[0] || g->verts[2] == f->verts[1] || g->verts[2] == f->verts[2])*4;
	vertex* uc;		//uc as in UnCommon
	switch(common){
		case 6: uc = g->verts[0]; break;
		case 5: uc = g->verts[1]; break;
		case 3: uc = g->verts[2]; break;}
	vertex v = {
		uc->x - f->verts[0]->x,
		uc->y - f->verts[0]->y,	
		uc->z - f->verts[0]->z};

	double theta = acos(vertex_dot_product(f->normal, g->normal));
	theta *= vertex_dot_product(&v, f->normal)<0 ? 1: -1;
	return theta;
}

void inset(shape* s){
	vector new_crn[s->ncrn];
	int i;
	double cos_t;
	vector a_unit, b_unit, b_norm;
	vector *curr, *next, *prev;
	for(i=0; i<s->ncrn; i++){
		prev = &(s->crn[(i+0)%s->ncrn]);
		curr = &(s->crn[(i+1)%s->ncrn]);
		next = &(s->crn[(i+2)%s->ncrn]);

		a_unit.x = next->x - curr->x;
		a_unit.y = next->y - curr->y;
		vector_unit(&a_unit);
		b_unit.x = prev->x - curr->x;
		b_unit.y = prev->y - curr->y;
		vector_unit(&b_unit);

		b_norm.x = -b_unit.y;
		b_norm.y =  b_unit.x;

		cos_t = 1/vector_dot_product(&a_unit, &b_norm);
		
		new_crn[(i+1)%s->ncrn].x = curr->x;
		new_crn[(i+1)%s->ncrn].y = curr->y;
		vector_add_mul(&new_crn[(i+1)%s->ncrn], &a_unit, -cos_t * tan(s->conn_t[(i+0)%s->ncrn]/2) * MAT);
		vector_add_mul(&new_crn[(i+1)%s->ncrn], &b_unit, -cos_t * tan(s->conn_t[(i+1)%s->ncrn]/2) * MAT);
	}
	for(i=0; i<s->ncrn; i++){
		s->crn[i].x = new_crn[i].x - new_crn[0].x;
		s->crn[i].y = new_crn[i].y - new_crn[0].y;
	}
}

void make_shapes(model* m, shape** shapes){
	shape* s;
	face*  f;
	int i, j;
	for(i=0; i<m->nfaces; i++){
		f = m->faces[i];
		//----allocate space for the shape
		shapes[i] = shape_new(f->nverts);
		s = shapes[i];
		//----order the corners so that the first side is the longest
		order_corners(f);
		//----do the spacial projection
		project_to(f, s);
		//----get adjacent faces and their angles
		for(j=0; j<s->ncrn; j++){
			//find f's neighbour number j in m
			s->conn[j] = find_neighbour(j, f, m);
			if(s->conn[j] != -1){
				s->conn_t[j] = angle(f, m->faces[s->conn[j]]);
			}
		}
		//----inset sides to enable building concave shapes
		//placement of connector holes is not as simple when we inset the tri's
		//uncomment for futher development
		//inset(s);
	}
}
