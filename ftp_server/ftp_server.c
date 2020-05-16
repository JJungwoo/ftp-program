#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <signal.h>
#include <pthread.h>
#include <errno.h>

#include "ftp_server.h"

conn gconn;

pthread_t p_thread;
int thr_id;
int exit_process = 0;

void print_info()
{
	static const char *help = 
	"================================ \n"
	"|          ftp server          | \n"
	"================================ \n"
	"usage: ftp-server [-option] \n"
	"-h : Help to ftp server usage \n"
	"-v : Confirm to ftp server version \n"
	"-d : Execute to ftp server daemon \n"
	"-c : Create to ftp server user \n"
	"-p : Setting ftp server port \n";

	fputs(help, stdout);
}

void *send_thread(void *data)
{
	pid_t pid;
	pthread_t tid;

	pid = getpid();
	tid = pthread_self();

	printf("[send_thread] pid: %u, tid:%x \n", (unsigned int)pid, (unsigned int)tid);

	while(1)
	{
		printf("Listening...\n");
		gconn.send_sockfd = accept(gconn.recv_sockfd, 
			(struct sockaddr*)&gconn.recvaddr, (socklen_t *)&gconn.recv_addr_len);

		if(gconn.send_sockfd == FAIL)
		{
			printf("accept failed (ret:%d)(errno:%d) \n", gconn.send_sockfd, errno);
			pthread_exit(NULL);
		}

		read(gconn.send_sockfd, gconn.buffer, BUF_LEN);
		printf("recv buf: %s ", gconn.buffer);

		if(exit_process)
		{
			printf("exit send_thread: %d\n", exit_process);
			break;
		}

		printf("Connected Client \n");
	}
	
	pthread_exit(NULL);
}

int run_process()
{
	printf("run_process \n");

	thr_id = pthread_create(&p_thread, NULL, send_thread, (void *)NULL);
	if(thr_id < 0)
	{
		printf("thread create error : %d \n", errno);
		exit(0);
	}

	return OK;
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
	printf("create_conn_st - port: %d \n", gconn.port);
	memset(&gconn.recvaddr, 0, sizeof(gconn.recvaddr));
	gconn.recvaddr.sin_family = AF_INET;
	gconn.recvaddr.sin_port = htons(gconn.port);
	gconn.recvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	//gconn.recvaddr.sin_addr.s_addr = htonl(atoi("127.0.0.1"));

	gconn.recv_addr_len = sizeof(gconn.sendaddr);

	ret = bind(gconn.recv_sockfd, (struct sockaddr*)&gconn.recvaddr, sizeof(gconn.recvaddr));
	if(ret == FAIL)
	{
		printf("[create_conn_st] bind failed (ret:%d) %d \n", ret, errno);
		return FAIL;
	}

	ret = listen(gconn.recv_sockfd, 5);
	if(ret == FAIL)
	{
		printf("[create_conn_st] listen failed (ret:%d) \n", ret);
		return FAIL;
	}

	return ret;
}

void close_socket()
{
	exit_process = 1;
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

/*
	for(int i=0;i<argc;i++)
	{
		if(*argv[i] == '-')
		{
			switch(*(argv[i]+1))
			{
				case 'h':
				print_info();
				return OK;
				break;
			}
		}		
	}
*/
	if(option == 0)
	{
		print_info();
		exit(0);
	}

	signal(SIGINT, signal_handler);

	ret = create_socket();
	if(ret == FAIL) 
	{
		printf("create socker failed (ret:%d) \n", ret);
		exit(0);
	}

	ret = create_conn_st();
	if(ret == FAIL) 
	{
		printf("create_conn_st failed (ret:%d) \n", ret);
		exit(0);
	}

	run_process();	
		
	printf("test\n");

	pthread_join(p_thread, NULL);

	close_socket();

	return 0;
}
