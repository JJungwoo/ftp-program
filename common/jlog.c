#include "jlog.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>

static void jtime(char *time_str, size_t time_str_size)
{
	int rc;
	time_t tt;
	struct tm local_time;

	time(&tt);

	localtime_r(&tt, &local_time);
	rc = strftime(time_str, time_str_size, "%m/%d/%T", &local_time);
	printf("jtime : %d \n", rc);
}

int jformat(int flag, const char *file, const long line, const char *fmt, ... )
{
	char time_str[20 + 1];
	FILE *fp = NULL;

	switch(flag){
		case DEBUG:
			fp = fopen("./jlog_debug.log", "a");
			if(!fp) return -1;
			jtime(time_str, sizeof(time_str));
			fprintf(fp, "%s [DEBUG] (%d:%s:%ld) ", time_str, getpid(), file, line);
			break;
		case ERROR:
			fp = fopen("./jlog_error.log", "a");
			if(!fp) return -1;
			jtime(time_str, sizeof(time_str));
			fprintf(fp, "%s [ERROR] (%d:%s:%ld) ", time_str, getpid(), file, line);
			break;
	}

	return 0;
}


int main(){

	char s[100];
	int rc;

	jtime(s, sizeof(s));

	printf("%s\n", s);
	return 0;
}

