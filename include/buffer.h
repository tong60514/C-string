/*
 * buffer.h
 *
 *  Created on: Nov 1, 2017
 *      Author: tong60514
 */

#ifndef BUFFER_H_
#define BUFFER_H_

struct buffer
{
	void* head;
	void* tail;
	ssize_t size;
};



#endif /* BUFFER_H_ */
