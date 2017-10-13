#include "matrix.h"
#include <string.h>
#define data_ptr 0
#define index_ptr 1
mat* get_matrix(float* src,int row,int col)
{
	int length = row*col;
	mat* matrix = malloc(sizeof(mat)
						+sizeof(float*)*col
						+sizeof(float)*length);

	matrix->meta.offset[data_ptr] = sizeof(mat)+sizeof(float*)*col;
	matrix->meta.offset[index_ptr] = sizeof(mat);
	matrix->col = col;
	matrix->row = row;
	MATRIX_LENGTH(matrix) = length;

	matrix->data = (float*)__META_OFFSET(matrix,matrix->meta.offset[data_ptr]);
	matrix->index = (float**)__META_OFFSET(matrix,matrix->meta.offset[index_ptr]);

	for(int i=0;i<col;i++)
		matrix->index[i] = &(matrix->data[i*row]);

	memcpy((void*)matrix->data,src,sizeof(float)*MATRIX_LENGTH(matrix));

	return matrix;
}
