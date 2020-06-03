



#define BUF_LEN 1024

// function failed
#define FAIL -1
#define OK 0

#define DEF_PORT 9999
#define DEF_IP "127.0.0.1"

typedef struct pear_conn_st{
	// server 
	int recv_sockfd;
	int recv_len;
	int recv_addr_len;
	struct sockaddr_in recvaddr;

	// client 
	int send_sockfd;
	struct sockaddr_in sendaddr;

	char buffer[BUF_LEN];

	int port;
	char *ip[64];

	FILE *fp;

}conn;

char cmdbuf[BUF_LEN];
