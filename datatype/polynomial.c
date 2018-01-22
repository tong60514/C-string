
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <output.h>
#include <vector.h>
#include <string.h>
#define data_offset 0



static plyml* malloc_polynomial(int length)
{
	plyml* res=malloc(sizeof(plyml));
	int data_size = sizeof(double)*length;
	res->length = length;

	/* mem map to coef */
	init_meta(&res->meta,data_size,sizeof(plyml));
	res->coef = (double*)(res->meta.data_mem_ptr);
	return res;
}
plyml* polynomial_byvector( vec* v){
	plyml* res = malloc_polynomial(v->length);
	res->coef =v->val;
	return res;
}
void init_plyml(plyml* p,vec* v){
	if(p->meta.data_mem_ptr)
		free(p->meta.data_mem_ptr);
	int data_size = sizeof(double)*v->length;
	p->length = v->length;
	init_meta(&p->meta,data_size,sizeof(plyml));
	p->coef = v->val;
}

plyml* polynomial( int length,...)
{
	plyml* res;
	int data_size = sizeof(double)*length;
	res = malloc(sizeof(plyml));
	res->length = length;
	/* mem map to coef */
	init_meta(&res->meta,data_size,sizeof(plyml));
	res->coef = (double*)(res->meta.data_mem_ptr);

	va_list coef_list;
	va_start( coef_list, length);
	for(int i=0;i<length;i++)
		res->coef[i] = va_arg(coef_list,double);
	va_end(coef_list);

	return res;
}

void free_plyml(plyml* p)
{
	free(p->meta.data_mem_ptr);
	free(p);
}

double eval_plyml(plyml* p,double x)
{
	double res = 0.0;
	unsigned int i= p->length ;
	while(i>0)
	{
		i--;
		res = res*x+p->coef[i];
	}
	return res;
}

double eval_by_vec(vec* p,double x)
{
	double* v = p->val;
	double res = 0.0;
	int i= p->length ;
	while(i>0)
	{
		i--;
		res = res*x+v[i];
	}
	return res;
}

double bisection(double (*func)(double x),double a,double b)
{

	double fc = 100.0;
	double c;

	while(fabs(fc)>0.00001f)
	{
		c = (a+b)/2;
		fc = func(c);
		if(fc==0.0)
			return c;
		if(fc>0.0)
			b = c;
		else
			a = c;
	}
	return c;
}

double caculate_interpolation(interpolator* p,double value)
{
	double* x = p->x->val;
	double* c = p->c->val;
	double res = 0.0;
	for(int i=p->length-2;i>=0;i--){
	   res+=c[i+1];
	   res*=(value-x[i]);
	}
	res += c[0];
	return res;

}


interpolator* interpolation(vec* x, vec*y ){
	assert(x->length==y->length);
	interpolator* interpolation = malloc(sizeof(interpolator));
	int length = x->length;
	interpolation->x = x;
	interpolation->y = y;
	interpolation->c = _malloc_vector(length);

	double* X = interpolation->x->val;
	double* Y = interpolation->y->val;
	double* C = interpolation->c->val;


	interpolation->length = length;

	for(int i=0;i<length;i++){
		C[i] = Y[i];
	    int count = 0;
	    double a = Y[i],b;
	    for(int j=i+1;j<length;j++){
	    	b = Y[j];
	        Y[j] = (b - a) / (X[j] - X[count]);
	        a = b;
	        count++;
	    }
	}
	return interpolation;
}

/*
 * public void DivDif(){
        double n[] = new double [this.pairs];
        for(int i=0;i<this.pairs;i++){
            this.c[i] = y[i];
            int count = 0;
            for(int j=i+1;j<this.pairs;j++){
                n[j] = (y[j] - y[j-1]) / (x[j] - x[count]);
                count++;
            }
            for(int j=i;j<this.pairs;j++)
                y[j] = n[j];
        }
    }
    */

