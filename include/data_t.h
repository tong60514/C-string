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
typedef struct polynomial plyml;
typedef struct matrix mat;
typedef struct vector vec;


typedef union data_type
{
	struct metadata meta;
	mat matrix;
	plyml polynomial;
	vec vector;
}data_t;







__always_inline inline void free_data_t(data_t* d)
{
	if(d->meta._delete)
		d->meta._delete(d);
	free(d->meta.data_mem_ptr);
	free(d);
}



#endif /* DATA_T_H_ */
