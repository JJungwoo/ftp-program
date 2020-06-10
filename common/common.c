#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "common.h"

int
parse_cmd(char *cmd, char *msg)
{
	int len = strlen(cmd);
	int ret = 0;
	int size = 0, count = 0;

	// command parsing
	printf("parse_cmd \n data: %d %s \n", len, cmd);
	char *data = strstr(cmd, " ");
	*data++;
	if(*data == NULL) {
		printf("parse_cmd has failed\n");
		return -1;
	}
	printf("path: %s (%d) \n", data, strlen(data));
	
	printf("data: %s, msg %s \n", data, msg);
	//msg = (char *)malloc(strlen(data)+1);
	//strcpy(data, msg);
	strcpy(msg, data);
	printf("data: %s, msg %s \n", data, msg);
	return 0;
}

void 
printHex(char *funcname, char *pData, unsigned int sz)
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




