#include <stdio.h>
#include <string.h>
#include <fcntl.h> 

#include "ftp_func.h"
#include "ftp_server.h"
#include "common.h"

static int
do_upload(char *filename, char *buf, size_t size)
{

}

int  
get_file(char *msg, char *buf)
{
    int *fd;
    int len = strlen(msg);
    int ret = 0;
    int size = 0, count = 0;

    // command parsing
    printf("path: %d %s \n", len, msg);
    char *path = strstr(msg, " ");
    *path++;
    printf("path: %s \n", path);

    // upload file

    fd = open(path, O_CREAT | O_WRONLY | O_EXCL, 0644);
    if(0 > fd){
        printf("file open failed\n");
        close(fd);
        return -1;
    }
    
    fseek(fd, 0, SEEK_END);
    size = ftell(fd);
    memset(buf, 0, size + 1);
    buf = (char*)malloc(size);
    count = fread(buf, size, 1, fd);

    fclose(fd);

    return size;
}

static int
do_download(char *filename, char *buf, size_t size)
{
     //FILE *fd;
    int fd;
    int ret = 0;

    fd = open(filename, O_CREAT | O_WRONLY | O_EXCL, 0644);
    //fp = open(filename, "w+");
    if(0 > fd){
        printf("file open failed\n");
        close(fd);
        return -1;
    }

    //fseek(fp, 0, SEEK_END);
    //size = ftell(fp);
    //printf("ftell size: %d \n", size);
    //memset(buf, 0, size+1);

    //fwrite(fp, buf, size);
    ret = write(fd, buf, strlen(buf));
    if(0 > ret){
        printf("file write failed\n");
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}

int 
put_file(char *msg, char *buf)
{
    int *fd;
    int len = strlen(msg);
    int ret = 0;
    int size = 0, count = 0;

    // command parsing
    printf("path: %d %s \n", len, msg);
    char *path = strstr(msg, " ");
    *path++;
    printf("path: %s \n", path);

    // download file
    ret = do_download(path, buf, BUF_LEN);
    if(0 > fd){
        printf("do_download failed\n");
        return -1;
    }

    return 0;
}

