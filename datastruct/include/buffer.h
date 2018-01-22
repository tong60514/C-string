/*
 * buffer.h
 *
 *  Created on: Nov 1, 2017
 *      Author: tong60514
 */

#ifndef BUFFER_H_
#define BUFFER_H_

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 4096
#endif

#include <bitmap.h>
#include <stdlib.h>
typedef struct circular_buffer
{
	char buf[BUFFER_SIZE];
	char buf_end;



	unsigned long flag[1];
	unsigned long head;
	unsigned long tail;
}cc_buf;








#define COUNT_BUFFER(buf) \
(buf->head>buf->tail?BUFFER_SIZE-buf->head+buf->tail:buf->tail-buf->head)




static __always_inline inline int check_left(cc_buf* b)
{
	return COUNT_BUFFER(b);
}

static __always_inline inline void poke_buf(cc_buf* b,char data)
{
	b->buf[b->tail] = data;
	b->tail = (b->tail+1)%BUFFER_SIZE;
}
static __always_inline inline char pick_buf(cc_buf* b)
{
	if(b->head==b->tail)
		return '\0';
	char c = b->buf[b->head];
	b->head = (b->head+1)%BUFFER_SIZE;
	return c;
}


static __always_inline inline void do_read_buf_all(cc_buf* b,char* data)
{
	char* buf_head = &b->buf[b->head];
	char* buf_tail = &b->buf[b->tail];
	if(b->head>b->tail)
	{
		while(buf_head<&b->buf_end)
		{
			*data = *buf_head;
			buf_head+=1;
			data+=1;
		}
		b->head = 0;
	}
	while(buf_head!=buf_tail)
	{
		*data = *buf_head;
		data+=1;
		buf_head+=1;
	}
	b->head = ++b->tail;
	return;
}


#define read_buf_all(b,data) do_read_buf_all(b,data)
#define empty_buffer(buf) buf->head==buf->tail
#define full_buffer(buf) ((buf->tail+1)%BUFFER_SIZE)==buf->head
#define INIT_CIRCLULAR_BUF(c) \
	do\
	{\
		((cc_buf*)c)->head = 0;\
		((cc_buf*)c)->tail = 0;\
	}while(0)




#endif /* BUFFER_H_ */
