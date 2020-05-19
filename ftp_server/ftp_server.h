
#define BUF_LEN 1024

// function failed
#define FAIL -1
#define OK 0

#define DEF_PORT 9999

typedef struct ftp_conn_st{
	// server 
	int recv_sockfd;
	int recv_len;
	int recv_addr_len;
	struct sockaddr_in recvaddr;

	// client 
	int send_sockfd[5];
	struct sockaddr_in sendaddr;

	int port;

	char buffer[BUF_LEN];
	char send_buf[BUF_LEN];
}conn;

void close_socket();
