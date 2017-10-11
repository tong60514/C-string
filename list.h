/*
 * list.h
 *
 *  Created on: Oct 11, 2017
 *      Author: tong60514
 */

#ifndef LIST_H_
#define LIST_H_

#define OFFSET(type,field) &((type*)0)->field
#define LIST_ENTRY(ptr,type,field) (type*)((char*)ptr - (unsigned long)OFFSET(type,field))
#define INIT_LIST(list)  list.next = &list;list.prev=&list;

#define LIST_ADD(head,new_list)\
			head->next->prev = new_list;\
			new_list->next = head->next;\
			head->next = new_list;\
			new_list->prev = head;
#define LIST_REMOVE(list) list->prev->next = list->next;list->next->prev = list->prev;

#define FOR_EACH(ptr,list_head) \
for(struct llist* ptr=list_head.next;ptr!=&list_head;ptr=ptr->next)


struct llist
{
	struct llist* next;
	struct llist* prev;
};


#endif /* LIST_H_ */
