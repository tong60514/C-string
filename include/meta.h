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
	ssize_t struct_bytes;
	ssize_t data_bytes;
	cstrct _new;
	dstrct _delete;
};


#define _get_offset(ptr,size_bytes) (((char*)ptr)+size_bytes)

/* construct data flag */

#define DEFAULT 0UL
#define PASS_BY_POINTER 1UL


#endif /* META_H_ */
