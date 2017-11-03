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
	unsigned long length;
	float* scal;
}vec;

vec* malloc_vector(float* array , int length);


#endif /* VECTOR_H_ */
