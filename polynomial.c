#include "polynomial.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define data_offset 0
plyml* polynomial( int length,...)
{
	plyml* res;
	int data_size = sizeof(float)*length;



	res = malloc(sizeof(plyml));
	res->length = length;
	/* mem map to coef */

	res->meta.data_mem_ptr = malloc(data_size);
	res->coef = (float*)(res->meta.data_mem_ptr);



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

float eval_plyml(plyml* p,float x)
{
	float res = 0;
	unsigned int i= p->length ;

	while(i>0)
	{
		i--;
		res = res*x+p->coef[i];
	}
	return res;
}

float bisection(float (*func)(float x),float a,float b)
{

	float fc = 100.0;
	float c;

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



