/*
 * struct.h
 *
 *  Created on: Oct 13, 2017
 *      Author: tong60514
 */



#ifndef STRUCT_H_
#define STRUCT_H_
#include <polynomial.h>
#include <matrix.h>
#include <vector.h>
#include <bitmap.h>
#include <meta.h>
#include <data_t.h>


void do_copy_to(struct metadata* m,struct metadata* res);

#define copy(ptr,type)  ((type)do_copy((struct metadata)ptr))
#define copy_to(ptr,to)  \
	do_copy_to((struct metadata*)ptr , (struct metadata*)to)

#endif /* STRUCT_H_ */
