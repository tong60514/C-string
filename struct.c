
#include <string.h>
#include <struct.h>
#include <stdlib.h>
/*
 * allocate memory
 *
 *
 *
 *
 *
 *
 *
 * */








inline void init_meta(struct metadata* meta,void* data,ssize_t data_size,ssize_t extra)
{
	meta->struct_bytes += extra;
	meta->ops[0] = 0;
	meta->data_mem_ptr = data;
	meta->data_bytes = data_size;
	return;
}


