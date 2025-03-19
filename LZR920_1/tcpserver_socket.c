/*
 * tcpserver_socket.c
 *
 *  Created on: 2020. 3. 6.
 *      Author: tommy
 */

#include "lwip/opt.h"
#include "lwip/api.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"
#include "lwip/apps/fs.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "tcpserver_socket.h"
#include "cmsis_os.h"
#include <struct_data.h>
#include <main.h>
#include <gpio.h>

#if defined(LZR_U92x)
#include "LZR_U92x.h"
#elif defined(MICROSCAN)
#include "microscan.h"
#elif defined(FLATSCAN)
#include "FLATSCAN.h"
#endif

#define tcp_server_thread_PRIO  ( tskIDLE_PRIORITY + 4 )
#define BUF_LEN 1024

static int client_fd[MAX_CLIENT] = {0, }; //Static Global client socket for server thread
//static int client_socket; // Static Global client socket number in client thread

static unsigned char client_alive[MAX_CLIENT] = {0, };

#if	defined(MULTI_PORT)
/*
static int multiport_server_fd[MAX_CLIENT]; //server socket number
static void tcp_server_multi_port_socket_thread(void *arg)
{
	static struct sockaddr_in server_addr, client_addr;
	unsigned char buffer[BUF_LEN];
	char temp[50];
	int len;
	int msg_size;
	int port_num;

	double tilt_h;
	double tilt_l;

	int thread_num = *(int*) arg;

	//to make sure the thread number
	//printf("TCP THRED %d\n\r", thread_num);

	if((multiport_server_fd[thread_num] = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{// Create server socket
		printf("Server : Can't open stream socket\n");
		exit(0);
	}
	memset(&server_addr, 0x00, sizeof(server_addr));
	//server_Addr Null initialize

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;

	if(thread_num == 0)
	{
		//port_num = SERVER_PORT1;
		port_num = 5000;
		server_addr.sin_port = htons(port_num);
	}
	else
	{
		//port_num = SERVER_PORT2;
		port_num = 5001;
		server_addr.sin_port = htons(port_num);
	}

	if(bind(multiport_server_fd[thread_num], (struct sockaddr *)&server_addr, sizeof(server_addr)) <0)
	{//bind() server socket bind
		printf("Server : Can't bind local address.\n\r");
		exit(0);
	}

	if(listen(multiport_server_fd[thread_num], 5) < 0)
	{//server socket listen
		printf("Server : Can't listening connect.\n\r");
		exit(0);
	}

	memset(buffer, 0x00, sizeof(buffer));

	len = sizeof(client_addr);

	while(1)
	{
		client_fd[thread_num] = accept(multiport_server_fd[thread_num], (struct sockaddr *)&client_addr, (socklen_t *)&len);

		if(client_fd[thread_num] < 0)
		{
			printf("Server: accept failed.\n\r");
			exit(0);
		}
		else
		{
			inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, temp, sizeof(temp));

			do {
				msg_size = read(client_fd[thread_num], buffer, BUF_LEN);

				if (msg_size>0) //no error
				{ 
					//To do...

				}//End of if (msg_size>0) //no error
			}  while (msg_size>0);
			close(client_fd[thread_num]);
		}
	}
	close(multiport_server_fd[thread_num]);
}
*/
#endif

#if defined(MULTI_CLIENT)
static int server_ready = 0;
static struct sockaddr_in server_addr;
static struct sockaddr_in client_addr[MAX_CLIENT];
static int server_fd = 0;

int clinet_cnt = 0;

static void tcp_server_multi_client_socket_thread(void *arg)
{
	unsigned char buffer[BUF_LEN];
	char temp[20];
	int len;
	int msg_size;
	//int i;
	double tilt_h;
	double tilt_l;

	int thread_num = *(int*) arg;

	int i;

	// 0 thread only can initialize the address and port
	if(thread_num == 0)
	{

		if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{// Create server socket
			printf("Server : Can't open stream socket\n");
			exit(0);
		}
		memset(&server_addr, 0x00, sizeof(server_addr));
		//server_Addr Null initialize

		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.s_addr = INADDR_ANY;
		server_addr.sin_port = htons(5000);

		if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <0)
		{//bind() server socket bind
			printf("Server : Can't bind local address.\n\r");
			exit(0);
		}

		if(listen(server_fd, 5) < 0)
		{//server socket listen
			printf("Server : Can't listening connect.\n\r");
			exit(0);
		}

		memset(buffer, 0x00, sizeof(buffer));

		//printf("Server : Waiting connection request.\n\r");
		//len = sizeof(client_addr[thread_num]);


		server_ready = 1;
	}
	else
	{
		while(1)
		{
			if(server_ready == 1)
				break;
		}
	}

	//printf("Server : Waiting connection request.\n\r");
	len = sizeof(client_addr[thread_num]);

	//printf("tcp_thread number %d\r\n", thread_num);
	
	printf("tcp_thread number %d : Waiting connection request.\r\n", thread_num);


	while(1)
	{
		client_fd[thread_num] = accept(server_fd, (struct sockaddr *)&client_addr[thread_num], (socklen_t *)&len);

		//clinet_cnt += 1;
		//printf("Client count : %d\r\n", clinet_cnt);


		if(client_fd[thread_num] < 0)
		{
			printf("Server: accept failed.\n\r");
			exit(0);
		}
		else
		{
			inet_ntop(AF_INET, &client_addr[thread_num].sin_addr.s_addr, temp, sizeof(temp));

			printf("Server : %s socket %d client connected.\n\r", temp, client_fd[thread_num]);

			client_alive[thread_num] = 1;

			do {
				msg_size = read(client_fd[thread_num], buffer, BUF_LEN);


				if (msg_size>0) { //no error

					/*
					for(int i=0; i < msg_size; i+=1)
					{
						printf("0x%2x ", buffer[i]);
					}
					printf("\r\n");
					*/

					//Check head part
					if(
							(buffer[0] == 0x53 ) &&
							(buffer[1] == 0x52 ) 
					  )
					{
						//tcp_cmd[0] = 0;
						//tcp_cmd[1] = 0;
						
						switch(buffer[3])
						{
							case CMD_USEMODE:
							case CMD_CONFIGMODE:
							case CMD_LEFTMODE:
							case CMD_RIGHTMODE:
								//printf("CMD 0x%2x\r\n", buffer[3]);
								break;

							case CMD_SETTILT:
								tilt_h = (double)GET_MSB_DATA(buffer, 1, INTEGER);
								tilt_l = (double)GET_MSB_DATA(buffer, 2, INTEGER)/1000.0;
								SENSOR_TILT[buffer[2]] = (tilt_h + tilt_l);// % 360.0;
								break;

							case CMD_LOADTILT:
							case CMD_SETBGD:
							case CMD_LOADBGD:
								//printf("CMD 0x%2x\r\n", buffer[3]);
								break;

							case CMD_SETZONE:

								AREA_NUMBER[buffer[2]] = buffer[20];
								detect_zone[buffer[2]][AREA_NUMBER[buffer[2]]].x1 = GET_MSB_DATA(buffer, 1, INTEGER);
								detect_zone[buffer[2]][AREA_NUMBER[buffer[2]]].y1 = GET_MSB_DATA(buffer, 2, INTEGER);
								detect_zone[buffer[2]][AREA_NUMBER[buffer[2]]].x2 = GET_MSB_DATA(buffer, 3, INTEGER);
								detect_zone[buffer[2]][AREA_NUMBER[buffer[2]]].y2 = GET_MSB_DATA(buffer, 4, INTEGER);

								break;
							case CMD_LOADZONE:
								//printf("CMD 0x%2x\r\n", buffer[3]);
								break;

							case CMD_SETSIZE:
								MIN_OBJECT_SIZE[buffer[2]] = buffer[4];
								MAX_OBJECT_SIZE[buffer[2]] = buffer[5];
								break;

							case CMD_LOADSIZE:
								//printf("CMD 0x%2x\r\n", buffer[3]);
								break;

							case CMD_SETIP:
								if(
										(buffer[4] != 0) &&
										(buffer[5] != 0) &&
										(buffer[6] != 0) &&
										(buffer[7] != 0)
								  )
								{
									IP_ADDR[0] = buffer[4];
									IP_ADDR[1] = buffer[5];
									IP_ADDR[2] = buffer[6];
									IP_ADDR[3] = buffer[7];
								}

								if(
										(buffer[8] != 0) &&
										(buffer[9] != 0) &&
										(buffer[10] != 0) &&
										(buffer[11] != 0) 
								  )
								{
									NETMASK_ADDR[0] = buffer[8];
									NETMASK_ADDR[1] = buffer[9];
									NETMASK_ADDR[2] = buffer[10];
									NETMASK_ADDR[3] = buffer[11];
								}

								if(
										(buffer[12] != 0) &&
										(buffer[13] != 0) &&
										(buffer[14] != 0) &&
										(buffer[15] != 0) 
								  )
								{
									GW_ADDR[0] = buffer[12];
									GW_ADDR[1] = buffer[13];
									GW_ADDR[2] = buffer[14];
									GW_ADDR[3] = buffer[15];
								}
								break;
							case CMD_IGNOREDETECTION:
								RELAY_ENABLED[buffer[2]] = buffer[4];
								break;

							case CMD_REBOOT:
								break;

							case CMD_SETMINTIME	:
								MIN_DETECT_TIME[buffer[2]] = GET_MSB_DATA(buffer, 1, INTEGER);
								break;

                            case CMD_SETHOLDTIME:
								SIG_HOLD_TIME[buffer[2]] = GET_MSB_DATA(buffer, 1, INTEGER);
								break;

							case CMD_SETRELAY :

								for( i = 0; i < MAX_AREA; i+=1)
								{
									relay_status[buffer[2]][i].relay_assign_num = buffer[4+i];
								}
								break;

							case CMD_LOADRELAY :
								break;

							default:
								break;
						}
						tcp_cmd[buffer[2]] = buffer[3];
					}
				} //End of if (msg_size>0)
			}	while (msg_size>0); //do-while
			close(client_fd[thread_num]);
			client_alive[thread_num] = 0;
			printf("Server : Socket %d client closed.\n\r", client_fd[thread_num]);

			//clinet_cnt -= 1;
			//printf("Client count : %d\r\n", clinet_cnt);

			//if(thread_num == 0)
			{
				tcp_cmd[0] = CMD_USEMODE;
				tcp_cmd[1] = CMD_USEMODE;
			}
		}
	}
	close(server_fd);
}

void client_socket_close(int thread_num)
{
	close(client_fd[thread_num]);
	client_alive[thread_num] = 0;
}

#endif


static void tcp_client_socket_thread(void *arg)
{
	//int socket_error = -1;
	//int test_count = 0;

	while(1)
	{
	}
}


void tcp_server_socket_init(void)
{
	int thread_num;
	for(thread_num = 0; thread_num < MAX_CLIENT; thread_num++)
	{
#ifdef MULTI_CLIENT
		sys_thread_new("TCP_SERVER", tcp_server_multi_client_socket_thread, (void *) &thread_num, DEFAULT_THREAD_STACKSIZE *3, tcp_server_thread_PRIO);
#endif

#ifdef MULTI_PORT
		sys_thread_new("TCP_SERVER", tcp_server_multi_port_socket_thread, (void *) &thread_num, DEFAULT_THREAD_STACKSIZE *3, tcp_server_thread_PRIO);
#endif

	}
}

void tcp_client_socket_init(void)
{
	sys_thread_new("TCP_CLIENT", tcp_client_socket_thread, NULL, DEFAULT_THREAD_STACKSIZE *2, tcp_server_thread_PRIO);
}

int tcp_server_write(const void *buff, int len, int socket_number)
{
	//return write(client_fd[socket_number], buff, len);
	if(client_alive[socket_number] == 1)
		return write(client_fd[socket_number], buff, len);
	else
		return -1;
}

int tcp_client_write(void *buff, int len, int socket_number)
{
	return write(client_fd[socket_number], buff, len);
}

void print_socket( const char *format, ...)
{
	int i;
	va_list ap;
	int size;
	char buf[200];

	va_start(ap, format);
	size=vsprintf(buf, format, ap);
	va_end(ap);

	for(i=0; i< MAX_CLIENT; i++)
	{
		if(client_alive[i] == 1)
		{
			tcp_server_write(buf, size, i);
		}
		else
		{
			;
		}
	}

}


