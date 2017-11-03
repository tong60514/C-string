/*
 * copy.c
 *
 *  Created on: Nov 3, 2017
 *      Author: tong60514
 */

#include <string.h>
#include <stdlib.h>
#include <meta.h>
void* copy(struct metadata* m);
void* copy(struct metadata* m)
{
	struct metadata* res = malloc(m->struct_bytes);
	memcpy(res,m,m->struct_bytes);
	res->data_mem_ptr = malloc(m->data_bytes);
	memcpy(res->data_mem_ptr,m->data_mem_ptr,m->data_bytes);
}
