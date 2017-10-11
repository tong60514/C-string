#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strsearch.h"
#include "list.h"

struct str_loc
{
	struct llist list_head;
	int index;
};



int main()
{
	struct pattern_info info;
	char test[] = "abc bcb cbc abc bca bcc bca bca cba cb abc";
	char pat[]="abc";
	char yo[] = "ppp";
	init_pattern(&info,pat,10);
	kmpMatch(test,&info);
	subs_str_to(test,pat,yo,20);

	fprintf(stderr,"%s==========",test);
	return 0;
}
