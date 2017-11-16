#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strsearch.h>
int getsize(char *str_buffer);


void init_pattern(struct pattern_info* info,char *pattern)
{
	int pattern_length = strlen(pattern);
	info->patlen = pattern_length;
	info->pat = malloc(pattern_length);
	memcpy(info->pat,pattern,pattern_length);
	INIT_LIST(info->list.listhead);
}




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
	int pat_length = strlen(info->pat);

	int* failf = malloc(sizeof(int)*(pat_length+1));
	int k=0;
	for(int q=1;q<pat_length;q++)
	{
		while( k!=0 && info->pat[k]!=info->pat[q])k =failf[k];
		if(info->pat[k]==info->pat[q])
			k++;
		failf[q+1]= k;
	}

	/*matching part*/
	int str_length = strlen(str);
	k=0;
	for(int i=0;i<str_length;i++)
	{
		while(k>0&&info->pat[k]!=str[i])
			k = failf[k];
		if(info->pat[k]==str[i])
			k++;
		if(k==info->patlen)
		{
			k=failf[k];
			add_index(&(info->list),i-pat_length+1);
		}
	}
	free(failf);
}


int* zfunc(char* str)
{
	int strlen = getsize(str);
	int* _zarr = malloc(sizeof(int)*(strlen));

	int L=0,R=0;
	for(int i=1;i<strlen;i++)
	{
		int ii = i-L;
		if(i>R)
		{
			int x = i;
			while( x < strlen && str[x]==str[x-i])x++;
			_zarr[i] = x-i;
			if(x>i)R=x-1;L=i;
			if(str[x]==str[x-i])
				_zarr[i]= x-i;
		}
		else if(_zarr[ii]>=R-i+1)
		{
			int x = R+1;
			while( x < strlen && str[x]==str[x-i])x++;
			_zarr[i] = x-i;
			L = i;R = x-1;
			if(str[x]==str[x-i])
				_zarr[i]= x-i;
		}
		else
			_zarr[i] = _zarr[ii];
	}
	return _zarr;
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






void subs_str_to(char* str,struct pattern_info* info,char* to,int pre)
{
	int tolen = strlen(to);
	struct llist* pos;
	struct llist* head = &info->list.listhead;
	int count = 0;
	LIST_FOR_EACH(pos,head)
	{
		int index = (LIST_ENTRY(pos,index_list,listhead))->index;
		for(int j=0;j<tolen;j++)
			str[index+j] = to[j];
		if(count==pre)
			break;
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


void kmp_str_pattern(char* str,char* pat,struct pattern_info* info)
{
	init_pattern(info,pat);
	kmpMatch(str,info);
	return ;
}

