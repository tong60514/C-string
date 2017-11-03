#include <matrix.h>
#include <string.h>
#include <assert.h>
#include <bitmap.h>
#include <math.h>
#include <stdio.h>
#include <vector.h>
#include <meta.h>

#define __do_exchange_array(arr_1,arr_2,length) \
do{ \
	float exch;\
	for(int __iter=0;__iter<length;__iter++){\
		exch = arr_1[__iter];\
		arr_1[__iter] = arr_2[__iter];\
		arr_2[__iter] = exch;\
	}\
}while(0)

static inline void exchange_col(float* a,float* b,int length)
{
	float exch;
	for(int n=0;n<length;n++)
	{
		exch = a[n];
		a[n] = b[n];
		b[n] = exch;
	}
	return;
}

#define set_mat_flag(mat,nr) __set_bit(nr,mat->meta.ops)
#define test_mat_flag(mat,nr) test_bit(nr,mat->meta.ops)
static inline void clear_mat_flag(mat* m,long nr)
{
	__clear_bit(nr,m->meta.ops);
}

static inline void clear_mat_all(mat* m)
{
	__clear_all(m->meta.ops);
}



static inline void _free_relate_mat(mat* m)
{
	if(test_mat_flag(m,LUP_DECOMP))
	{
		free(m->related.LU_tri);
		free(m->related.per_mat);
	}
}

void free_matrix(mat* m)
{
	_free_relate_mat(m);
	free(m->meta.data_mem_ptr);
	free(m);
}
static mat* _malloc_mat(int row,int col);
mat* mat_cpy(mat* m)
{
	mat* cpy = _malloc_mat(m->row,m->col);
	if(cpy!=NULL)
		memcpy(cpy->meta.data_mem_ptr,m->meta.data_mem_ptr,m->meta.data_bytes);
	return cpy;
}




/*allocate memory */
static mat* _malloc_mat(int row,int col)
{
	unsigned int col_size = row*sizeof(float);
	unsigned int matrix_size = row*col_size;
	unsigned int index_array_size = col*sizeof(void*);
	mat* matrix = malloc(sizeof(mat)+index_array_size);
	if(matrix!=NULL)
	{
		matrix->index = (float**)_get_offset(matrix,sizeof(mat));
		matrix->meta.data_bytes = matrix_size;
		matrix->meta.struct_bytes = index_array_size+sizeof(mat);
		matrix->meta.ops[0] = 0;
		matrix->meta.data_mem_ptr = malloc(matrix_size);

		matrix->related.LU_tri = NULL;
		matrix->related.per_mat = NULL;
		matrix->row = row;
		matrix->col = col;

		/*map pointer*/
		matrix->index[0] = matrix->meta.data_mem_ptr;
		char* index_ptr = (char*)matrix->meta.data_mem_ptr;
		for(int i = 1;i<col;i++)
			matrix->index[i] = (float*)(index_ptr+=col_size);



		if(matrix->row == matrix->col)
			set_mat_flag(matrix,SQUQRE_MAT);
	}

	return matrix;
}



mat* matrix(int row,int col,float* src_data_ptr)
{

	/*variable declartion*/
	mat* matrix = _malloc_mat(row,col);
	if(matrix!=NULL)
		memcpy(matrix->meta.data_mem_ptr,src_data_ptr,matrix->meta.data_bytes);

	return matrix;
}


mat* mat_mul_mat(mat* mL,mat* mR)
{

	assert(mL->row==mR->col);

	mat* res =  _malloc_mat(mR->row,mL->col);

	for(int res_row_index = 0;res_row_index<mR->row;res_row_index++)
		for(int j=0;j<mL->col;j++)
			for(int i=0;i<mR->col;i++)
				res->index[j][res_row_index] +=
						mL->index[j][i]*mR->index[i][res_row_index];
	return res;

}



mat* mul2x2(mat* a,mat* b)
{
	assert( a->row==2&&a->col==2&&
			b->row==2&&b->col==2);
	mat* res =  _malloc_mat(2,2);
	float m1,m2,m3,m4,m5,m6,m7;

	m1= (a->index[0][0] + a->index[1][1]) * (b->index[0][0] + b->index[1][1]);
	m2= (a->index[1][0] + a->index[1][1]) * b->index[0][0];
	m3=  a->index[0][0] * (b->index[0][1] - b->index[1][1]);
	m4=  a->index[1][1] * (b->index[1][0] - b->index[0][0]);
	m5= (a->index[0][0] + a->index[0][1]) * b->index[1][1];
	m6= (a->index[1][0] - a->index[0][0]) * (b->index[0][0]+b->index[0][1]);
	m7= (a->index[0][1] - a->index[1][1]) * (b->index[1][0]+b->index[1][1]);

	res->index[0][0] = m1 + m4 - m5 + m7;
	res->index[0][1] = m3 + m5;
	res->index[1][0] = m2 + m4;
	res->index[1][1] = m1 - m2 + m3 + m6;
	return res;
}
mat* mul3x3(mat* a,mat* b);
mat* mul4x4(mat* a,mat* b);

void mat_mul_const(mat* a,float c)
{
	for(int i=0;i<a->col;i++)
		for(int j=0;j<a->row;j++)
			a->index[i][j]*=c;
	return;
}
mat* mat_add_mat(mat* a,mat* b)
{
	assert(a->row==b->row&&a->col==b->col);
	mat* sum = _malloc_mat(a->row,a->col);

	for(int i=0;i<a->col;i++)
		for(int j=0;j<a->row;j++)
			sum->index[i][j] = a->index[i][j]+b->index[i][j];
	return sum;
}

mat* mat_trans(mat* m)
{
	mat* trans = _malloc_mat(m->col,m->row);
	for(int i=0;i<m->col;i++)
		for(int j=0;j<m->row;j++)
			trans->index[j][i] = m->index[i][j];
	return trans;
}

#define ROW n
#define COL n
#define MAT m->related.LU_tri
void LU_decomposite(mat* m)
{
	assert(test_mat_flag(m,SQUQRE_MAT));
	int n = m->col;
	set_mat_flag(m,LUP_DECOMP);
	m->related.LU_tri = mat_cpy(m);
	m->related.per_mat = malloc(sizeof(int)*COL);
	/*init permutation matrix
	 * represent by an array */
	for(int i=0; i < COL ; i++)
		m->related.per_mat[i] = i;
	/*decompsite L and U tri matrix  into LU_tri*/

	int kp;
	int exch;
	for(int k=0;k<ROW;k++)
	{
		float p = 0.0f;
		for(int i = k;i<n;i++)
		{
			if(fabs(MAT->index[i][k])>p)
			{
				p = fabs(MAT->index[i][k]);
				kp = i;
			}
		}

		if(p==0.0f)
		{
			set_mat_flag(m,SINGULAR);
			return;
		}

		exch = m->related.per_mat[k];
		m->related.per_mat[k] = m->related.per_mat[kp];
		m->related.per_mat[kp] = exch;

		__do_exchange_array(MAT->index[kp],MAT->index[k],n);
		//exchange_col(m->index[kp],m->index[k],n);

		for(int i=k+1;i<n;i++)
		{
			MAT->index[i][k]/=MAT->index[k][k];
			for(int j=k+1;j<n;j++)
				MAT->index[i][j]-=MAT->index[i][k]*MAT->index[k][j];
		}
	}


	/*end  of decomposition */


}
#undef MAT
#undef COL
#undef ROW


/*PA = LU
 *
 * want ot find Ax = yCOL
 * x = ?
 *
 * PAx = Py ,
 *
 * */

float* mul_P_mat(int* perm, vec* y)
{
	float* res = malloc(y->meta.data_bytes);

	for(int i=0;i<y->length;i++)
		res[i] = y->scal[perm[i]];

	return res;
}


#define ROW n
#define COL n
#define LU_mat A->related.LU_tri
vec* linear_equation(mat* A,vec* y)
{
	//const static unsigned long buf_size = ;


	int n = A->row;
	assert(A->row == y->length);
	assert(test_mat_flag(A,SQUQRE_MAT));
	if(!test_mat_flag(A,LUP_DECOMP))
		LU_decomposite(A);
	/* have to be free */
	float* res_vec = mul_P_mat(A->related.per_mat,y);

	for(int i=0;i<ROW;i++)
		for(int j=i+1;j<COL;j++)
			res_vec[j] -= LU_mat->index[j][i] * res_vec[i];

	for(int i=ROW-1;i>=0;i--)
	{
		res_vec[i]/=LU_mat->index[i][i];
		for(int j=i-1;j>=0;j--)
			res_vec[j] -= (LU_mat->index[j][i] * res_vec[i]);
	}


	vec* res = malloc_vector(res_vec,n);

	free(res_vec);

	return res;
}
#undef ROW
#undef COL
#undef LU_mat
mat* inverse_mat(mat* A)
{
	assert(test_mat_flag(A,SQUQRE_MAT));
	assert(!test_mat_flag(A,SINGULAR));

	mat* res = _malloc_mat(A->row ,A->col);


}



