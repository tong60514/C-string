/*
 * copy.c
 *
 *  Created on: Nov 3, 2017
 *      Author: tong60514
 */

#include <string.h>
#include <stdlib.h>
#include <struct.h>


void* do_copy(struct metadata* m)
{
	struct metadata* res = malloc(m->container_struct_size);
	memcpy(res,m,m->container_struct_size);
	res->data_mem_ptr = malloc(m->data_bytes);
	memcpy(res->data_mem_ptr,m->data_mem_ptr,m->data_bytes);
	return res;
}
void do_copy_to(struct metadata* m,struct metadata* res)
{
	memcpy(res,m, m->container_struct_size);
	res->data_mem_ptr = malloc(m->data_bytes);
	memcpy(res->data_mem_ptr,m->data_mem_ptr,m->data_bytes);
}
