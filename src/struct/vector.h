#ifndef VECTOR_H
#define VECTOR_H
 
typedef struct vector{
	double x, y;
}vector;

//adds u to v
void vector_add(vector* v, vector* u);

//adds a multiple of u to v
void vector_add_mul(vector* v, vector* u, double f);

//subtracts u from v
void vector_sub(vector* v, vector* u);

//multiplies v by a fector of f
void vector_mul(vector* v, double f);

//sets v to have the value of u
void vector_set(vector* v, vector* u);

//rotates the vector a quarter CCW
void vector_normal(vector* v);

//returns the distance of the vector
double vector_distance(vector* v);

//makes the vector a unit vector
void vector_unit(vector* v);

//returns the dot product of a and b
double vector_dot_product(vector* a, vector*b);

#endif /*VECTOR_H*/
