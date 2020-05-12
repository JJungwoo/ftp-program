#include <stdio.h>
#include <stdlib.h>

#include <string.h>

// conn
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

// signal
#include <signal.h>

// error
#include <errno.h>

#define BUF_LEN 1024

// function failed
#define FAIL -1
#define OK 0

typedef struct ftp_conn_st{
	// server 
	int recv_sockfd;
	int recv_len;
	int recv_addr_len;
	struct sockaddr_in recvaddr;

	// client 
	int send_sockfd;
	struct sockaddr_in sendaddr;

	char buffer[BUF_LEN];
}conn;


