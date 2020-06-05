#include <stdio.h>
#include <string.h>

#include "ftp_func.h"

int  
get_file(char *msg)
{
    FILE *fp;
    
    char *path = strchr(msg, " ");
    printf("path: %s\n", path);
    

    return 0;
}

int 
put_file(char *msg)
{

    return 0;
}

