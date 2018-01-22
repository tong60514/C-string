#include <struct.h>
#include <string.h>
#include <matrix.h>
#include <assert.h>




vec* _malloc_vector(const unsigned int length)
{

	vec* v = malloc(sizeof(vec));
	if(v!=NULL){
		v->length = length;
		init_meta(&(v->meta),length*sizeof(double),sizeof(vec));
		v->val = (double*)v->meta.data_mem_ptr;
	}
	return v;
}
vec* vector(double* array,int length)
{
	vec* res = _malloc_vector(length);
	if(res!=NULL)
		memcpy(res->val,array,res->meta.data_bytes);
	return res;
}
void init_zero_vector(vec* v,int length){
	v->length = length;
	init_meta(&(v->meta),length*sizeof(double),sizeof(vec));
	v->val = (double*)v->meta.data_mem_ptr;
}
vec* zero_vector(int length){
	vec* zeros = _malloc_vector(length);
	for(int i=0;i<length;i++)
		zeros->val[i] = 0.0;
	return zeros;
}
void free_vector(vec* v)
{
	free(v->meta.data_mem_ptr);
	free(v);
}
double dot(vec* a,vec* b){
	double res = 0.0;
	for(int i=0;i<a->length;i++){
		res += (a->val[i] * b->val[i]);
	}
	return res;
}
double norm(vec* a){
	double res = 0.0;
	for(int i=0;i<a->length;i++)
		res += (a->val[i]*a->val[i]);

	return sqrt(res);
}
void normalize(vec *v){
	double ith_norm = norm(v);
	for(int i=0;i<v->length;i++)
		v->val[i]/=ith_norm;
}

void vec_sub(vec* a,vec* b,vec* res){
	for(int i=0;i<a->length;i++)
		res->val[i] = a->val[i] - b->val[i];
}
vec* vec_sub_vec(vec* a,vec* b){
	vec* res = zero_vector(a->length);
	for(int i=0;i<a->length;i++)
		res->val[i] = a->val[i] - b->val[i];
	return res;
}

void sub_to(vec* src,vec* x){
	for(int i=0;i<src->length;i++)
		src->val[i] -=x->val[i];
}
void vec_mul_const(vec* v,double c,vec* res){
	for(int i=0;i<v->length;i++)
		res->val[i]= v->val[i]*c;
}
vec* vec_mul_c(vec* v,double c){
	vec* res = zero_vector(v->length);
	for(int i=0;i<v->length;i++)
		res->val[i]= v->val[i]*c;
	return res;
}
void vec_write(vec* from,vec* to){
	for(int i=0;i<to->length;i++)
		to->val[i] = from->val[i];
}
