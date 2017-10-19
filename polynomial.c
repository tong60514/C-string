#include "polynomial.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#define data_offset 0
plyml* get_plyml(int length,...)
{
	va_list coef_list;
	va_start( coef_list, length);
	int data_size = sizeof(int)*(length);
	plyml* res = malloc(sizeof(plyml)+data_size);

	PLYML_LENGTH(res) = length;
	res->meta.offset[data_offset] = data_size;
	/* mem map to coef */
	res->coef = (int*)(_get_offset(res,sizeof(plyml)));

	for(int i=0;i<length;i++)
		res->coef[i] = va_arg(coef_list,int);

	va_end(coef_list);

	return res;
}
