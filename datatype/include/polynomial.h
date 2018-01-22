#ifndef POLYNOMIAL_H_
#define POLYNOMIAL_H_
#include <meta.h>

#include <vector.h>
typedef double (*interpolation_res)(double x);
typedef struct polynomial
{
	struct metadata meta;
	long length;
	double* coef;
}plyml;


typedef struct interpolation_info
{
	vec* x;
	vec* y;
	vec* c;
	long length;
}interpolator;


plyml* polynomial(int length,...);
plyml* polynomial_byvector( vec* arr);

void init_plyml(plyml* p,vec* v);
double eval_plyml(plyml* p,double x);
double caculate_interpolation(interpolator* p,double x);
interpolator* interpolation(vec* x,vec* y);
double bisection(double (*func)(double x),double a,double b);
double eval_by_vec(vec* p,double x);


#endif
