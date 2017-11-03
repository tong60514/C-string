#ifndef POLYNOMIAL_H_
#define POLYNOMIAL_H_
#include <meta.h>

typedef struct polynomial
{
	struct metadata meta;
	unsigned long length;
	float* coef;

}plyml;

plyml* get_plyml( int length,...);
float eval_plyml(plyml* p,float x);




#endif
