/*
 * meta.h
 *
 *  Created on: Oct 13, 2017
 *      Author: tong60514
 */

#ifndef META_H_
#define META_H_
#include <stdlib.h>
#include <bitmap.h>

typedef void (*preprocess)(struct metadata* meta);
struct metadata
{
	void* data_mem_ptr;
	DECLARE_BITMAP(ops,64);
	ssize_t struct_bytes;
	ssize_t data_bytes;
	preprocess prep;
};


#define _get_offset(ptr,size_bytes) (((char*)ptr)+size_bytes)





/* construct data flag */

#define DEFAULT 0UL
#define PASS_BY_POINTER 1UL



#endif /* META_H_ */
