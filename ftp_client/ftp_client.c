#include "ftp_client.h"

conn gconn;

void print_info()
{
	//printf("============================= \n");
	printf("##################### \n");
	printf("#    ftp server     # \n");
	printf("##################### \n");
	//printf("============================= \n");
	printf("-h : Help to p2p agent usage \n");
	printf("-v : Confirm to p2p agent version \n");
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

int create_conn_st(int port)
{
	int ret = 0;

	memset(&gconn.recvaddr, 0, sizeof(gconn.recvaddr));
	gconn.recvaddr.sin_family = AF_INET;
	gconn.recvaddr.sin_port = htons(port);
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
	int ret = 0;
	if (argc-1 <= 0)
	{
		printf("bad usage\n");
		print_info();
		return OK;
	}

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

	signal(SIGINT, signal_handler);
	/*while(1){
		printf("TEST\n");
		sleep(10);
	}*/

	ret = create_socket();
	if(ret == FAIL) 
	{
		printf("create socker failed (ret:%d) \n", ret);
		return FAIL;
	}

	printf("%d \n", gconn.recv_sockfd);

	ret = connect(gconn.recv_sockfd, (struct sockaddr*)&gconn.recvaddr, sizeof(gconn.recvaddr));
	if(ret == FAIL) {
		close_socket(gconn.recv_sockfd);
	}
	else {
		printf("Connected Success \n");
	}

	close_socket();

	return 0;
}
