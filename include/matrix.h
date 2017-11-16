#ifndef MATRIX_H_
#define MATRIX_H_

#include <x86intrin.h>
#include <meta.h>
#include <vector.h>
typedef struct matrix mat;
struct related_matrix
{
	mat *LU_tri;
	int *per_mat;
	mat* inverse;
};

typedef struct matrix
{
	struct metadata meta;
	struct related_matrix related;
	int row,col;
	float** index;
}mat;

/*copy data to the matrix */
mat* malloc_matrix(float* data,int row,int col);

/* index ptr will map to data*/
mat* matrix(int row,int col,float* src_data_ptr);

void free_matrix(mat* m);


mat* mat_mul_mat(mat* mL,mat* mR);
void mat_mul_const(mat* m,float c);
mat* mat_add_mat(mat* a,mat* b);



mat* mul2x2(mat* a,mat* b);

mat* mat_trans(mat* m);

mat* mat_cpy(mat* m);
void LU_decomposite(mat* m);

mat* inverse_mat(mat* A);



vec* linear_equation(mat* A,vec* y);

float* mul_P_mat(int* perm, vec* y);
vec* iter_jocabi(mat* A,vec* b);
/* flag */
#define SQUQRE_MAT 0
#define LUP_DECOMP 1
#define SINGULAR   2





#endif
