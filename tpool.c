#include <pthread.h>
#include <tpool.h>
#include <stdlib.h>
#include <assert.h>

static inline task_set_attr(struct pthread_task* task,struct task_attr* attr)
{

	task->attr.prior = attr->prior;
	return;
}
task_ptr add_task(tpool* pool,pthread_rout task,void* arg,struct task_attr* attr)
{
	assert(pool->nthrds<=pool->maxthrds);
	if(pool->nthrds==pool->maxthrds)
		return NULL;


	struct pthread_task* new_task = malloc(sizeof(struct pthread_task));


	task_set_attr(new_task,attr);

	/*        add task to pool
	 *
	 * -------critical section start---------- */
	pthread_mutex_lock(&pool->mutex_task);
	if(pool->head==NULL)
	{
		pool->head = new_task;
		pool->tail = new_task;
	}
	else
	{
		pool->tail->next = new_task;
		pool->tail = new_task;
	}
	pool->nthrds++;
	pthread_cond_signal(&pool->nonempty);

	pthread_mutex_unlock(&pool->mutex_task);
	/* -------critical section end ---------- */



	return (void*)new_task;

}
int del_task(tpool* pool,task_ptr t)
{
	struct pthread_task* task = (struct pthread_task*)t;
	/*        del task from pool
	 *
	 * -------critical section start---------- */

	pthread_mutex_lock(&pool->mutex_task);


	pthread_mutex_unlock(&pool->mutex_task);
	/* -------critical section end ---------- */


	return 0;
}

