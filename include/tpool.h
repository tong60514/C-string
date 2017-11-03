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
typedef void* pthread_task_ptr;
struct pthread_info;


typedef struct thread_pool
{
	int maxthrds; // max threads in pool
	pthread_t* thrd; // pthread id

	pthread_mutex_t mutex_task; // find task mutex
	pthread_cond_t empty; // cond of empty task to do
	/* record pool status
	 * interact with funtion
	 *
	 * tpool_start
	 * tpool_init
	 * tpool_cancel
	 *
	 *
	 * */
	unsigned long flag[1];
	/*manage all threads in pool structure*/
	struct pthread_info* thrds_info;


	/*pthread task to do
	 *
	 *  operations
	 *
	 * tpool_add_task
	 * tpool_del_task
	 *
	 *
	 *
	 * */
	struct pthread_task* head;
	struct pthread_task* tail;

	/* 0 if no error occur */
	unsigned long pool_errno;

}tpool;



struct pthread_info
{
	tpool* belong;
	pthread_t id;
};



struct task_attr
{
	int prior;
};






struct pthread_task
{
	/*pthead_task queue
	 *
	 * point to next task */
	struct pthread_task* next;

	/*it points to  prev's    next pointers
	 *
	 * this make the operation tpool_del_task
	 * easier ....
	 * */
	struct pthread_task** prev_next_ptr;


	/*task flag record what happens on it
	 *
	 * and check will iteract with the function
	 *
	 * add and del */
	unsigned long task_flag[1];


	/*
	 * about pthread routine
	 *
	 * entry point
	 * current thread
	 *    */
	struct pthread_routine pthread_task_rt_info;


	/*#### future
	 * maybe and in some exception handling*/
	jmp_buf quit;

	/*the arg will be pass to
	 *   	pthread_task_rt_info.task_rt*/
	void* arg;

	/*#### future
	 * with exception handling or thread management*/
	struct task_attr attr;



	/*current thread*/
	pthread_t current_thread;
};

/*bitmap 64 bits pool flag*/
#define POOL_ON 1
#define POOL_INIT 63


/* unsigned lond errno pool */
#define STATE_UNAVAILABLE 2
/* bitmap 64 bits task status*/
#define TASK_RUN 0

#define TASK_WAIT 1

#define TASK_INTERRUPT 2



/* task operation */
pthread_task_ptr tpool_add_task(tpool* pool,pthread_rout task,
			void* arg,struct task_attr* attr);


void tpool_del_task(tpool* pool,pthread_task_ptr t);



/*tpool operations */
void tpool_start(tpool* pool);
void tpool_init(tpool* pool,int nthrds);
void tpool_cancel(tpool* pool);





#endif /* TPOOL_H_ */
