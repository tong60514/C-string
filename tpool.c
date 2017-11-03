#include <pthread.h>
#include <tpool.h>
#include <stdlib.h>
#include <assert.h>
#include <setjmp.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>
extern int errno;


#define DEBUG
#ifdef DEBUG
#define __pthread_mutex_debug_(func,mesg) \
		do{\
			errno = func;\
			perror(mesg);\
		}while(0)

#define PTHREAD_MUTEX_LOCK(mutex) do{\
	errno = pthread_mutex_lock(mutex);\
	if(errno)\
	{\
		perror("pthread_mutex_lock error");\
		fprintf(stderr,"at %s line %d\n",__FILE__,__LINE__);\
	}\
	}while(0)
#define PTHREAD_MUTEX_UNLOCK(mutex) do{\
	errno = pthread_mutex_unlock(mutex);\
	if(errno)\
	{\
		perror("pthread_mutex_unlock error");\
		fprintf(stderr,"at %s line %d\n",__FILE__,__LINE__);\
	}\
	}while(0)




#define PTHREAD_MUTEX_DESTROY(mutex) \
	do{\
		errno = pthread_mutex_destroy(mutex);\
		if(errno)\
		{\
			perror("pthread_mutex_destroy error");\
			fprintf(stderr,"at %s line %d\n",__FILE__,__LINE__);\
		}\
	}while(0)
#define PTHREAD_MUTEX_INIT(mutex,attr) \
	do{\
		errno = pthread_mutex_init(mutex,attr);\
		if(errno)\
		{\
			perror("pthread_mutex_init error");\
			fprintf(stderr,"at %s line %d\n",__FILE__,__LINE__);\
		}\
	}while(0)



#define PTHREAD_COND_INIT(cond,attr) \
	do{\
		errno = pthread_cond_init(cond,attr);\
		if(errno)\
		{\
			perror("pthread_cond_init error");\
			fprintf(stderr,"at %s line %d\n",__FILE__,__LINE__);\
		}\
	}while(0)

#define PTHREAD_COND_WAIT(cond,mutex) \
	do{\
		errno = pthread_cond_wait(cond,mutex);\
		if(errno)\
		{\
			perror("pthread_cond_wait error");\
			fprintf(stderr,"at %s line %d\n",__FILE__,__LINE__);\
		}\
	}while(0)



#define PTHREAD_CANCEL(pid) \
	do{\
		errno = pthread_cancel(pid);\
		if(errno)\
		{\
			perror("pthread_cancel error");\
			fprintf(stderr,"at %s line %d\n",__FILE__,__LINE__);\
		}\
	}while(0)
#else
#define PTHREAD_MUTEX_LOCK(mutex)      pthread_mutex_lock(arg)
#define PTHREAD_MUTEX_UNLOCK(mutex)    pthread_mutex_unlock(arg)
#define PTHREAD_MUTEX_INIT(mutex,attr) pthread_mutex_init(mutex,attr)
#define PTHREAD_MUTEX_DESTROY(mutex)   pthread_mutex_destroy(mutex)
#define PTHREAD_COND_INIT(cond,attr)   pthread_cond_init(cond,attr)
#define PTHREAD_COND_WAIT(cond,mutex)  pthread_cond_init(cond,mutex)
#define PTHREAD_CANCEL(pid)            pthread_cancel(pid)
#endif




static inline void task_set_attr(struct pthread_task* task,struct task_attr* attr)
{
	task->attr.prior = attr->prior;
}
pthread_task_ptr tpool_add_task(tpool* pool,pthread_rout task_rt,void* arg,struct task_attr* attr)
{
	struct pthread_task* new_task = NULL;
	if(test_bit(POOL_INIT,pool->flag))
	{
		new_task = malloc(sizeof(struct pthread_task));

		new_task->pthread_task_rt_info.task_rt = task_rt;
		new_task->arg = arg;
		new_task->task_flag[0] = 0;
		/*        add task to pool
		 *
		 * -------critical section start---------- */


		pthread_mutex_lock(&pool->mutex_task);
		if(pool->head==NULL)
		{
			pool->head = new_task;
			pool->tail = new_task;
			new_task->prev_next_ptr = NULL;
		}
		else
		{
			new_task->prev_next_ptr = &pool->tail->next;
			pool->tail->next = new_task;
			pool->tail = new_task;
		}
		pthread_cond_signal(&pool->empty);
		pthread_mutex_unlock(&pool->mutex_task);

		/* -------critical section end ---------- */
	}

	else
	{
		pool->pool_errno = STATE_UNAVAILABLE;

	}


	return (void*)new_task;

}
void tpool_del_task(tpool* pool,pthread_task_ptr t)
{
	struct pthread_task* task = (struct pthread_task*)t;
	/*        del task from pool
	 *
	 * -------critical section start---------- */

		pthread_mutex_lock(&pool->mutex_task);



	if(test_bit(TASK_WAIT,task->task_flag))
		*task->prev_next_ptr = task->next;



		pthread_mutex_unlock(&pool->mutex_task);

	/* -------critical section end ---------- */

	free(t);

	return ;
}
void tpool_init(tpool* pool,int nthrds)
{

	pool->maxthrds = nthrds;
	pool->head = NULL;
	pool->tail = NULL;
	pool->flag[0] = 0;
	pool->thrds_info = malloc(sizeof(struct pthread_info)*nthrds);


	SET_BIT(pool->flag,POOL_INIT);

	pool->thrd = malloc(sizeof(pthread_t)*nthrds);
	PTHREAD_MUTEX_INIT(&pool->mutex_task,NULL);

	PTHREAD_COND_INIT(&pool->empty,NULL);




	printf(" tpool init  \n");

}
static void* find_next_task(struct pthread_info* info);






			/*error handling
			 *
			 * in the funtion
			 * 		add_task ,del_task ,cancel task
			 * 		should  implement error handling by signal or interrupt
			 * 		if error occurs and it will be asynchronous
			 *
			 * thers's serveral chioce to implement the error handling
			 *
			 * 1 . check the error at the end
			 * 2 . if there's error occurs and break the for loop
			 * 		clean up the threads left in pool init state.
			 * 3 . implement by longjmp if the error occurs clean up the pool
			 * 		or do something user handle function by option
			 *
			 *
			 *
			 *
			 *
			 * last : implment all of it but provide a option struct to specified
			 *
			 *
			 *
			 *
			 * */
void tpool_start(tpool* pool)
{

	int err;
	int success_thread = 0;
	SET_BIT(pool->flag,POOL_ON);
	if(test_bit(POOL_INIT,pool->flag))
	{
		for(int i=0;i<pool->maxthrds;i++)
		{
			pool->thrds_info[i].belong = pool;
			pool->thrds_info[i].id = pool->thrd[i];
			err = pthread_create(&pool->thrd[i],NULL,find_next_task,&pool->thrds_info[i]);
			/*error handling*/
			if(err)
			{
				pool->pool_errno = err;
				errno = err;
				perror("pthread_create");
				break;
			}
			success_thread++;
			/*error handling*/
		}
	}
	else
	{
		pool->pool_errno = STATE_UNAVAILABLE;

		/*clean up thread left in pool */

		for(int i =0;i<pool->maxthrds;i++)
		{
			PTHREAD_CANCEL(pool->thrd[i]);
		}
	}



}


static void* find_next_task(struct pthread_info* info)
{

	struct pthread_task* task_get;
	tpool* tp = info->belong;


	assert(&tp->mutex_task!=NULL);

	while(1)//test_bit(POOL_ON,tp->flag)
	{
		pthread_mutex_lock(&tp->mutex_task);

		while(tp->head==NULL)
			pthread_cond_wait(&tp->empty,&tp->mutex_task);


		task_get = tp->head;
		tp->head = tp->head->next;
		pthread_mutex_unlock(&tp->mutex_task);




		/*should be atomic*/
		SET_BIT(task_get->task_flag,TASK_RUN);

		task_get->pthread_task_rt_info.task_rt(task_get->arg);
		free(task_get);
	}
	return NULL;
}

void tpool_cancel(tpool* pool)
{
	if(test_bit(POOL_ON,pool->flag))
	{
		PTHREAD_MUTEX_LOCK(&pool->mutex_task);
		CLEAR_BIT(pool->flag,POOL_ON);
		for(int i=0;i<pool->maxthrds;i++)
		{
			PTHREAD_CANCEL(pool->thrd[i]);
			/*error handling*/
		}


		PTHREAD_MUTEX_UNLOCK(&pool->mutex_task);
	}
	else
		pool->pool_errno = STATE_UNAVAILABLE;
}
