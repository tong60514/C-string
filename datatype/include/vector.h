/*
 * vector.h
 *
 *  Created on: Oct 18, 2017
 *      Author: tong60514
 */

#ifndef VECTOR_H_
#define VECTOR_H_
#include <meta.h>
typedef struct vector
{
	struct metadata meta;
	long length;
	double* val;
}vec;

vec* vector(double* array , int length);
vec* zero_vector(int length);
void init_zero_vector(vec* v,int length);
double norm(vec* a);
void normalize(vec *v);
void vec_sub(vec* a,vec* b,vec* res);
vec* vec_sub_vec(vec* a,vec* b);
void sub_to(vec* src,vec* x);
vec* vec_mul_c(vec* v,double c);
void vec_write(vec* from,vec* to);
double dot(vec* ,vec*);
void vec_mul_const(vec* v,double c,vec* res);
#endif /* VECTOR_H_ */
