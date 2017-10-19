#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strsearch.h"
#include "list.h"
#include "bitmap.h"
#include "polynomial.h"
#include "matrix.h"
#include "output.h"
#include <vector.h>
#include <tpool.h>

struct str_loc
{
	struct llist list_head;
	int index;
};



int main()
{
#ifdef STRINGTEST
	struct pattern_info info;
	char test[] = "abc bcb cbc abc bca bcc bca bca cba cb abc";
	char pat[]="abc";
	char yo[] = "ppp";
	init_pattern(&info,pat,10);
	kmpMatch(test,&info);
	subs_str_to(test,pat,yo,20);
#endif

#ifdef BITTSET
	DECLARE_BITMAP(bit,64);
	bit[0] = 0;
	for(int i=0;i<64;i++)
	{
		__set_bit(i,bit);
		fprintf(stderr,"%lu \n",bit[0]);
	}
	bit[0] = 4;
	unsigned long re = variable_test_bit(3,bit);
	fprintf(stderr,"%lu",re);
#endif
#ifdef POLYTEST
	plyml* p = get_plyml(5,1,2,3,5,6);
	print_plyml(p);
	printf("---\n");
#endif

//#ifdef MATRIX_TEST
	float arr[][4]={
			{2.0,0.0,2.0,0.6},
			{3.0,3.0,4.0,-2.0},
			{5.0,5.0,4.0,2.0},
			{-1.0,-2.0,3.4,-1.0}
	};

	mat* x = malloc_matrix(arr,4,4);
	mat* xy2 = mat_mul_mat(x,x);
	printf("-------------mul test-------------------\n");
	print_mat(xy2);
	mat* trans = mat_trans(x);
	printf("-------------trans test-------------------\n");
	print_mat(trans);
	printf("-------------mul_const test-------------------\n");
	mat_mul_const(x,2.0);
	print_mat(x);
	printf("-------------mat_add_mat test-------------------\n");
	mat* sum = mat_add_mat(x,xy2);
	print_mat(sum);
	printf("-------------cpy_mat test-----------------------\n");
	mat* cpy = mat_cpy(sum);
	print_mat(cpy);
	mat* xx = malloc_matrix(arr,4,4);

	LU_decomposite(xx);
	printf("-------------LUP decompose test-----------------------\n");
	printf("-------------upper triangle matrix--------------\n");
	print_mat(xx->related.U_tri);
	printf("-------------lower triangle matrix--------------\n");
	print_mat(xx->related.L_tri);

	printf("-------------PA = LU--------------\n");

	float arru[][4]={
					{2.0,3.0,1.0,5.0},
					{0.0,4.0,2.0,4.0},
					{0.0,0.0,1.0,2.0},
					{0.0,0.0,0.0,3.0}
			};
	float arrl[][4]={
				{1.0,0.0,0.0,0.0},
				{3.0,1.0,0.0,0.0},
				{1.0,4.0,1.0,0.0},
				{2.0,1.0,7.0,1.0}
		};
	mat* u = malloc_matrix(arru,4,4);
	mat* l = malloc_matrix(arrl,4,4);
	print_mat(u);
	print_mat(l);
	mat* PA = mat_mul_mat(xx->related.L_tri ,xx->related.U_tri);
	print_mat(PA);
	printf("-------test vector --------\n");
	float a[]={
			1.0,5.4,0.8,0.9,
	};

	vec* t = malloc_vector(a,4);
	print_vec(t);
	float idd[][4]={
					{1.0,0.0,0.0,0.0},
					{0.0,1.0,0.0,0.0},
					{0.0,0.0,1.0,0.0},
					{0.0,0.0,0.0,1.0}
			};
	mat* id = malloc_matrix(idd,4,4);
	vec* res1 = mat_mul_vec(id,t);
	print_vec(res1);


	return 0;
}
