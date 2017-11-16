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
#include<signal.h>
#include<unistd.h>
#include <psignal.h>
#include <math.h>
#include <buffer.h>
struct str_loc
{
	struct llist list_head;
	int index;
};
 int i = 1;
void* hello(void* id);
static void test_threadpool();



float afunc(float x)
{
	return cos(x)*cos(x)+ 6.0-x;
}
float bfunc(float x)
{
	return 1.0 +5.0*x  - 6.0*x*x*x - exp(2.0*x);
}
float cfunc(float x)
{
	return exp(x)+sin(x)-4.0;
}


void test_kmp()
{
	struct pattern_info info;
	char test[] = "abc bcb cbc abc bca bcc bca bca cba cb abc";
	char pat[]="abc";
	char yo[] = "ppp";

	init_pattern(&info,pat,10);
	kmpMatch(test,&info);
	subs_str_to(test,pat,yo,20);
}

void bit_test()
{
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
}

void polynomial_test()
{
	plyml* p = polynomial(5,1.0,2.0,3.0,5.0,6.0);
	print_plyml(p);
	printf("%f \n" , eval_plyml(p,2.0));
	printf("---\n");
}

void matrix_test()
{
	float arr[][4]={
		{2.0,0.0,2.0,0.6},
		{3.0,3.0,4.0,-2.0},
		{5.0,5.0,4.0,2.0},
		{-1.0,-2.0,3.4,-1.0}
	};

	mat* x = matrix(4,4,arr);
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
	mat* xx = matrix(4,4,arr);

	LU_decomposite(xx);
	printf("-------------LUP decompose test-----------------------\n");
	printf("-------------upper triangle matrix--------------\n");
	print_mat(xx->related.LU_tri);
	printf("-------------lower triangle matrix--------------\n");
	print_mat(xx->related.LU_tri);

	float sys[] ={
		4.0,2.0,-1.0,3.0,
		3.0,-4.0,2.0,5.0,
		-2.0,6.0,-5.0,-2.0,
		5.0,1.0,6.0,-3.0
	};

	float v[] ={16.9,-14.0,25.0,9.4};
	mat* m = matrix(4,4,sys);

	mat*  mp = matrix(4,4,sys);

	print_mat(m);

	printf("---------\n");
	print_mat(mp);
	printf("matrix mp size : %d \n",mp->meta.struct_bytes);
	free(mp);
	printf("--------- 0 \n");
	mp = matrix(4,4,sys);
	print_mat(mp);
	printf("matrix mp size : %d \n",mp->meta.struct_bytes);

	vec* q = vector(v,4);
	print_vec(q);


	vec* ans = linear_equation(m,q);
	printf(" test linear equation \n");
	print_vec(ans);

	for(int i=0;i<4;i++)
		printf("%d\n",m->related.per_mat[i]);

	float input[] = {
		3.0, -1.0 ,0.0 ,0.0,0.0,0.5,
		-1.0, 3.0,-1.0 ,0.0,0.5,0.0,
		0.0, -1.0, 3.0,-1.0,0.0,0.0,
		0.0, 0.0, -1.0, 3.0, -1.0, 0.0,
		0.0, 0.5, 0.0, -1.0, 3.0,-1.0,
		0.5, 0.0, 0.0, 0.0, -1.0, 3.0
	};
	float d[] = {
		1.0, 0.0 ,0.0 ,0.0,0.0,0.0,
		0.0, 1.0, 0.0 ,0.0,0.0,0.0,
		0.0, 0.0, 1.0,0.0,0.0,0.0,
		0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0, 1.0,0.0,
		0.0, 0.0, 0.0, 0.0, 0.0, 1.0
	};

	mat* A = matrix(6,6,input);
	//mat* dm = matrix(6,6,d);
	float un[] = {
		2.5, 1.5, 1.0, 1.0, 1.5, 2.5
	};





	vec* b = vector(un,6);

	//vec* res_vec = mat_mul_vec(dm,b);

	vec* ans__= iter_jocabi(A,b);
	printf("------------\n");
	print_vec(ans__);

	float input2[] = {
		3.0, -1.0 ,0.0 ,0.0,0.0,0.5,
		-1.0, 3.0,-1.0 ,0.0,0.5,0.0,
		0.0, -1.0, 3.0,-1.0,0.0,0.0,
		0.0, 0.0, -1.0, 3.0, -1.0, 0.0,
		0.0, 0.5, 0.0, -1.0, 3.0,-1.0,
		0.5, 0.0, 0.0, 0.0, -1.0, 3.0
	};
	mat* mcc = matrix(6,6,input2);

	free_data_t((data_t*)mcc);
	free_data_t((data_t*)b);
	free_data_t((data_t*)q);
	free_data_t((data_t*)mp);
	free_data_t((data_t*)ans);

	printf("-------------------end matrix test-------------------\n");
}





int main()
{
#ifdef STRINGTEST
	test_kmp();
#endif

#ifdef BITTSET
	bit_test();
#endif
#ifdef POLYTEST
	polynomial_test()
#endif
#define MATRIX_TEST
#ifdef MATRIX_TEST
	matrix_test();
#endif
	struct circular_buf b;
	INIT_CIRCLULAR_BUF(&b);
	char bn[]={
			"efefefeeeeeeeeeeeeefefewGRQEAGEG\0"
	};


	for(int i=0;i<50;i++)
	{
		poke_buf(&b,bn[i]);
		printf("%d left ---- \n",check_left(&b));
	}

	char* t = malloc(sizeof(char)*strlen(bn));


	read_buf_all(&b,t);

	printf("%s",t);
	memcpy(b.buf,bn,4096);
	printf("------buffer left after read all %d----------\n",check_left(&b));


	printf("----------------\n");

	printf("\n");
	printf("-------------------end buffer test-------------------\n");



}





void* hello(void* id);
static void test_threadpool()
{
	tpool tp;
	tpool_init(&tp,4);
	tpool_start(&tp);
	int a = 0;
	tpool_add_task(&tp,hello,&a,NULL);
	while(i)
		printf("%d\n",i);
	tpool_cancel(&tp);

}

void* hello(void* id)
{
	i = *(int*)id;
	fprintf(stderr,"%d arrive\n" , i);
	return NULL;
}

