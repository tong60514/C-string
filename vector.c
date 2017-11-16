#include <struct.h>
#include <string.h>
#include <matrix.h>
#include <assert.h>




static vec* _malloc_vector(const unsigned int length)
{

	vec* v = malloc(sizeof(vec));
	if(v!=NULL)
	{
		v->length = length;
		v->meta.struct_bytes = sizeof(vec);
		v->meta.data_bytes = length*sizeof(float);
		v->scal = malloc(v->meta.data_bytes);
	}
	return v;
}
vec* vector(float* array,int length)
{
	vec* res = _malloc_vector(length);
	if(res!=NULL)
		memcpy(res->scal,array,res->meta.data_bytes);
	return res;
}

/*vec* mat_mul_vec(mat* m,vec* v)
{
	assert(m->row==v->length);
	vec* res = _malloc_vector(m->col);
	register float p asm("r15");
	if(res!=NULL)
	{
		for(int i=0;i<m->col;i++)
		{
			p=0;
			for(int j=0;j<m->row;j++)
			{
				p += m->index[i][j]*v->scal[j];
				res->scal[i] = p;
			}
		}
	}
	return res;
}*/


void free_vector(vec* v)
{
	free(v->meta.data_mem_ptr);
	free(v);
}

