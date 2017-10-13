#ifndef MATRIX_H_
#define MATRIX_H_

#include <x86intrin.h>
#include "meta.h"

typedef struct matrix
{
	struct metadata meta;
	int row,col;
	float** index;
	float* data;
}mat;

mat* get_matrix(float* data,int row,int col);

#define MATRIX_LENGTH(mat) __META_LENGTH(mat->meta)


#endif
