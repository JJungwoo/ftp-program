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
	"===================================== 	\n"
	"|            ftp client             |	\n"
	"===================================== 	\n"
	"usage: ftp-client [-option] [value] 	\n"
	"-H : Help to ftp client usage 			\n"
	"-v : Confirm to ftp client version 	\n"
	"-c : Create to ftp user 				\n"
	"-p : Setting ftp target server port	\n"
	"-h : Setting ftp target server ip		\n";

	fputs(help, stdout);
	exit(0);
}

int create_socket()
{
	int ret = 0;
	int opt_val = 1;

	ret = gconn.recv_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(ret == FAIL) {
		printf("[create_socket] socket failed (ret:%d) \n", ret);
		return FAIL;
	}

	setsockopt(gconn.recv_sockfd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val));

	return ret;
}

int create_conn_st()
{
	int ret = 0;

	if(gconn.ip[0] == '\0') {
		strcpy(gconn.ip, DEF_IP);
	}

	if(gconn.port == 0) {
		gconn.port = DEF_PORT;
	}

	memset(&gconn.recvaddr, 0, sizeof(gconn.recvaddr));
	gconn.recvaddr.sin_family = AF_INET;
	gconn.recvaddr.sin_port = htons(gconn.port);
	gconn.recvaddr.sin_addr.s_addr = inet_addr(gconn.ip);

	gconn.recv_addr_len = sizeof(gconn.sendaddr);

	return ret;
}

void close_process()
{
	close(gconn.recv_sockfd);
}

void signal_handler(int signo)
{
	printf("signal_hander \n");
	signal(SIGINT, SIG_DFL);
	close_process();
}

int main(int argc, char **argv)
{
	int ret = 0, c = 0; 
	int option = 0;
	char filename[1024] = "\0";

	while((c = getopt(argc, argv, "Hph:")) != -1) 
	{
		switch(c) 
		{
			case 'H':
			print_info();
			break;
			case 'p':
			gconn.port = atoi(optarg);
			option = 1;
			break;
			case 'h':
			strcpy(gconn.ip, optarg);
			option = 1;
			break;
			case '?':
			printf("command not found \n");
			exit(0);
			break;
		}
	}

	if(option == 0) {
		//print_info();
		//gconn.port = DEF_PORT;
		terminal_mode = 1;
	}

	signal(SIGINT, signal_handler);

	ret = create_socket();
	if(ret == FAIL) {
		printf("create socker failed (ret:%d) \n", ret);
		exit(0);
	}

	printf("get sockfd:%d \n", gconn.recv_sockfd);

	ret = create_conn_st();
	if(ret == FAIL) {
		printf("create_conn_st failed (ret:%d) \n", ret);
		exit(0);
	}

	ret = connect(gconn.recv_sockfd, (struct sockaddr*)&gconn.recvaddr, sizeof(gconn.recvaddr));
	if(ret == FAIL) {
		perror("ftp server connection failed \n");
		close_process(gconn.recv_sockfd);
		exit(0);
	}
	else {
		printf("Connected Success \n");
	}

	//write(gconn.recv_sockfd, "client enrol\n", BUF_LEN);

	if(terminal_mode) 
	{
		while(!exit_process) 
		{

			printf("ftp>");
			fgets(cmdbuf, BUF_LEN, stdin);
			
			write(gconn.recv_sockfd, cmdbuf, BUF_LEN);
			//fprintf(stderr, "ftp>%s \n", cmdbuf);

			if(!strcmp(cmdbuf, "exit\n") || !strcmp(cmdbuf, "quit\n")){
				exit_process = 1;
				break;
			}else if(!strcmp(cmdbuf, "get\n")){		// file download
				printf("input file name: \n");
				fgets(filename, 1024, stdin);

				read(gconn.recv_sockfd, gconn.buffer, BUF_LEN);
				
			}else if(!strcmp(cmdbuf, "put\n")){		// file upload

			}else if(!strcmp(cmdbuf, "pwd\n")){
				read(gconn.recv_sockfd, gconn.buffer, BUF_LEN);
				printf("recv buf: \n %s ", gconn.buffer);
			}else if(!strcmp(cmdbuf, "ls\n")){
				read(gconn.recv_sockfd, gconn.buffer, BUF_LEN);
				printf("recv buf: \n %s ", gconn.buffer);
			}else if(!strcmp(cmdbuf, "cd\n")){
				
			}else if(!strcmp(cmdbuf, "help\n")){
				
			}
		}
	}
	else // command mode
	{

	}
	
	close_process();

	return 0;
}
