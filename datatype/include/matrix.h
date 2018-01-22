#ifndef MATRIX_H_
#define MATRIX_H_

#include <x86intrin.h>
#include <meta.h>
#include <vector.h>
typedef struct matrix mat;
typedef double (*func)(vec* x);
void vec_write(vec* from,vec* to);
vec* vec_sub_vec(vec* a,vec* b);
struct related_data
{
	mat *LU_tri;
	mat* inverse;
	int *per_mat;
};
typedef struct matrix
{
	struct metadata meta;
	struct related_data related;
	int column,row;
	double** index;
}mat;

struct jocabianMatrix{
	func** entry;
	int r,c;
};

typedef struct jocabianMatrix* jcbTable;
/* data handling */
mat* mat_cpy(mat* m);
mat* matrix_v(int column,int row,double* src_data_ptr);
void free_matrix(mat* m);

/*matrix_v arthemetic*/
mat* mat_mul_mat(mat* mL,mat* mR);
void mat_mul_const(mat* m,double c);
mat* mat_add_mat(mat* a,mat* b);
mat* mul2x2(mat* a,mat* b);
double* mul_P_mat(int* perm,vec* y);

/*matrix_v operation*/
mat* inverse_mat(mat* A);
mat* mat_trans(mat* m);

/*data fitting*/


mat* generate_linear_model(vec* x);
mat* generate_parabola_model(vec* x);
vec* least_square_approximation(mat* A,vec* b);

/*solving linear system*/
void LU_decomposite(mat* m);
vec* linear_equation(mat* A,vec* y);
vec* guess_seidle(mat* A,vec* b);
mat* orthalgonalization(vec** basis,int col,int row);

/* flag */

/*jocabian Matrix*/

jcbTable jcb_create(int r,int c,func* entry);
void fillMat(mat* jcbMat , jcbTable fTable,vec* args);
vec* guass_newton(jcbTable t,func* f);
vec* multi_newton(jcbTable t,func* f);
#define SQUARE_MAT 0
#define LUP_DECOMP 1
#define SINGULAR   (1<<1)

void fillTrans(mat*,mat*);




#endif
