/*
 * strsearch.h
 *
 *  Created on: Oct 10, 2017
 *      Author: tong60514
 */

#ifndef STRSEARCH_H_
#define STRSEARCH_H_
#include "list.h"

typedef struct pattern_index_list
{
	struct llist listhead;
	int index;
}index_list;


struct pattern_info
{
	char* pat;
	int patlen;
	int* faif;
	index_list list;
};
void add_index(index_list* list,int index);
int getsize(char *str_buffer);
void subs_str_to(char* str,struct pattern_info* info,char* to,int pre);
int* failure(char *str);
void kmpMatch(char*,struct pattern_info*);

void kmp_str_pattern(char* str,char* pat,struct pattern_info* info);
void sub_all_pattern(char* str,char* pat);



#endif /* STRSEARCH_H_ */
