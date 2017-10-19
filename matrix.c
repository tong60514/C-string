#include <matrix.h>
#include <string.h>
#include <assert.h>
#include <bitmap.h>
#include <math.h>
#include <stdio.h>
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
static inline void set_mat_flag(mat* m,long nr)
{
	__set_bit(nr,m->meta.ops);
}
static inline void clear_mat_flag(mat* m,long nr)
{
	__clear_bit(nr,m->meta.ops);
}

static inline void clear_mat_all(mat* m)
{
	__clear_all(m->meta.ops);
}

static inline int test_mat_flag(mat* m,long nr)
{
	return variable_test_bit(nr,m->meta.ops);
}

static inline void _free_relate_mat(mat* m)
{
	if(test_mat_flag(m,LSYS_SOLVED))
	{
		free(m->related.L_tri);
		free(m->related.U_tri);
		free(m->related.per_mat);
	}
}

mat* mat_cpy(mat* m)
{
	mat* cpy = malloc(m->meta.bytes);
	memcpy(cpy,m,m->meta.bytes);
	return cpy;
}

void free_matrix(mat* m)
{
	_free_relate_mat(m);
	free(m);
}


static mat* _malloc_mat(int row,int col)
{
	int length = row*col;
	ssize_t _bytes = sizeof(mat)+sizeof(float*)*col+sizeof(float)*length;
	mat* matrix = malloc(_bytes);
	if(matrix!=NULL)
	{

		matrix->meta.offset[data_offset] = sizeof(mat)+sizeof(float*)*col;
		matrix->meta.offset[index_offset] = sizeof(mat);
		matrix->meta.data_bytes = sizeof(float)*length;

		__META_SIZE_T(matrix->meta)= _bytes;

		matrix->col = col;
		matrix->row = row;
		MATRIX_LENGTH(matrix) = length;

		matrix->data = (float*)_get_offset(matrix,matrix->meta.offset[data_offset]);
		matrix->index = (float**)_get_offset(matrix,matrix->meta.offset[index_offset]);

		/* memory map to index */
		for(int i=0;i<col;i++)
		matrix->index[i] = &(matrix->data[i*row]);
		/*check matrix property*/
		if(row==col)
			set_mat_flag(matrix,SQUQRE_MAT);
	}
	return matrix;
}
mat* malloc_matrix(float* src,int row,int col)
{
	mat* matrix = _malloc_mat(row,col);
	if(matrix!=NULL)
		memcpy((void*)matrix->data,src,matrix->meta.data_bytes);
	return matrix;
}


mat* mat_mul_mat(mat* mL,mat* mR)
{

	assert(mL->row==mR->col);

	mat* res =  _malloc_mat(mR->row,mL->col);

	for(int res_row_index = 0;res_row_index<mR->row;res_row_index++)
	{
		for(int j=0;j<mL->col;j++)
			for(int i=0;i<mR->col;i++)
			{
				res->index[j][res_row_index] +=
						mL->index[j][i]*mR->index[i][res_row_index];
			}
	}
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
void LU_decomposite(mat* m)
{
#define ROW n
#define COL n
	assert(test_mat_flag(m,SQUQRE_MAT));
	int n = m->col;
	set_mat_flag(m,LSYS_SOLVED);

	m->related.U_tri = _malloc_mat(ROW,COL);
	m->related.L_tri = mat_cpy(m);
	m->related.per_mat = malloc(sizeof(int)*COL);
	/*init permutation matrix
	 * represent by an array */
	for(int i=0; i < COL ; i++)
		m->related.per_mat[i] = i;
	/*decompsite L and U tri matrix*/
#define MAT m->related.L_tri

	for(int k=0;k<ROW;k++)
	{
		float p = 0.0f;
		int kp;
		for(int i = k;i<n;i++)
		{
			if(fabs(MAT->index[i][k])>p)
			{
				p = fabs(MAT->index[i][k]);
				kp = i;
			}
		}

		if(p==0.0f)
			return;

		int exch = m->related.per_mat[k];
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
#undef MAT
	/*end of first part of decompose */
#define UP_TRI m->related.U_tri
#define LOW_TRI m->related.L_tri
	/*saperate the matrix*/
	for(int r = 0; r<n  ;r++)
	{
		for(int c=r;c<n ;c++)
		{
			UP_TRI->index[r][c] = LOW_TRI->index[r][c];
			LOW_TRI->index[r][c] = 0;
		}
		LOW_TRI->index[r][r] = 1.0f;
	}




#undef COL
#undef ROW
}



