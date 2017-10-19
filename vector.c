#include <vector.h>
#include <string.h>
#include <matrix.h>
#include <assert.h>
#define _vec_init_meta(meta,len,_bytes)\
	do{\
		__META_LENGTH(meta) = len;\
		__META_SIZE_T(meta) = _bytes;\
	}while(0);


static vec* _malloc_vector(int length)
{

	ssize_t size = length*sizeof(float)+sizeof(vec);
	vec* v = malloc(size);
	if(v!=NULL)
	{
		_vec_init_meta(v->meta,length,size);
		v->meta.offset[data_offset] = sizeof(vec);
		v->meta.data_bytes = length*sizeof(float);
		v->scal = (float*)_get_offset(v,sizeof(vec));
	}
	return v;
}
vec* malloc_vector(float* array,int length)
{
	vec* res = _malloc_vector(length);
	if(res!=NULL)
		memcpy(res->scal,array,length*sizeof(float));
	return res;
}

vec* mat_mul_vec(mat* m,vec* v)
{
	assert(m->row==v->meta.length);
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
}

