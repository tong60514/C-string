#ifndef MATRIX_H_
#define MATRIX_H_

#include <x86intrin.h>
#include "meta.h"
typedef struct matrix mat;
struct related_matrix
{
	mat *L_tri;
	mat *U_tri;
	int *per_mat;
};

typedef struct matrix
{
	struct metadata meta;
	struct related_matrix related;
	int row,col;
	float** index;
	float* data;
}mat;


mat* malloc_matrix(float* data,int row,int col);

void free_matrix(mat* m);


mat* mat_mul_mat(mat* mL,mat* mR);
void mat_mul_const(mat* m,float c);
mat* mat_add_mat(mat* a,mat* b);

#define MATRIX_LENGTH(mat) __META_LENGTH(mat->meta)

mat* mul2x2(mat* a,mat* b);

mat* mat_trans(mat* m);
mat* mat_cpy(mat* m);
void LU_decomposite(mat* m);


#define MATRIX_SET_ZERO(m) \
	memset((void*)m->data,0,MATRIX_LENGTH(m)*sizeof(float));


/* flag */
#define SQUQRE_MAT 0
#define LSYS_SOLVED 1

#endif
