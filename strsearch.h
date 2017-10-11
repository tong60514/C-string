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

void add_index(index_list* list,int index);

struct pattern_info
{
	char* pat;
	int patlen;
	int* faif;

	int ret_nums;
	index_list list;
};

int getsize(char *str_buffer);
void match_str_pattern(char* str,struct pattern_info* info);
void init_pattern(struct pattern_info* info,char *pattern,int retnum);
void subs_str_to(char* str,char* pat,char* to,int pre);
int* failure(char *str);
void kmpMatch(char*,struct pattern_info*);


#endif /* STRSEARCH_H_ */
