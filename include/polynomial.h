#ifndef POLYNOMIAL_H_
#define POLYNOMIAL_H_
#include <meta.h>

typedef struct polynomial
{
	struct metadata meta;
	unsigned long length;
	float* coef;

}plyml;

plyml* polynomial(int length,...);
float eval_plyml(plyml* p,float x);
float bisection(float (*func)(float x),float a,float b);



#endif
