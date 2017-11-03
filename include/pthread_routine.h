/*
 * task.h
 *
 *  Created on: Oct 19, 2017
 *      Author: tong60514
 */

#ifndef PTHREAD_ROUTINE_H_
#define PTHREAD_ROUTINE_H_
#include <setjmp.h>

typedef void *(*pthread_rout)(void *);

struct pthread_routine
{
	pthread_rout task_rt;
};





#endif /* PTHREAD_ROUTINE_H_ */
