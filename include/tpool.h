/*
 * tpool.h
 *
 *  Created on: Oct 19, 2017
 *      Author: tong60514
 */

#ifndef TPOOL_H_
#define TPOOL_H_
#include <pthread.h>
#include <bitmap.h>
#include <pthread_routine.h>
typedef void *(*pthread_rout)(void *);
typedef void* task_ptr;
typedef struct thread_pool
{
	int nthrds;
	int maxthrds;
	pthread_t* thrd;
	pthread_mutex_t mutex_task;
	pthread_cond_t non_empty;

	unsigned long pool_status[1];

	struct pthread_task* head;
	struct pthread_task* tail;

}tpool;


struct task_attr
{
	int prior;
};






struct pthread_task
{
	struct pthread_task* next;
	unsigned long task_flag[1];

	struct pthread_routine routine_data;

	void* arg;
	struct task_attr attr;
};


/* 64 bits */
#define TASK_RUN 0

#define TASK_WAIT 1

#define TASK_INTERRUPT 2




task_ptr add_task(tpool* pool,pthread_rout task,
			void* arg,struct task_attr* attr);
int del_task(tpool* pool,task_ptr t);






#endif /* TPOOL_H_ */
