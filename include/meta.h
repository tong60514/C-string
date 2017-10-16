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
#define META_MAX 3
struct metadata
{
	int length;
	DECLARE_BITMAP(ops,64);
	ssize_t offset[META_MAX];
	ssize_t mat_bytes;
};

#define __META_LENGTH(meta) meta.length
#define __META_OFFSET(ptr,size_bytes) (((char*)ptr)+size_bytes)

#define data_offset 0
#define index_offset 1

#endif /* META_H_ */
