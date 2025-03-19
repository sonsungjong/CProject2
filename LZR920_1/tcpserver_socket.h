#ifndef __TCPSERVER_SOCKET_H__
#define __TCPSERVER_SOCKET_H__

void tcp_server_socket_init(void);
void tcp_client_socket_init(void);
int tcp_server_write(const void *buff, int len, int socket_number);
void client_socket_close(int thread_num);

#define MULTI_CLIENT
#if defined(MULTI_CLIENT)
	//#define MAX_CLIENT 2
	#define MAX_CLIENT 1
#else
	#define MULTI_PORT
	#define MAX_CLIENT 2
#endif

#endif
