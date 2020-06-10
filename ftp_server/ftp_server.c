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
#include "ftp_func.h"
#include "common.h"

conn gconn;

pthread_t p_thread;
int thr_id;
int exit_process;

static void print_info()
{
	static const char *help = 
	"==================================== 	\n"
	"|            ftp server            | 	\n"
	"==================================== 	\n"
	"usage: ftp-server [-option] [value] 	\n"
	"-H : Help to ftp server usage 			\n"
	"-v : Confirm to ftp server version 	\n"
	"-p : Setting ftp server port 			\n";

	fprintf(stderr, help);
	exit(0);
}

void *send_thread(void *data)
{
	int sock_cnt = 0;
	pid_t pid;
	pthread_t tid;
	int size = 0;

	char *msg = "\0";

	FILE *fp;

	pid = getpid();
	tid = pthread_self();

	printf("[send_thread] pid: %u, tid:%x \n", (unsigned int)pid, (unsigned int)tid);
	
	//while(1) 
	{
		gconn.send_sockfd[sock_cnt] = accept(gconn.recv_sockfd, 
			(struct sockaddr*)&gconn.recvaddr, (socklen_t *)&gconn.recv_addr_len);
	
		if(gconn.send_sockfd[sock_cnt] == FAIL)
		{
			printf("accept failed (ret:%d)(errno:%d) \n", gconn.send_sockfd[sock_cnt], errno);
			exit_process = 1;
		}

		while(!exit_process)
		{
			//printf("Listening... %d \n", gconn.send_sockfd[sock_cnt]);
			read(gconn.send_sockfd[sock_cnt], gconn.buffer, BUF_LEN);
			
			//printf("recv buf: %s ", gconn.buffer);
			
			if(!strcmp(gconn.buffer, "exit")) {
				exit_process = 1;
				break;
			}else if(!strcmp(gconn.buffer, "ls")) {
				//system("ls");
				
				fp = popen("ls", "r");
				if(NULL == fp){
					perror("popen() failed");
					exit_process = 1;
				}

				fread((void*)gconn.send_buf, sizeof(char), BUF_LEN, fp);
				printf("%s", gconn.send_buf);

				pclose(fp);

				/*
				while(fgets(gconn.send_buf, BUF_LEN, fp))
				{
					printf("%s", gconn.send_buf);
				}
				*/
				
				//fgets(gconn.send_buf, BUF_LEN, fp);
				//getcwd(gconn.send_buf, BUF_LEN);
				write(gconn.send_sockfd[sock_cnt], gconn.send_buf, BUF_LEN);
				
			}else if(!strncmp(gconn.buffer, "cd", 2)) {
			//}else if(!strcmp(gconn.buffer, "cd")) {
				printf("cd! %s \n", gconn.buffer);
				//system(gconn.buffer);
				//system("cd ..");
				//if(chdir(gconn.buffer + 3) == 0)
				//	write(gconn.send_sockfd[sock_cnt], "1", BUF_LEN);
				
				parse_cmd(gconn.buffer, msg);
				printf("cmd : %s, parsing : %s \n", gconn.buffer , msg);
			}else if (!strcmp(gconn.buffer, "pwd")) {
				getcwd(gconn.send_buf, BUF_LEN);
				write(gconn.send_sockfd[sock_cnt], gconn.send_buf, BUF_LEN);
			}else if(strstr(gconn.buffer, "get")) {
				printf("Get: %s \n", gconn.buffer);
				size = get_file(gconn.buffer, gconn.send_buf);
				write(gconn.send_sockfd[sock_cnt], gconn.send_buf, size);
			}else if(!strcmp(gconn.buffer, "put")) {
				printf("Put: %s \n", gconn.buffer);
				put_file(gconn.buffer, gconn.send_buf);
			}
			

			if(exit_process)
			{
				printf("exit send_thread: %d\n", exit_process);
				break;
			}

			//printf("Connected Client Status: %d \n", exit_process);
		}

		close_socket();
		exit_process = 0;
		//sock_cnt++;
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
	//gconn.recvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
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
	pid_t pid;

	while((c = getopt(argc, argv, "Hp:")) != -1) {
		switch(c) {
			case 'H':
			print_info();
			break;
			case 'p':
			gconn.port = atoi(optarg);
			option = 1;
			break;
			case '?':
			printf("command not found \n");
			exit(0);
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
		gconn.port = DEF_PORT;
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

	// log_open

/*
	pid = fork();
	if (pid == -1) {
		perror("fork");
		cleanup_exit(1);
	}
	if (pid != 0) {		
		close(sock);
		snprintf(pidstrbuf, sizeof pidstrbuf, "%ld", (long)pid);
		if (ac == 0) {
			format = c_flag ? "setenv %s %s;\n" : "%s=%s; export %s;\n";
			printf(format, SSH_AUTHSOCKET_ENV_NAME, socket_name,
			    SSH_AUTHSOCKET_ENV_NAME);
			printf(format, SSH_AGENTPID_ENV_NAME, pidstrbuf,
			    SSH_AGENTPID_ENV_NAME);
			printf("echo Agent pid %ld;\n", (long)pid);
			exit(0);
		}
		if (setenv(SSH_AUTHSOCKET_ENV_NAME, socket_name, 1) == -1 ||
		    setenv(SSH_AGENTPID_ENV_NAME, pidstrbuf, 1) == -1) {
			perror("setenv");
			exit(1);
		}
		execvp(av[0], av);
		perror(av[0]);
		exit(1);
	}
*/

	// log_init


	run_process();	
		
	pthread_join(p_thread, NULL);

	close_socket();

	printf("exit process\n");

	return 0;
}
