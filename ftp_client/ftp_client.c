#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <signal.h>

#include <errno.h>

#include "ftp_client.h"

conn gconn;
int terminal_mode;
int exit_process;

void print_info()
{
	static const char *help = 
	"================================ \n"
	"|          ftp client          | \n"
	"================================ \n"
	"usage: ftp-client [-option] \n"
	"-h : Help to ftp client usage \n"
	"-v : Confirm to ftp client version \n"
	"-d : Execute to ftp client daemon \n"
	"-c : Create to ftp client user \n"
	"-p : Setting ftp client port \n";

	fputs(help, stdout);
}

int create_socket()
{
	int ret = 0;
	int opt_val = 1;

	ret = gconn.recv_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(ret == FAIL) 
	{
		printf("[create_socket] socket failed (ret:%d) \n", ret);
		return FAIL;
	}

	setsockopt(gconn.recv_sockfd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val));

	return ret;
}

int create_conn_st()
{
	int ret = 0;

	memset(&gconn.recvaddr, 0, sizeof(gconn.recvaddr));
	gconn.recvaddr.sin_family = AF_INET;
	gconn.recvaddr.sin_port = htons(gconn.port);
	gconn.recvaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	gconn.recv_addr_len = sizeof(gconn.sendaddr);

	return ret;
}

void close_socket()
{
	close(gconn.recv_sockfd);
}

void signal_handler(int signo)
{
	printf("signal_hander \n");
	signal(SIGINT, SIG_DFL);
	close_socket();
}

int main(int argc, char **argv)
{
	int ret = 0, c = 0; 
	int option = 0;

	while((c = getopt(argc, argv, "hp:")) != -1) {
		switch(c) {
			case 'h':
			print_info();
			return OK;
			break;
			case 'p':
			gconn.port = atoi(optarg);
			option = 1;
			break;
		}
	}

	if(option == 0)
	{
		//print_info();
		gconn.port = 9999;
		terminal_mode = 1;
		//exit(0);
	}

	signal(SIGINT, signal_handler);

	ret = create_socket();
	if(ret == FAIL) 
	{
		printf("create socker failed (ret:%d) \n", ret);
		exit(0);
	}

	printf("get sockfd:%d \n", gconn.recv_sockfd);

	ret = create_conn_st();
	if(ret == FAIL) 
	{
		printf("create_conn_st failed (ret:%d) \n", ret);
		exit(0);
	}

	ret = connect(gconn.recv_sockfd, (struct sockaddr*)&gconn.recvaddr, sizeof(gconn.recvaddr));
	if(ret == FAIL) {
		perror("fail ..\n");
		printf("FAIL\n");
		close_socket(gconn.recv_sockfd);
	}
	else {
		printf("Connected Success \n");
	}


	if(terminal_mode){
		while(!exit_process) {
			printf("ftp>");
			fgets(cmdbuf, BUF_LEN, stdin);
			//fprintf(stderr, "ftp>%s \n", cmdbuf);
			if(!strcmp(cmdbuf, "exit\n") || !strcmp(cmdbuf, "quit\n")){
				exit_process = 1;
			}else if(!strcmp(cmdbuf, "get\n")){	// 파일 다운
				printf("get! %s\n", cmdbuf);
				fgets(cmdbuf, BUF_LEN, stdin);
				send(gconn.recv_sockfd, cmdbuf, strlen(gconn.buffer), 0);
				send(gconn.recv_sockfd, "test1", strlen(gconn.buffer), 0);
				send(gconn.recv_sockfd, "test2", strlen(gconn.buffer), 0);

			}else if(!strcmp(cmdbuf, "put\n")){	// 파일 업로드

			}else if(!strcmp(cmdbuf, "pwd\n")){

			}else if(!strcmp(cmdbuf, "ls\n")){
				
			}else if(!strcmp(cmdbuf, "cd\n")){
				
			}else if(!strcmp(cmdbuf, "help\n")){
				
			}

		}
	}
	else {

		
		
	}
	
	close_socket();

	return 0;
}
