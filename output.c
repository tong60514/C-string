#include "output.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
void print_plyml(plyml* p)
{
	for(int i=0;i<p->meta.length;i++)
		printf("coef:%d x power:%d\n",p->coef[i],i);
}
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

void print_vec(vec* p)
{
	printf("{ ");
	for(int i=0;i<p->meta.length;i++)
		printf(" %f, ",p->scal[i]);
	printf(" }\n");
}
