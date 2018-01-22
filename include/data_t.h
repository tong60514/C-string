/*
 * data_t.h
 *
 *  Created on: Nov 4, 2017
 *      Author: tong60514
 */

#ifndef DATA_T_H_
#define DATA_T_H_


#include <polynomial.h>
#include <matrix.h>
#include <vector.h>
#include <bitmap.h>
#include <meta.h>



typedef union data_type data_t;

typedef struct matrix mat;
typedef struct vector vec;

typedef struct polynomial plyml;
typedef union data_type
{
	struct metadata meta;
	mat matrix;
	plyml polynomial;
	vec vector;
}data_t;




void free_data_t(data_t* d);
#define FREE_T(d) free_data_t((data_t*)d);







#endif /* DATA_T_H_ */
