#ifndef POLYNOMIAL_H_
#define POLYNOMIAL_H_
#include "meta.h"

typedef struct polynomial
{
	struct metadata meta;
	int* coef;
}plyml;

plyml* get_plyml(int length,...);


#define PLYML_LENGTH(p) __META_LENGTH(p->meta)


#endif
