#include "output.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
void print_plyml(plyml* p)
{
	for(int i=0;i<p->meta.length;i++)
		printf("coef:%d x power:%d\n",p->coef[i],i);
}
#define SYMBOL 2
#define END 2
#define _SPACE_ 2
void print_mat(mat* p)
{
	for(int i=0;i<p->col;i++)
	{
		printf(" | ");
		for(int j=0;j<p->row;j++)
			printf(" %f ",p->index[i][j]);
		printf(" | \n");
	}
}
