#include "polynomial.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

plyml* get_plyml(int length,...)
{
	va_list coef_list;
	va_start( coef_list, length);
	plyml* res = malloc(sizeof(plyml)+sizeof(int)*(length));

	PLYML_LENGTH(res) = length;

	res->coef = (int*)(__META_OFFSET(res,sizeof(plyml)));

	for(int i=0;i<length;i++)
		res->coef[i] = va_arg(coef_list,int);

	va_end(coef_list);

	return res;
}
