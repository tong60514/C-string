#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strsearch.h"
int getsize(char *str_buffer);


void init_pattern(struct pattern_info* info,char *pattern,int retnum)
{
	int pattern_length = strlen(pattern);
	info->ret_nums = retnum;
	info->patlen = pattern_length;
	info->pat = malloc(pattern_length);
	memcpy(info->pat,pattern,pattern_length);
	INIT_LIST(info->list.listhead);
}



void match_str_pattern(char* str,struct pattern_info* info);

int* failure(char *str)
{
	int len = strlen(str);
	int* failfunc = malloc(sizeof(int)*(len+1));
	int k=0;
	for(int q=1;q<len;q++)
	{
		while( k!=0 && str[k]!=str[q])
			k =failfunc[k];
		if(str[k]==str[q])
			k++;
		failfunc[q+1]= k;
	}
	return failfunc;
}


void kmpMatch(char* str,struct pattern_info* info)
{
	/*failure funtion
	 * the failure function of the pattern we compute first
	 * and it'll have pattern's length +1 elements
	 * by defination of the ff
	 * Pn it means there's a string call P[]
	 * and Pn denote the substring of P from P[0]....P[n]
	 *
 	 * f(i) = max{k : k<q and Pk is a suffix of Pq }
 	 * for example :
 	 *  string P = { abcabaabc }
 	 *  the failure function will look like this:
 	 *  F = { 0, 0,0,0,1,2,1,1,2,0 }
 	 *  F[n] will have n-1's failure value
 	 *	the reason I do this is  I want to
 	 *	reduce the caculation when the compare fail (caculate n-1)
 	 *	to index the previous f()
 	 *  the first element is means
 	 *  when no char match while loop will decrease q to zero
 	 *  and chech the first char */
	/*pattern part*/
	int str_length = strlen(info->pat);

	int* failf = malloc(sizeof(int)*(str_length+1));
	int k=0;
	for(int q=1;q<str_length;q++)
	{
		while( k!=0 && info->pat[k]!=info->pat[q])k =failf[k];
		if(info->pat[k]==info->pat[q])
			k++;
		failf[q+1]= k;
	}

	/*matching part*/
	str_length = strlen(str);
	k=0;
	for(int i=0;i<str_length;i++)
	{
		while(q>0&&info->pat[k]!=str[i])
			k = failf[k];
		if(info->pat[k]==str[i])
			k++;
		if(k==info->patlen)
			k=failf[k];
	}
	free(failf);
}


void zMatch(char* str,struct pattern_info* info)
{
	int rescount = 0;
	int size = info->ret_nums;
	int strlen = getsize(str);
	int patlen = info->patlen;

	int* ptrn_z = malloc(sizeof(int)*(strlen));
	int L=0,R=0;
	for(int i=1;i<strlen;i++)
	{
		int ii = i-L;
		int index = i-patlen;
		if(i>R)
		{
			int x = i;
			while( x < strlen && str[x]==str[x-i])x++;
			ptrn_z[i] = x-i;
			if(x>i)R=x-1;L=i;
			if(ptrn_z[i]>=patlen&&rescount<size&&index>=0)
			{
				add_index(&(info->list),index);
				rescount++;
			}
		}
		else if(ptrn_z[ii]>=R-i+1)
		{
			int x = R+1;
			while( x < strlen && str[x]==str[x-i])x++;
			ptrn_z[i] = x-i;
			L = i;R = x-1;
			if(ptrn_z[i]>=patlen&&rescount<size&&index>=0)
			{
				add_index(&(info->list),index);
				rescount++;
			}
		}
		else
			ptrn_z[i] = ptrn_z[ii];
	}
	free(ptrn_z);
}


#define BUFFERLEN 1024
int getsize(char *str_buffer)
{
	ssize_t size = 0;
	char *ptr = str_buffer;
	int i = 1023;
	while(i==1023)
	{
		for(i=0;i<1024;i++)
		{
			if(ptr[i]=='\0')
				break;
			size++;
		}
	}
	return size;
}

void match_str_pattern(char* str,struct pattern_info* info)
{
	int str_len = strlen(str);
	int ptr_len = info->patlen;
	char* cmbstr = malloc(str_len+ptr_len);
	memcpy((void*)cmbstr,info->pat,ptr_len);
	memcpy((void*)(cmbstr+ptr_len),str,str_len);
	zMatch(cmbstr,info);
	free(cmbstr);
}



void subs_str_to(char* str,char* pat,char* to,int pre)
{
	struct pattern_info* info = malloc(sizeof(struct pattern_info));
	int tolen = strlen(to);
	init_pattern(info,pat,pre);
	match_str_pattern(str,info);
	FOR_EACH(ptr,info->list.listhead)
	{
		int index = (LIST_ENTRY(ptr,index_list,listhead))->index;
		for(int j=0;j<tolen;j++)
			str[index+j] = to[j];
	}
}

void add_index(index_list* list,int index)
{
	index_list* new_loc = malloc(sizeof(index_list));
	new_loc->index = index;
	struct llist* h = &(list->listhead);
	struct llist* n = &(new_loc->listhead);
	LIST_ADD(h,n);
}
