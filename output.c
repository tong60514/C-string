#include "output.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
void print_plyml(plyml* p)
{
	printf("| ");
	printf("%f ",p->coef[0]);
	for(int i=1;i<p->length;i++)
		printf(" + %f*x^%d ",p->coef[i],i);
	printf(" |\n");
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
	for(int i=0;i<p->length;i++)
		printf(" %f, ",p->scal[i]);
	printf(" }\n");
}
