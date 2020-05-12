#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "common.h"

void printHex(char *funcname, char *pData, unsigned int sz)
{
	int i;
	int size = sz;

	printf("printHex: [%s] \n", funcname);
	printf("=========start=========\n");
	for(i=0;i<size;i++)
	{
		if(isprint(pData[i]))
			printf(" %c ", pData[i]);
		else 
			printf(" . ");
		if(i % 8 == 0)
			printf("\n");
	}
	printf("\n");
	printf("==========end==========\n");
}

//int 




