
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <struct.h>
#include <output.h>
#include <sys/resource.h>
#include <mcheck.h>
#include <meta.h>
extern vec* _malloc_vector(int length);

static inline void _free_mat(mat *m);
#define __do_exchange_array(arr_1,arr_2,length) \
do{ \
	double exch;\
	for(int __iter=0;__iter<length;__iter++){\
		exch = arr_1[__iter];\
		arr_1[__iter] = arr_2[__iter];\
		arr_2[__iter] = exch;\
	}\
}while(0)
#define init_related_mat(matrix) \
	do\
	{\
		matrix->related.LU_tri = NULL;\
		matrix->related.per_mat = NULL;\
	}while(0);



static inline void exchange_col(double* a,double* b,int length)
{
	__do_exchange_array(a,b,length);
	return;
}
void tri_UP_subsitution(mat* A,double* arr_x)
{
	int n = A->row;
	assert(n>=0);
	for(int i=n-1;i!=-1;i--)
	{
		arr_x[i] /= A->index[i][i];
		for(int j=i-1;j!=-1;j--)
			arr_x[j] -= A->index[j][i]*arr_x[i];
	}
}

void tri_LOW_subsitution(mat* A,double* arr_x)
{
	int n = A->row;
	for(int i=0;i<n;i++)
	{
		arr_x[i] /= A->index[i][i];
		for(int j=i+1;j<n;j++)
			arr_x[j] -= A->index[j][i]*arr_x[i];
	}
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
		_free_mat(m->related.LU_tri);
		free(m->related.per_mat);
	}
}

void free_matrix(mat* m)
{
	_free_relate_mat(m);
	_free_mat(m);
}
mat* _malloc_mat(int col,int row);
static inline void _free_mat(mat *m){
	free(m->meta.data_mem_ptr);
	free(m);
}
mat* mat_cpy(mat* m)
{
	mat* cpy = _malloc_mat(m->column,m->row);
	if(cpy!=NULL)
		memcpy(cpy->meta.data_mem_ptr,m->meta.data_mem_ptr,m->meta.data_bytes);
	return cpy;
}



static inline void _map_pointer(mat* m,int col,int row){
	m->row = row;
	m->column = col;
	m->index[0] = m->meta.data_mem_ptr;

	double* data_loc = m->meta.data_mem_ptr;

	for(int i=1;i<row;i++)
		m->index[i] = (data_loc+=col);
}
static inline void _mat_config(mat* m){
	if(m->column == m->row)
		set_mat_flag(m,SQUARE_MAT);
}


/*matrix memory
 *
 * address high :
 * 				 ------  struct ref		|   0x000000
 * 				 ------  			   	|
 * 				 ------					|
 * 				 ------					|   } sizeof(mat)
 * 				 ------					|
 * 				 ------  -----------	|	0x000000+sizeof(mat)
 * 				 data index pointer 	|
 * 				 		*				|
 * 				 		*				|
 * 				 		*				|
 * 				 		*				|
 * 				 		row				|	} each pointer point
 * 				 		*				|		to an array entry
 * 				 		*				|
 * 				 		*				|
 * 				 -------------------
 *
 *
 *
 * address low  :
 * */
/*allocate memory */
mat* _malloc_mat(int col,int row)
{
	int matrix_size = row*col*sizeof(double);
	int index_array_size = row*sizeof(double*);

	mat* matrix = malloc(sizeof(mat)+index_array_size);

	if(matrix!=NULL)
	{
		matrix->index = (double**)_get_offset(matrix,sizeof(mat));
		init_meta(&matrix->meta,matrix_size,sizeof(mat));
		init_related_mat(matrix);
		_map_pointer(matrix,col,row);
		_mat_config(matrix);
	}

	return matrix;
}






mat* matrix_v(int row,int col,double* src_data_ptr)
{
	/*variable declartion*/
	mat* matrix = _malloc_mat(col,row);
	if(matrix!=NULL)
		memcpy(matrix->meta.data_mem_ptr,src_data_ptr,matrix->meta.data_bytes);
	return matrix;
}


mat* mat_mul_mat(mat* mL,mat* mR)
{
	assert(mL->column==mR->row);
	mat* res =  _malloc_mat(mR->column,mL->row);
	for(int res_row_index = 0;res_row_index<mR->column;res_row_index++)
		for(int j=0;j<mL->row;j++)
			for(int i=0;i<mR->row;i++)
				res->index[j][res_row_index] +=
						mL->index[j][i]*mR->index[i][res_row_index];
	return res;
}




mat* mul2x2(mat* a,mat* b)
{
	assert( a->column==2&&a->row==2&&
			b->column==2&&b->row==2);
	mat* res =  _malloc_mat(2,2);
	double m1,m2,m3,m4,m5,m6,m7;

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
mat* mul3x3(mat* a,mat* res);
mat* mul4x4(mat* a,mat* res);

void mat_mul_const(mat* a,double c)
{
	for(int i=0;i<a->row;i++)
		for(int j=0;j<a->column;j++)
			a->index[i][j]*=c;
	return;
}
mat* mat_add_mat(mat* a,mat* b)
{
	assert(a->column==b->column&&a->row==b->row);
	mat* sum = _malloc_mat(a->column,a->row);

	for(int i=0;i<a->row;i++)
		for(int j=0;j<a->column;j++)
			sum->index[i][j] = a->index[i][j]+b->index[i][j];
	return sum;
}

mat* mat_trans(mat* m)
{

	mat* trans = _malloc_mat(m->row,m->column);
	for(int i=0;i<m->row;i++)
		for(int j=0;j<m->column;j++)
			trans->index[j][i] = m->index[i][j];
	return trans;
}

#define ROW n
#define COL n
#define LUMAT m->related.LU_tri
void LU_decomposite(mat* m)
{
	assert(test_mat_flag(m,SQUARE_MAT));
	int n = m->row;
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
	for(int k=0;k<ROW;k++){
		double p = 0.0f;
		for(int i = k;i<n;i++){
			if(fabs(LUMAT->index[i][k])>p){
				p = fabs(LUMAT->index[i][k]);
				kp = i;
			}
		}
		if(p==0.0f){
			set_mat_flag(m,SINGULAR);
			return;
		}
		exch = m->related.per_mat[k];
		m->related.per_mat[k] = m->related.per_mat[kp];
		m->related.per_mat[kp] = exch;
		__do_exchange_array(LUMAT->index[kp],LUMAT->index[k],n);
		//exchange_col(m->index[kp],m->index[k],n);
		for(int i=k+1;i<n;i++){
			LUMAT->index[i][k]/=LUMAT->index[k][k];
			for(int j=k+1;j<n;j++)
				LUMAT->index[i][j]-=LUMAT->index[i][k]*LUMAT->index[k][j];
		}
	}


	/*end  of decomposition */


}
#undef LUMAT
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

double* mul_P_mat(int* perm, vec* y)
{
	double* res = malloc(y->meta.data_bytes);
	for(int i=0;i<y->length;i++)
		res[i] = y->val[perm[i]];

	return res;
}


#define ROW n
#define COL n
#define LU_mat A->related.LU_tri
vec* linear_equation(mat* A,vec* y)
{
	//const static unsigned long buf_size = ;
	int n = A->column;
	assert(A->row == y->length);
	assert(test_mat_flag(A,SQUARE_MAT));
	if(test_mat_flag(A,LUP_DECOMP)) _free_mat(A->related.LU_tri);
	LU_decomposite(A);
	/* have to be free */
	double* res_vec = mul_P_mat(A->related.per_mat,y);
	for(int i=0;i<ROW;i++)
		for(int j=i+1;j<COL;j++)
			res_vec[j] -= LU_mat->index[j][i] * res_vec[i];
	for(int i=ROW-1;i>=0;i--){
		res_vec[i]/=LU_mat->index[i][i];
		for(int j=i-1;j>=0;j--)
			res_vec[j] -= (LU_mat->index[j][i] * res_vec[i]);
	}


	vec* res = vector(res_vec,n);
	return res;
}
void linearSolve(mat* A,vec* y,vec* res)
{
	//const static unsigned long buf_size = ;
	int n = A->column;
	assert(A->row == y->length);
	assert(test_mat_flag(A,SQUARE_MAT));
	if(test_mat_flag(A,LUP_DECOMP))
		_free_mat(A->related.LU_tri);
	LU_decomposite(A);
	for(int i=0;i<y->length;i++)
		res->val[i] = y->val[A->related.per_mat[i]];
	double* res_vec = res->val;
	for(int i=0;i<ROW;i++)
		for(int j=i+1;j<COL;j++)
			res_vec[j] -= LU_mat->index[j][i] * res_vec[i];
	for(int i=ROW-1;i>=0;i--){
		res_vec[i]/=LU_mat->index[i][i];
		for(int j=i-1;j>=0;j--)
			res_vec[j] -= (LU_mat->index[j][i] * res_vec[i]);
	}
}
#undef ROW
#undef COL
#undef LU_mat
mat* inverse_mat(mat* A)
{
	assert(test_mat_flag(A,SQUARE_MAT));
	assert(!test_mat_flag(A,SINGULAR));
	return NULL;
}

static inline void _mat_mul_vec(mat* A,double* x,double* res)
{
	for(int i=0;i<A->row;i++)
	{
		res[i]=0.0;
		for(int j=0;j<A->column;j++)
			res[i]+= x[j]*A->index[i][j];
	}
}

vec* mat_mul_vec(mat* A,vec* x)
{
	double* v = malloc(sizeof(double)*A->row);
	_mat_mul_vec(A,x->val,v);
	return vector(v,A->row);
}

void MATxV(mat* A,vec* x,vec* res){
	double* v = res->val;
	_mat_mul_vec(A,x->val,v);
}
/*  res = b - a */
static inline void _vec_sub(int length, double* a, double* b,double* res)
{
	for(int i=0;i<length;i++)
		res[i] = a[i]-b[i];
}
/* res[i]  = b[i] + a[i]*/
static inline void _vec_add(int length, double* a, double* b,double* res)
{
	for(int i=0;i<length;i++)
		res[i] = a[i]+b[i];
}
#define MASK (1<<31)
static void do_guess_seidle(mat* A,double* b,double* x)
{
	int n = A->row;
	double* x1 = malloc(n*sizeof(double));
	int converge = 0;

	while(!converge)
	{
		converge = 1;
		for(int i=0;i<n;i++)
		{

			double rec = A->index[i][i];
			A->index[i][i] = 0.0;
			x1[i] = 0.0;
			for(int j=i;j<n;j++)
				x1[i] += x[j]*A->index[i][j];

			A->index[i][i] = rec;
			x1[i]  = b[i] - x1[i];
		}

		tri_LOW_subsitution(A,x1);
		for(int i=0;i<n;i++)
		{
			converge&=(fabs(x1[i]-x[i])<40.4e-10);
			x[i] = x1[i];
		}
	}
	free(x1);
}

vec* guess_seidle(mat* A,vec* b)
{
	double* guess = malloc(A->row*sizeof(double));
	for(int i=0;i<A->column;i++)
		guess[i] = 0.0;
	do_guess_seidle(A,b->val,guess);
	vec* res = vector(guess,A->row);
	return res;
}





vec* least_square_approximation(mat* A,vec* b){
	mat* transposeA = mat_trans(A);
	mat* cosistent = mat_mul_mat(transposeA,A);
	vec* bp = mat_mul_vec(transposeA,b);
	vec* xlambda = linear_equation(cosistent,bp);
	FREE_T(cosistent);
	FREE_T(transposeA);
	FREE_T(bp);
	return xlambda;
}
mat* generate_linear_model(vec* x){
	mat* A = _malloc_mat(2,x->length);

	for(int i=0;i<x->length;i++){
		A->index[i][0] = 1.0;
		A->index[i][1] = x->val[i];
	}
	return A;
}
mat* generate_parabola_model(vec* x){
	mat* A = _malloc_mat(3,x->length);
	for(int i=0;i<x->length;i++){
		A->index[i][0] = 1.0;
		A->index[i][1] = x->val[i];
		A->index[i][2] = x->val[i]*x->val[i];
	}
	return A;
}







mat* orthalgonalization(vec** basis,int col,int row){
	mat* R = _malloc_mat(col,row);
	vec* nn = _malloc_vector(row);
	for(int i=0;i<col;i++){
		for(int j=0;j<row;j++){
			if(j<i){
				double scal = dot(basis[i],basis[j]);
				R->index[j][i] = scal;
				vec_mul_const(basis[j],scal,nn);
				sub_to(basis[i],nn);
			}else R->index[j][i] = 0.0;
		}
		R->index[i][i] = norm(basis[i]);
		normalize(basis[i]);
	}
	for(int i=row;i<col;i++)
		for(int j=0;j<row;j++)
			R->index[j][i] = 0.0;
	free_data_t((data_t*)nn);
	return R;
}

static double m_dot(double* a,double* b,int length){
	double res = 0.0;
	for(int i=0;i<length;i++){
		res+= (a[i]*b[i]);
	}
	return res;
}

static void m_cxv(double c,double* v,double* res,int length){

	for(int i=0;i<length;i++){
		res[i] = v[i]*c;
	}
}
static void m_subto(double* res,double* v,int length){
	for(int i=0;i<length;i++){
		res[i] -= v[i];
	}
}


static double m_norm(double* v,int length){
	double res = 0.0;
	for(int i=0;i<length;i++)
		res+= (v[i]*v[i]);
	return sqrt(res);
}

mat* orthMatirx(mat* basis){
	int r,c;
	if(basis->column > basis->row){
		r = basis->row;
		c = basis->row;
	}else{
		r = basis->column;
		c = basis->row;
	}
	mat* R = _malloc_mat(c,c);
	double* nn = malloc(sizeof(double)*basis->column);

	for(int i=0;i<c;i++){
		for(int j=0;j<c;j++){
			if(j<i){
				double dt = m_dot(basis->index[i],basis->index[j],basis->column);
				R->index[i][j] = dt;
				m_cxv(dt,basis->index[j],nn,basis->column);
				m_subto(basis->index[i],nn,basis->column);
			}else R->index[i][j] = 0.0;
		}
		double nm = m_norm(basis->index[i],basis->column);
		R->index[i][i] = nm;
		for(int k=0;k<basis->column;k++)
			basis->index[i][k]/=nm;
	}
	return R;
}


void fillMat(mat* jcbMat,jcbTable fTable,vec* args){
	for(int i=0;i<jcbMat->row;i++)
		for(int j=0;j<jcbMat->column;j++)
			jcbMat->index[i][j] = fTable->entry[i][j](args);
}
jcbTable jcb_create(int r,int c,func* entry){
	jcbTable tab = malloc(sizeof(struct jocabianMatrix));
	tab->entry = malloc(sizeof(func*)*r);
	for(int i=0;i<r;i++)
		tab->entry[i] = &entry[i*c];
	tab->r = r;
	tab->c = c;
	return tab;
}

static inline void fillVec(vec* v,vec* arg,func* f){
	for(int i=0;i<v->length;i++)
		v->val[i] = f[i](arg);
}

void fillTrans(mat* A,mat* transA){
	for(int i=0;i<A->row;i++)
		for(int j=0;j<A->column;j++)
			transA->index[j][i] = A->index[i][j];
}

void subvecto(vec* a,vec* dst){
	for(int i=0;i<a->length;i++)
		dst->val[i] -= a->val[i];
}

void MatMul(mat* mL, mat* mR,mat* res){
	for(int res_row_index = 0;res_row_index<mR->column;res_row_index++)
			for(int j=0;j<mL->row;j++)
				for(int i=0;i<mR->row;i++)
					res->index[j][res_row_index] +=
							mL->index[j][i]*mR->index[i][res_row_index];
}
vec* multi_newton(jcbTable t,func* f){
	vec* x1= _malloc_vector(t->c);
	vec* x = _malloc_vector(t->c);
	vec* s = _malloc_vector(t->c);

	for(int i=0;i<t->c;i++)
		x->val[i] = 1.0;
	mat* A = _malloc_mat(t->c,t->r);
	int converge = 0;

	while(!converge)
	//for(int i=0;i<50;i++)
	{
		converge = 1;
		fillMat(A,t,x);
		fillVec(x1,x,f);
		linearSolve(A,x1,s);
		for(int k=0;k<s->length;k++)
			converge &= (fabs(s->val[k])<0.00001);
		subvecto(s,x);
	}
	FREE_T(x1);
	FREE_T(s);
	FREE_T(A);
	return x;
}
vec* guass_newton(jcbTable t,func* f){

	vec* x1= _malloc_vector(t->r);
	vec* x = _malloc_vector(t->c);
	vec* s = _malloc_vector(t->c);
	vec* b = _malloc_vector(t->c);
	mat* normal = _malloc_mat(t->c,t->c);
	for(int i=0;i<t->r;i++)
		x->val[i] = 2.0;
	mat* A = _malloc_mat(t->c,t->r);
	mat* transA = _malloc_mat(t->r,t->c);

	int converge = 0;

	while(!converge)
	{
		converge = 1;
		fillMat(A,t,x);
		fillVec(x1,x,f);
		fillTrans(A,transA);
		MatMul(transA,A,normal);
		MATxV(transA,x1,b);
		linearSolve(normal,b,s);
		for(int k=0;k<s->length;k++)
			converge &= (fabs(s->val[k])<0.00001);
		subvecto(s,x);
	}
	fillVec(x1,x,f);
	print_vec(x1);
	free(s->meta.data_mem_ptr);
	FREE_T(x1);
	FREE_T(b);
	FREE_T(transA);
	FREE_T(A);
	FREE_T(normal);
	print_vec(x);



	return x;
}
// -----------------------------------------



