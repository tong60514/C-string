/*
 * meta.h
 *
 *  Created on: Oct 13, 2017
 *      Author: tong60514
 */

#ifndef META_H_
#define META_H_
#include <stdlib.h>
#define META_MAX 3
struct metadata
{
	int length;
	ssize_t offset[META_MAX];
};

#define __META_LENGTH(meta) meta.length
#define __META_OFFSET(ptr,size_bytes) (((char*)ptr)+size_bytes)

#endif /* META_H_ */
