
#include <stdio.h>
int main(){




	double p[]={
			12.0,45.0,
			5.0,52.0
	};

	mat* A = matrix_v(2,2,p);
	print_mat(A);
	return 0;
}
