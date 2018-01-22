#include "output.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
void print_plyml(plyml* p)
{
	printf("| ");
	printf("%lf ",p->coef[0]);
	for(int i=1;i<p->length;i++)
		printf(" + %lf*x^%d ",p->coef[i],i);
	printf(" |\n");
}
void print_mat(mat* p)
{
	printf("\n");
	for(int i=0;i<p->row;i++)
	{
		printf(" | ");
		for(int j=0;j<p->column;j++)
			printf(" %lf ",p->index[i][j]);
		printf(" | \n");
	}
}

void print_vec(vec* p)
{
	printf("\n{ ");
	for(int i=0;i<p->length;i++)
		printf(" %lf, ",p->val[i]);
	printf(" }\n");
}
