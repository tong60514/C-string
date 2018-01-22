/*
 * meta.h
 *
 *  Created on: Nov 4, 2017
 *      Author: tong60514
 */

#ifndef META_H_
#define META_H_
#include <bitmap.h>
#include <stdlib.h>
typedef void (*cstrct)(void* d);
typedef void (*dstrct)(void* d);





struct metadata
{
	void* data_mem_ptr;
	DECLARE_BITMAP(ops,64);
	ssize_t data_bytes;
	cstrct _new;
	dstrct _delete;
	ssize_t container_struct_size;
};


#define _get_offset(ptr,size_bytes) (((char*)ptr)+size_bytes)

/* construct data flag */

#define DEFAULT 0UL
#define PASS_BY_POINTER 1UL

static inline void init_meta(struct metadata* meta,ssize_t data_size,ssize_t container_size)
{
	meta->container_struct_size = container_size;
	meta->ops[0] = 0;
	meta->data_mem_ptr = malloc(data_size);
	meta->data_bytes = data_size;
	return;
}
inline void set_meta(struct metadata* meta,ssize_t data_size,ssize_t container_size)
{
	meta->container_struct_size = container_size;
	meta->ops[0] = 0;
	meta->data_bytes = data_size;
	return;
}
void* copy(struct metadata* m);

#endif /* META_H_ */
