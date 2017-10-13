#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strsearch.h"
#include "list.h"
#include "bitmap.h"
#include "polynomial.h"
#include "matrix.h"
#include "output.h"

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
	float arr[]={
			1.5,1.5,3.4,4.2,
			1.5,1.5,3.4,4.2,
			1.5,1.5,3.7,4.2,
	};
	mat* y = get_matrix(arr,4,3);
	printf("%f %f %f %f \n",y->index[0][0],y->index[0][1],y->index[0][2],y->index[0][3]);
	print_mat(y);
	return 0;
}
