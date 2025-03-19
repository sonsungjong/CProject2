/**
 ******************************************************************************
 * @file    LwIP/LwIP_HTTP_Server_Netconn_RTOS/Src/main.c
 * @author  MCD Application Team
 * @brief   This sample code implements a http server application based on
 *          Netconn API of LwIP stack and FreeRTOS. This application uses
 *          STM32F7xx the ETH HAL API to transmit and receive data.
 *          The communication is done with a web browser of a remote PC.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics International N.V.
 * All rights reserved.</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted, provided that the following conditions are met:
 *
 * 1. Redistribution of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of other
 *    contributors to this software may be used to endorse or promote products
 *    derived from this software without specific written permission.
 * 4. This software, including modifications and/or derivative works of this
 *    software, must execute solely and exclusively on microcontroller or
 *    microprocessor devices manufactured by or for STMicroelectronics.
 * 5. Redistribution and use of this software other than as permitted under
 *    this license is void and will automatically terminate your rights under
 *    this license.
 *
 * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
 * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
 * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/
#include "tcpserver_socket.h"
#include "main.h"
#include "cmsis_os.h"
#include "ethernetif.h"
#include "lwip/netif.h"
#include "lwip/tcpip.h"
#include "app_ethernet.h"
#include "usart.h"
#include "gpio.h"
#include "math.h"
#include "string.h"
#include "struct_data.h"
#include "i2c.h"
#include "json/cJSON.h"

#if defined(FLATSCAN)

#if defined(LZR_U92x)
#include "LZR_U92x.h"
#elif defined(MICROSCAN)
#include "microscan.h"
#elif defined(FLATSCAN)
#include "FLATSCAN.h"
#endif

/* DEFINES -------------------------------------------------------------------*/

//DEFINES options
#define TCP_SERVER_ENABLED
#define MUTEX_ENABLED
#define DEBUG
#define QUEUE_CORE
//#define STACK_CORE


#if defined(DEBUG)
#define Debug_printf printf
#else
#define Debug_printf
#endif

#define RELAY_DELAY	1000

/* Private functions ---------------------------------------------------------*/
static void SystemClock_Config(void);
static void MPU_Config(void);
static void CPU_CACHE_Enable(void);

//Thread functions 
static void MAIN_THREAD (void const * argument);
static void ETC_THREAD (void const * argument);


#if defined(TCP_SERVER_ENABLED)
static void Netif_Config(void);
static void Server_Thread(void const * argument);
static void TCP_OUTPUT_THREAD(void const *argument);
#endif

//Mutex
void mutex_lock(void);
void mutex_unlock(void);
#if defined(MUTEX_ENABLED)
static osMutexId osMutex;
#endif

#define MAX_DUMMY_CNT	10	

#define XY_PLUS_LIMIT 50 	//mm unit
#define XY_MINUS_LIMIT XY_PLUS_LIMIT*(-1)

#define DUMMY_PLUS_LIMIT	30
#define DUMMY_MINUS_LIMIT	DUMMY_PLUS_LIMIT*(-1)

struct DUMMY{
	unsigned char valid;
	unsigned short min_dist;
	int px;
	int py;
	int last_px;
	int last_py;
};
struct DUMMY current_dummy[MAX_DUMMY_CNT];
unsigned char current_dummy_cnt = 0;
struct DUMMY pre_dummy[MAX_DUMMY_CNT];
unsigned char pre_dummy_cnt = 0;
unsigned char configuration_mode = 0;

// Local functions
static int Init_Sin_Cos_Table(unsigned char port_num, double tilt);

__inline__ static int Get_X_Location(unsigned char port_num, double dist, int index);
__inline__ static int Get_Y_Location(unsigned char port_num, double dist, int index);

//inline function
__inline__ static void dection_function(unsigned char port_num, unsigned short *bgd, unsigned char *data, int x1, int y1, int x2, int y2);
__inline__ static void send_xy_coordinate(unsigned char port_num);

/* Global variables  ---------------------------------------------------------*/

#if !defined(QUEUE_CORE)
char tmp_message[100];
int tmp_message_size;
#endif

#if defined(TCP_SERVER_ENABLED)
struct netif gnetif; /* network interface structure */
unsigned char tcp_connected = 0;
unsigned char IP_VALID = 0;
unsigned char NETMASK_VALID = 0;
unsigned char GW_VALID = 0;

//unsigned int IP_ADDR[4] = {0, };
//unsigned int NETMASK_ADDR[4] = {0, };
//unsigned int GW_ADDR[4] = {0, };

unsigned char IP_ADDR[4] = {0, };
unsigned char NETMASK_ADDR[4] = {0, };
unsigned char GW_ADDR[4] = {0, };

unsigned int SERVER_PORT = 0;
#endif

double SENSOR_TILT[U951_CNT];
double x_table[U951_CNT][MAX_POINT];
double y_table[U951_CNT][MAX_POINT];
struct DETECT_ZONE detect_zone[U951_CNT];
unsigned char install_mode = LEFT_MODE;

unsigned char *tcp_message = NULL;
unsigned char tcp_cmd = 0;
unsigned char error_check = 0;

int main(void) {


	/* Configure the MPU attributes as Device memory for ETH DMA descriptors */
	MPU_Config();

	/* Enable the CPU Cache */
	CPU_CACHE_Enable();

	/* STM32F7xx HAL library initialization:
	   - Configure the Flash ART accelerator on ITCM interface
	   - Configure the Systick to generate an interrupt each 1 msec
	   - Set NVIC Group Priority to 4
	   - Global MSP (MCU Support Package) initialization
	   */
	HAL_Init();

	/* Configure the system clock to 200 MHz */
	SystemClock_Config();

	GPIO_Init();
	EEPROM_Init();
	DEBUG_UART_Init();
	DEBUG_INT_ENABLE();


	/* Custom code start --------------------------------------------------------*/ 

	printf("\033[2J"); //Clear screen
	printf("\r\n");
	printf("********************************************\r\n");
	printf("***** ### #    #  ### ### ###  #  #   # ****\r\n");
	printf("***** #   #   # #  #  #   #   # # ##  # ****\r\n");
	printf("***** ### #   ###  #  ### #   ### # # # ****\r\n");
	printf("***** #   #   # #  #    # #   # # #  ## ****\r\n");
	printf("***** #   ### # #  #  ### ### # # #   # ****\r\n");
	printf("********************************************\r\n");

	//Allco memeory for background_data with Flatscan

	printf("++++++++++++++++++++++++++++++++++++++++++++\r\n");
	if(ALLOC_MEMORY() == 0)
	{
		Error_Handler();
	}
	printf("--------------------------------------------\r\n");

#if defined(QUEUE_CORE)
	init_queue(5);
	Debug_printf("QUEUE CORE 1.1\n\r");
#endif

#if defined(STACK_CORE)
	init_stack(3);
	Debug_printf("STACK CORE 1.0\n\r");
#endif

	tcp_message = (unsigned char *)malloc(sizeof(unsigned char) * QUEUE_MAX_DATA_SIZE);

	if(tcp_message == NULL)
	{
		printf("TCP buffer alloc faild\r\n");
		Error_Handler();
	}

#if defined(TCP_SERVER_ENABLED)
	unsigned char *tmp_data;
	tmp_data = (unsigned char *)malloc(sizeof(unsigned char) * 4);

	eeprom_read(EEPROM_IP_ADDR, tmp_data, 4);
	IP_ADDR[0] = tmp_data[0];
	IP_ADDR[1] = tmp_data[1];
	IP_ADDR[2] = tmp_data[2];
	IP_ADDR[3] = tmp_data[3];

	eeprom_read(EEPROM_NETMASK_ADDR, tmp_data, 4);
	NETMASK_ADDR[0] = tmp_data[0];
	NETMASK_ADDR[1] = tmp_data[1];
	NETMASK_ADDR[2] = tmp_data[2];
	NETMASK_ADDR[3] = tmp_data[3];

	eeprom_read(EEPROM_GW_ADDR, tmp_data, 4);
	GW_ADDR[0] = tmp_data[0];
	GW_ADDR[1] = tmp_data[1];
	GW_ADDR[2] = tmp_data[2];
	GW_ADDR[3] = tmp_data[3];

	eeprom_read(EEPROM_PORT, tmp_data, 4);
	SERVER_PORT = (tmp_data[0] << 24) | (tmp_data[1] << 16) | (tmp_data[2] << 8) | (tmp_data[3]);

	free(tmp_data);

	printf("============================================\r\n");
	printf("|| IP : %3d %3d %3d %3d\r\n", IP_ADDR[0], IP_ADDR[1], IP_ADDR[2], IP_ADDR[3]);
	printf("|| MK : %3d %3d %3d %3d\r\n", NETMASK_ADDR[0], NETMASK_ADDR[1],NETMASK_ADDR[2],NETMASK_ADDR[3]);
	printf("|| GW : %3d %3d %3d %3d\r\n", GW_ADDR[0], GW_ADDR[1],GW_ADDR[2],GW_ADDR[3]);
	printf("|| PT : %d\r\n", SERVER_PORT);
	printf("|| UG : %d\r\n", USER_PORT);
	printf("|| MAC: %02x-%02x-%02x-%02x-%02x-%02x\r\n", MAC_ADDR0, MAC_ADDR1, MAC_ADDR2, MAC_ADDR3, MAC_ADDR4, MAC_ADDR5);
	printf("============================================\r\n");
#endif

	//Read sensor status
	printf("++++++++++++++++++++++++++++++++++++++++++++\r\n");

	int i;
	double tilt_h = 0.0;
	double tilt_l = 0.0;

	unsigned char *tmp = (unsigned char *)malloc(sizeof(unsigned char)*800);

	eeprom_read(EEPROM_MODE, tmp, 1);
	install_mode = tmp[0];
	printf(" >>> INSTALL MODE : %d\r\n", install_mode);

	eeprom_read (EEPROM_DEGREE(U951_1), tmp, 8);
	tilt_h = (double)GET_MSB_DATA(tmp, 0, INTEGER);
	tilt_l = (double)GET_MSB_DATA(tmp, 1, INTEGER)/1000.0;
	SENSOR_TILT[U951_1] = tilt_h + tilt_l;
	printf(" >>> Sensor tilt status : %0.3f\r\n", SENSOR_TILT[U951_1]);
	Init_Sin_Cos_Table(U951_1, SENSOR_TILT[U951_1]);
	printf(" >>> Update tilt table  : OK !!\r\n");

	//This value have to save in cm unit
	eeprom_read(EEPROM_DETECTZONE_ADDR(U951_1), tmp, 16);
	detect_zone[U951_1].x1 = GET_MSB_DATA(tmp, 0, INTEGER);
	detect_zone[U951_1].y1 = GET_MSB_DATA(tmp, 1, INTEGER);
	detect_zone[U951_1].x2 = GET_MSB_DATA(tmp, 2, INTEGER);
	detect_zone[U951_1].y2 = GET_MSB_DATA(tmp, 3, INTEGER);

	printf(" >>> Detection Zone     : \r\n ( x1 = %5d : y1 = %5d )\r\n ( x2 = %5d : y2 = %5d ) \r\n",
			detect_zone[U951_1].x1,
			detect_zone[U951_1].y1,
			detect_zone[U951_1].x2,
			detect_zone[U951_1].y2
		  );
	printf("--------------------------------------------\r\n");

	printf("++++++++++++++++++++++++++++++++++++++++++++\r\n");

	for(i = 0; i < DISTANCE_SIZE; i += 1)
	{
		eeprom_read(EEPROM_BACKGROUND(U951_1) +i, &tmp[i], 1);
	}

	for(i = 0; i < MAX_POINT; i += 1)
	{
		SET_BACKGROUND_DATA(U951_1, 0, i, GET_LSB_DATA(tmp, i, SHORT));

		printf("%5d ", GET_BACKGROUND_DATA(U951_1, 0, i));
		if((i%10) == 9)
		{
			printf("\r\n");
		}
	}
	//printf("\r\n");
	printf(" >>> Load background    : OK !!\r\n");

	free(tmp);
	printf("--------------------------------------------\r\n");
	//Read sensor status

	/* Start thread --------------------------------------------------------*/ 
	printf("++++++++++++++++++++++++++++++++++++++++++++\r\n");
#if defined(MUTEX_ENABLED)
	osMutexDef(osMutex);
	osMutex = osMutexCreate(osMutex(osMutex));
	if(osMutex != NULL)
#endif
	{
#if defined(TCP_SERVER_ENABLED)
		osThreadDef(server, Server_Thread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 5);

		if(osThreadCreate(osThread(server), NULL) != NULL)
		{
			printf("SERVER_THREAD \t\t: OK\r\n");
		}
		else
		{
			Error_Handler();
		}

		osThreadDef(tcp_output, TCP_OUTPUT_THREAD, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 2);
		if(osThreadCreate(osThread(tcp_output), NULL) != NULL)
		{
			printf("TCP_OUTPUT_THREAD \t: OK\r\n");
		}
		else
		{
			Error_Handler();
		}

#endif

		static const unsigned char port_num_1 = U951_1;
		//osThreadDef(U951_1, MAIN_THREAD, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 10);
		osThreadDef(U951_1, MAIN_THREAD, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 5);
		if(osThreadCreate(osThread(U951_1), (void *)&port_num_1) != NULL)
		{
			printf("MAIN_THREAD \t\t: OK\r\n");
		}
		else
		{
			Error_Handler();
		}

		osThreadDef(ERROR_THREAD, ETC_THREAD, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 1);
		if(osThreadCreate(osThread(ERROR_THREAD), NULL) != NULL)
		{
			printf("ETC_THREAD \t\t: OK\r\n");
		}
		else
		{
			Error_Handler();
		}

	}
	printf("--------------------------------------------\r\n");

	/* Start scheduler */
	osKernelStart();

	/* We should never get here as control is now taken by the scheduler */
	while (1) {
	}

}

int mutex_push_queue(unsigned char *data, int data_size)
{
	int i = 0;
#if defined(QUEUE_CORE)

	mutex_lock();
	while(1)
	{
		i = push_queue(data,data_size);
		if(i == 1)
		{
			break;
		}
	}
	mutex_unlock();
#endif
	return i;
}

int mutex_push_stack(unsigned char *data, int data_size)
{
	int i = 0;
#if defined(STACK_CORE)

	mutex_lock();
	while(1)
	{
		i = push_stack(data,data_size);
		if(i == 1)
		{
			break;
		}
	}
	mutex_unlock();
#endif
	return i;

}

static void ETC_THREAD (void const * argument)
{
	UNUSED(argument);

	while(1)
	{
		if(error_check == 1)
		{
			delay(5000);

			if(error_check == 1)
			{
				printf("Sensor working is not stable, need to reset\r\n");
				NVIC_SystemReset();
			}
			else
			{
				break;
			}
		}
	}

	printf("No error on communication\r\n");

	for( ;; )
	{
		/* Delete the Init Thread */
		osThreadTerminate(NULL);
	}

}

static void MAIN_THREAD (void const * argument)
{
	unsigned char port_num = *(unsigned char*)argument;


	//	unsigned int error_cnt = 0;
	unsigned char status = 0;
	unsigned char *data_buffer;
	unsigned int address_offset = 0;
	int i;
	unsigned char d1, d2 = 0;
	unsigned char eeprom_data[50];

	int share, remainder;
	int tilt_h, tilt_l;
	int x1, y1, x2, y2;


	unsigned int heart_beat = 0;

	//Check routine for TCP connection
	while(1)
	{
		if(tcp_connected == 1)
		{
			break;
		}
	}

	if(port_num == U951_1)
	{
		RS485_1_Init();
		UART_LZR_1_INT_ENABLE();
	}
	else
	{
		RS485_2_Init();
		UART_LZR_2_INT_ENABLE();
	}


	status = 0; //initialize status value


	printf("++++++++++++++++++++++++++++++++++++++++++++\r\n");
	printf("MAIN Thread number %d\r\n", port_num);
	while(1)
	{
		if(read_buffer_ready(port_num))
		{
			if( MDI == read_buffer_cmd(port_num))
			{
				printf("Sensor is working......\r\n");
				break;
			}
		}
		move_next_read_buffer(port_num);
	}
	printf("--------------------------------------------\r\n");


	//printf("test \r\n");


	//Initialize dummy data
	for(i = 0; i < MAX_DUMMY_CNT; i += 1)
	{
		current_dummy[i].valid = 0;

		current_dummy[i].min_dist = 0xffff;
		current_dummy[i].px = 0;
		current_dummy[i].py = 0;

		current_dummy[i].last_px = 0;
		current_dummy[i].last_py = 0;


		pre_dummy[i].valid = 0;

		pre_dummy[i].min_dist = 0xffff;
		pre_dummy[i].px = 0;
		pre_dummy[i].py = 0;

		pre_dummy[i].last_px = 0;
		pre_dummy[i].last_py = 0;
	}

	while(1)
	{

		// IP reset part from USER1 switch
		d1 = read_sw(0);
		if((d2 ==0 ) && (d1 == 1))
		{
			eeprom_data[0] = 192;
			eeprom_data[1] = 168;
			eeprom_data[2] = 1;
			eeprom_data[3] = 77;
			eeprom_write(EEPROM_IP_ADDR, eeprom_data, 4);

			eeprom_data[0] = 255;
			eeprom_data[1] = 255;
			eeprom_data[2] = 255;
			eeprom_data[3] = 0;
			eeprom_write(EEPROM_NETMASK_ADDR, eeprom_data, 4);

			eeprom_data[0] = 192;
			eeprom_data[1] = 168;
			eeprom_data[2] = 1;
			eeprom_data[3] = 1;
			eeprom_write(EEPROM_GW_ADDR, eeprom_data, 4);


			//sever port : 5000
			eeprom_data[0] = 0;
			eeprom_data[1] = 0;
			eeprom_data[2] = 0x13;
			eeprom_data[3] = 0x88;
			eeprom_write(EEPROM_PORT, eeprom_data, 4);

			Debug_printf("SET default IP, SYSTEM will be resetting\r\n");

			delay(500);

			NVIC_SystemReset();

		}//End of if((d2 ==0 ) && (d1 == 1))
		d2 = d1;


		//TCP COMMAND part
		switch(tcp_cmd)
		{
			case CMD_USERMODE:
				tcp_cmd = 0;	
				configuration_mode = 0;

				tcp_message[0] = 0; //Not use
				tcp_message[1] = 0; //To GUI
				tcp_message[2] = 0x52;//header 0
				tcp_message[3] = 0x44;//header 1
				tcp_message[4] = 0xff;//header 2
				tcp_message[5] = ACK_USERMODE; //CMD
				mutex_push_queue(tcp_message, 6);

				printf("SET USER MODE\r\n");
				break;

			case CMD_CONFIGMODE	:
				tcp_cmd = 0;
				configuration_mode = 1;

				tcp_message[0] = 0; //Not use
				tcp_message[1] = 0; //To GUI
				tcp_message[2] = 0x52;//header 0
				tcp_message[3] = 0x44;//header 1
				tcp_message[4] = 0xff;//header 2
				tcp_message[5] = ACK_CONFIGMODE; //CMD
				mutex_push_queue(tcp_message, 6);

				printf("SET CONFIG MODE\r\n");
				break;

			case CMD_LEFTMODE	:
				tcp_cmd = 0;

				install_mode = LEFT_MODE;
				eeprom_data[0] = LEFT_MODE;
				
				Init_Sin_Cos_Table(port_num, SENSOR_TILT[port_num]);
				
				eeprom_write(EEPROM_MODE, eeprom_data, 1);

				tcp_message[0] = 0; //Not use
				tcp_message[1] = 0; //To GUI
				tcp_message[2] = 0x52;//header 0
				tcp_message[3] = 0x44;//header 1
				tcp_message[4] = 0xff;//header 2
				tcp_message[5] = ACK_LEFTMODE; //CMD
				mutex_push_queue(tcp_message, 6);
				printf("SET LEFT INSTALLATION\r\n");
				break;

			//case CMD_RIGHTMODE	:
			case CMD_CENTERMODE	:
				tcp_cmd = 0;

				//install_mode = RIGHT_MODE;
				//eeprom_data[0] = RIGHT_MODE;
				install_mode = CENTER_MODE;
				eeprom_data[0] = CENTER_MODE;

				Init_Sin_Cos_Table(port_num, SENSOR_TILT[port_num]);

				eeprom_write(EEPROM_MODE, eeprom_data, 1);

				tcp_message[0] = 0; //Not use
				tcp_message[1] = 0; //To GUI
				tcp_message[2] = 0x52;//header 0
				tcp_message[3] = 0x44;//header 1
				tcp_message[4] = 0xff;//header 2
				//tcp_message[5] = ACK_RIGHTMODE; //CMD
				tcp_message[5] = ACK_CENTERMODE; //CMD
				mutex_push_queue(tcp_message, 6);
				printf("SET CENTER INSTALLATION\r\n");
				break;

			case CMD_SETTILT	:	
				tcp_cmd = 0;

				share = (int)SENSOR_TILT[port_num];
				remainder = (SENSOR_TILT[port_num] - (double)share) * 1000.0;

				SET_MSB_DATA(share, eeprom_data, 0, INTEGER);
				SET_MSB_DATA(remainder, eeprom_data, 4, INTEGER);

				eeprom_write(EEPROM_DEGREE(port_num), eeprom_data, 8);

				Init_Sin_Cos_Table(port_num, SENSOR_TILT[port_num]);

				tcp_message[0] = 0; //Not use
				tcp_message[1] = 0; //To GUI
				tcp_message[2] = 0x52;//header 0
				tcp_message[3] = 0x44;//header 1
				tcp_message[4] = 0xff;//header 2
				tcp_message[5] = ACK_SETTILT; //CMD
				mutex_push_queue(tcp_message, 6);
				printf("SET TILT %f\r\n", SENSOR_TILT[port_num]);
				break;

			case CMD_SETBGD		:
				tcp_cmd = 0;

				//teach in
				teach_in(port_num, 10, 100);

				//write
				for( i=0; i < MAX_POINT; i += 1)
				{
					eeprom_data[0] =  GET_BACKGROUND_DATA(port_num, 0, i) & 0xff;
					eeprom_data[1] = (GET_BACKGROUND_DATA(port_num, 0, i) >>8) & 0xff;

					eeprom_write(EEPROM_BACKGROUND(port_num) + (i << 1), eeprom_data, 2);
				}

				//read
				for( i=0; i < DISTANCE_SIZE; i += 2)
				{
					eeprom_read(EEPROM_BACKGROUND(port_num) + i, eeprom_data, 2);

					printf("%5d ", GET_LSB_DATA(eeprom_data, 0, SHORT));
					if((i%20) == 18)
					{
						printf("\r\n");
					}
				}

				//eeprom write

				tcp_message[0] = 0; //Not use
				tcp_message[1] = 0; //To GUI
				tcp_message[2] = 0x52;//header 0
				tcp_message[3] = 0x44;//header 1
				tcp_message[4] = 0xff;//header 2
				tcp_message[5] = ACK_SETBGD; //CMD
				mutex_push_queue(tcp_message, 6);
				printf("SET BACK GROUND DATA\r\n");
				break;

			case CMD_SETZONE	:		
				tcp_cmd = 0;

				//EEPROM_WRITE
				SET_MSB_DATA(detect_zone[port_num].x1, eeprom_data,  0, INTEGER);
				SET_MSB_DATA(detect_zone[port_num].y1, eeprom_data,  4, INTEGER);
				SET_MSB_DATA(detect_zone[port_num].x2, eeprom_data,  8, INTEGER);
				SET_MSB_DATA(detect_zone[port_num].y2, eeprom_data, 12, INTEGER);

				eeprom_write(EEPROM_DETECTZONE_ADDR(port_num), eeprom_data, 16);

				tcp_message[0] = 0; //Not use
				tcp_message[1] = 0; //To GUI
				tcp_message[2] = 0x52;//header 0
				tcp_message[3] = 0x44;//header 1
				tcp_message[4] = 0xff;//header 2
				tcp_message[5] = ACK_SETZONE; //CMD
				mutex_push_queue(tcp_message, 6);
				printf("SET DETECT ZONE (%d : %d) (%d : %d)\r\n", 
						detect_zone[port_num].x1, detect_zone[port_num].y1, 
						detect_zone[port_num].x2, detect_zone[port_num].y2
					  );
				break;

			case CMD_LOADBGD	:	
				tcp_cmd = 0;	

				for( i=0; i < DISTANCE_SIZE; i += 2)
				{
					eeprom_read(EEPROM_BACKGROUND(port_num) + i, eeprom_data, 2);

					tcp_message[6 + i + 0] = eeprom_data[0];
					tcp_message[6 + i + 1] = eeprom_data[1];

					printf("%5d ", GET_SHORT_DATA(eeprom_data, 0));
					if((i%20)==18)
					{
						printf("\r\n");
					}

				}

				tcp_message[0] = 0; //Not use
				tcp_message[1] = 0; //To GUI
				tcp_message[2] = 0x52;//header 0
				tcp_message[3] = 0x44;//header 1
				tcp_message[4] = 0xff;//header 2
				tcp_message[5] = ACK_LOADBGD; //CMD

				mutex_push_queue(tcp_message, 6+DISTANCE_SIZE);

				printf("GET BACK GROUND DATA\r\n");
				break;

			case CMD_LOADTILT:
				tcp_cmd = 0;	

				eeprom_read (EEPROM_DEGREE(port_num), eeprom_data, 8);
				tilt_h = GET_MSB_DATA(eeprom_data, 0, INTEGER);
				tilt_l = GET_MSB_DATA(eeprom_data, 1, INTEGER);

				tcp_message[0] = 0; //Not use
				tcp_message[1] = 0; //To GUI
				tcp_message[2] = 0x52;//header 0
				tcp_message[3] = 0x44;//header 1
				tcp_message[4] = 0xff;//header 2
				tcp_message[5] = ACK_LOADTILT; //CMD

				SET_MSB_DATA(tilt_h, tcp_message,  6, INTEGER);
				SET_MSB_DATA(tilt_l, tcp_message, 10, INTEGER);

				mutex_push_queue(tcp_message, 14);
				printf("GET SENSOR TILT\r\n");
				break;

			case CMD_LOADZONE:	
				tcp_cmd = 0;

				eeprom_read(EEPROM_DETECTZONE_ADDR(port_num), eeprom_data, 16);

				x1 = GET_MSB_DATA(eeprom_data, 0, INTEGER);
				y1 = GET_MSB_DATA(eeprom_data, 1, INTEGER);
				x2 = GET_MSB_DATA(eeprom_data, 2, INTEGER);
				y2 = GET_MSB_DATA(eeprom_data, 3, INTEGER);

				tcp_message[0] = 0; //Not use
				tcp_message[1] = 0; //To GUI
				tcp_message[2] = 0x52;//header 0
				tcp_message[3] = 0x44;//header 1
				tcp_message[4] = 0xff;//header 2
				tcp_message[5] = ACK_LOADZONE; //CMD

				SET_MSB_DATA(x1, tcp_message,  6, INTEGER);
				SET_MSB_DATA(y1, tcp_message, 10, INTEGER);
				SET_MSB_DATA(x2, tcp_message, 14, INTEGER);
				SET_MSB_DATA(y2, tcp_message, 18, INTEGER);

				mutex_push_queue(tcp_message, 22);

				printf("GET DETECT ZONE (%d : %d) (%d : %d)\r\n", 
						x1, y1, 
						x2, y2
					  );
				break;

			case CMD_SETIP:
				tcp_cmd = 0;
			
				if(IP_VALID == 1)
				{
					IP_VALID = 0;
					eeprom_write(EEPROM_IP_ADDR, IP_ADDR, 4);
					printf("     IP CHANGE %3d.%3d.%3d.%3d\r\n", IP_ADDR[0], IP_ADDR[1], IP_ADDR[2], IP_ADDR[3]);
				}
				
				if(NETMASK_VALID == 1)
				{
					NETMASK_VALID = 0;
					eeprom_write(EEPROM_NETMASK_ADDR, NETMASK_ADDR, 4);
					printf("NETMASK CHANGE %3d.%3d.%3d.%3d\r\n", NETMASK_ADDR[0], NETMASK_ADDR[1], NETMASK_ADDR[2], NETMASK_ADDR[3]);
				}

				if(GW_VALID == 1)
				{
					GW_VALID = 0;
					eeprom_write(EEPROM_GW_ADDR, GW_ADDR, 4);
					printf("GATEWAY CHANGE %3d.%3d.%3d.%3d\r\n", GW_ADDR[0], GW_ADDR[1], GW_ADDR[2], GW_ADDR[3]);
				}
	
				tcp_message[0] = 0; //Not use
				tcp_message[1] = 0; //To GUI
				tcp_message[2] = 0x52;//header 0
				tcp_message[3] = 0x44;//header 1
				tcp_message[4] = 0xff;//header 2
				tcp_message[5] = ACK_SETIP; //CMD

				mutex_push_queue(tcp_message, 6);

				break;

			case CMD_REBOOT:	
				printf("SET default IP, SYSTEM will be resetting\r\n");
				NVIC_SystemReset();
				break;

			default :
				break;
		}
		//TCP COMMAND part


		if(read_buffer_ready(port_num))
		{
			//Set configuration part
			//This part have to seperate from laser scanner reading part
			switch(status)
			{
				case 0:
					set_led_fucntion(U951_1, SINGLE_LED, GREEN, LED_OFF, 1);
					status++;
					break;

				case 1:
					//set_default_config_data(port_num, 0, 10000, MAX_POINT, IMMUNITY_3);
					set_default_config_data(port_num, 0, 10000, MAX_POINT, IMMUNITY_1);
					set_parameter(port_num,MODE_HD);
					beep_on();
					//error_cnt = 0;
					error_check = 1;
					status++;
					break;

				default : //default status	
					break;
			}

			switch(read_buffer_cmd(port_num))
			{
				case SEND_PARAMETERS :
					data_buffer = rawdata_copy(port_num, SYNC_CMD_SIZE);
					read_config_data(data_buffer, port_num);
					//led_off(0);
					beep_off();
					error_check = 0;
					break;

				case SET_LED : //No response
					break;

				case EMERGENCY:

					if(read_config_CAN(port_num))
					{
						data_buffer = rawdata_copy(port_num, SYNC_CMD_SIZE+CAN_CNTR_SIZE);
					}
					else
					{
						data_buffer = rawdata_copy(port_num, SYNC_CMD_SIZE);
					}

					printf("RS485 ERROR %x\n\r", GET_SHORT_DATA(data_buffer, 0));

					printf("HEAD  ERROR %x\n\r", GET_SHORT_DATA(data_buffer, 1));
					break;

				case MDI :

					heart_beat += 1;
					//printf("%d\r\n", heart_beat);

					if((heart_beat % 30) == 0)
					{
						//printf("heart beat\r\n");
						led_toggle(3);
					}
					//*/

					address_offset = 0;

					if(read_config_CAN(port_num))
					{
						address_offset = address_offset+CAN_CNTR_SIZE;
					}

					if(read_config_CTN(port_num))
					{
						address_offset = address_offset+CTN_SIZE;
					}

					if(read_config_FACET(port_num))
					{
						address_offset = address_offset+FACET_SIZE;
					}

					data_buffer = rawdata_copy(port_num, SYNC_CMD_SIZE+address_offset);

					if(configuration_mode == 1)
					{
						//send buffer
						tcp_message[0] = 0; //Not use
						tcp_message[1] = 0; // To GUI
						tcp_message[2] = 0x52;//header 0
						tcp_message[3] = 0x44;//header 1
						tcp_message[4] = 0xff;//header 2
						tcp_message[5] = ACK_CONFIGMODE; //cmd

						memcpy(&tcp_message[6], data_buffer, DISTANCE_SIZE);
						mutex_push_queue(tcp_message, 6+DISTANCE_SIZE);
					}
					else
					{

						dection_function(
								port_num,
								baseline_plane[port_num], 
								data_buffer, 
								detect_zone[port_num].x1, 
								detect_zone[port_num].y1, 
								detect_zone[port_num].x2, 
								detect_zone[port_num].y2
								);
					}
					break; // end of case MDI :

				default : //ERROR
					break;

			}//End of switch(read_buffer_cmd(port_num))

		} //End of read_buffer_ready(port_num))
		move_next_read_buffer(port_num);

	}//End of while(1)

}

__inline__ static void dection_function(unsigned char port_num, unsigned short *bgd, unsigned char *data, int x1, int y1, int x2, int y2)
{	

	unsigned char valid_flag = 0;
	unsigned short dist;

	int i;
	int dummy_cnt = 0;
	int px, py;

	for(i = 0; i < MAX_POINT; i += 1)
	{

		dist = GET_LSB_DATA(data, i, SHORT); //Get laser scanner distance data

		//Clear digit
		dist = dist / 10; //mm to cm, unit change
		dist = dist * 10;
		dist = dist + 20; // add 2cm 

		px = Get_X_Location(port_num, (double)dist, i); // Get x coordinate
		py = Get_Y_Location(port_num, (double)dist, i); // Get y coordinate

		//Clear digit
		px = px / 10;		//mm to cm
		px = px * 10;
		py = py / 10;		//mm to cm
		py = py * 10;


		if(	// Smaller than back ground and x, y coordinate must be between x,y1 and x,y2 
				( dist < bgd[i]) && 
				( (x1 <= px) && (px <= x2) ) &&
				( (y1 <= py) && (py <= y2) )
		  )
		{
			valid_flag = 1;

			//Empty case
			if(current_dummy[dummy_cnt].valid == 0)
			{
				current_dummy[dummy_cnt].valid = 1;

				current_dummy[dummy_cnt].min_dist = dist;
				current_dummy[dummy_cnt].px = px;
				current_dummy[dummy_cnt].py = py;

				current_dummy[dummy_cnt].last_px = px;
				current_dummy[dummy_cnt].last_py = py;
			}
			else //Not empty case
			{
				// if current px and py is close to previous px, py then compare dist 
				// If dist data smaller than previous, update dist, px, py in dummy
				// Matched case
				if(
						((current_dummy[dummy_cnt].last_px + XY_MINUS_LIMIT <= px) && (px <= current_dummy[dummy_cnt].last_px + XY_PLUS_LIMIT)) &&
						((current_dummy[dummy_cnt].last_py + XY_MINUS_LIMIT <= py) && (py <= current_dummy[dummy_cnt].last_py + XY_PLUS_LIMIT))
				  )
				{
					//if(dist < (current_dummy[dummy_cnt].min_dist - MINUS_LIMIT ))
					if(dist < current_dummy[dummy_cnt].min_dist)
					{
						current_dummy[dummy_cnt].min_dist = dist;
						current_dummy[dummy_cnt].px = px;
						current_dummy[dummy_cnt].py = py;
					}

					//last px, py always update to compare with next point value
					current_dummy[dummy_cnt].last_px = px;
					current_dummy[dummy_cnt].last_py = py;
				}

				//Not matched case, move to next dummy
				else // Far from previos px, py case
				{
					//increase the dummy_cnt
					dummy_cnt += 1;

					//dummy_cnt can not over MAX_DUMMY_CNT
					if(MAX_DUMMY_CNT <= dummy_cnt)
					{
						dummy_cnt = MAX_DUMMY_CNT -1;
					}

					//Next dummy awlays and must be empty
					//Set with new values
					current_dummy[dummy_cnt].valid = 1;

					current_dummy[dummy_cnt].min_dist = dist;
					current_dummy[dummy_cnt].px = px;
					current_dummy[dummy_cnt].py = py;

					current_dummy[dummy_cnt].last_px = px;
					current_dummy[dummy_cnt].last_py = py;
				}
			}// End of Not emtpy case
		}
		else 
		{
			; // do nothing
		}
	}//End of for(i = 0; i < MAX_POINT; i += 1)

	//Send x,y point to user
	//If only one data valid, dummy_cnt is 0, so we need to use valid_flag
	if(valid_flag == 1) 
	{
		current_dummy_cnt = dummy_cnt + 1; //we need to add +1 counter for global current_dummy_cnt
		//printf("dummy cnt %d\r\n", current_dummy_cnt);
		
		//Call the function that sending xy coordinate
		send_xy_coordinate(port_num);
	}
	else
	{
		//No valid data, reset as 0 on current_dummy_cnt and pre_dummy list
		current_dummy_cnt = 0;	
		for(i = 0; i < MAX_DUMMY_CNT; i += 1)
		{
			pre_dummy[i].px = 0;
			pre_dummy[i].py = 0;
		}
	}
	//Copy counter data
	pre_dummy_cnt = current_dummy_cnt;
}

__inline__ static void send_xy_coordinate(unsigned char port_num)
{
	int i;
	int j;
	unsigned char message[100];
	const int X_ADDR_OFF_SET =  8;
	const int Y_ADDR_OFF_SET = 12;


	message[0] = 0x00;
	message[1] = 0x01; //User

	//header (3 bytes)
	message[2] = 0x52;
	message[3] = 0x44;
	message[4] = 0xff;

	//cmd (1bytes)
	message[5] = ACK_USERMODE;
	message[6] = 0xEE;
	message[7] = current_dummy_cnt;

	//X, Y (8 bytes)

	//Check routine
	// if current mummy xy value are in pre_dummy xy value, replace from pre value to current value
	for(j = 0; j < pre_dummy_cnt; j += 1)
	{
		for(i = 0; i < current_dummy_cnt; i += 1)
		{
			if( 
					((pre_dummy[j].px + DUMMY_MINUS_LIMIT <= current_dummy[i].px) && (current_dummy[i].px <= pre_dummy[j].px + DUMMY_PLUS_LIMIT)) &&
					((pre_dummy[j].py + DUMMY_MINUS_LIMIT <= current_dummy[i].py) && (current_dummy[i].py <= pre_dummy[j].py + DUMMY_PLUS_LIMIT)) 
			  )
			{
				current_dummy[i].px = pre_dummy[j].px;
				current_dummy[i].py = pre_dummy[j].py;
			}
		}
	}
	//Check routine


	for(i = 0; i < current_dummy_cnt; i += 1)
	{
		if(current_dummy[i].valid == 1)
		{

			current_dummy[i].valid = 0;
			current_dummy[i].min_dist = 0xffff;

			SET_MSB_DATA(current_dummy[i].px-detect_zone[port_num].x1, message, X_ADDR_OFF_SET + (i * 8), INTEGER);
			SET_MSB_DATA(current_dummy[i].py-detect_zone[port_num].y1, message, Y_ADDR_OFF_SET + (i * 8), INTEGER);

			printf("%d >> %5d %5d\r\n",i, current_dummy[i].px-detect_zone[port_num].x1, current_dummy[i].py-detect_zone[port_num].y1);
	
			//copy data from current dummy to pre_dummy
			pre_dummy[i].px = current_dummy[i].px;
			pre_dummy[i].py = current_dummy[i].py;

		}
	}

	//2 + 3+ 1+ 2 + (current_dummy_cnt * 8) 
	mutex_push_queue( message, 8 + (8*current_dummy_cnt) );
}

int GET_LSB_DATA(unsigned char *buff, unsigned int index, unsigned int byte_digit)
{
	int i;
	int result = 0;

	if (((byte_digit % 2) != 0) || byte_digit <= 1)
	{
		printf("This is not even array data, Failed\r\n");
		return 0;
	}

	for(i = byte_digit-1 ; i >= 0; i -= 1)
	{
		result = result | (buff [ (index * byte_digit) + i] << (8 * i) );
		//printf("%x %x \r\n", buff[ (index * byte_digit) + i], result);
	}

	return result;
}

int GET_MSB_DATA(unsigned char *buff, unsigned int index, unsigned int byte_digit)
{
	int i;
	int j;
	int result = 0;

	if (((byte_digit % 2) != 0) || byte_digit <= 1)
	{
		printf("This is not even array data, Failed\r\n");
		return 0;
	}

	j = byte_digit - 1;
	for(i = 0 ; i < byte_digit; i += 1)
	{
		result = result | (buff [ (index * byte_digit) + i] << (8 * j) );
		j -= 1;
		//printf("%x %x \r\n", buff[ (index * byte_digit) + i], result);
	}

	return result;
}

void SET_LSB_DATA(int data, unsigned char *buff, unsigned int index, unsigned int byte_digit)
{
	buff[index + 0] = data & 0xff;
	buff[index + 1] = (data >> 8) & 0xff;

	if(byte_digit == INTEGER)
	{
		buff[index + 2] = (data >> 16) & 0xff;
		buff[index + 3] = (data >> 24) & 0xff;
	}
}

void SET_MSB_DATA(int data, unsigned char *buff, unsigned int index, unsigned int byte_digit)
{
	if(byte_digit == SHORT)
	{
		buff[index + 0] = (data >> 8) & 0xff;
		buff[index + 1] = (data     ) & 0xff;
	}
	else
	{		
		buff[index + 0] = (data >> 24) & 0xff;
		buff[index + 1] = (data >> 16) & 0xff;
		buff[index + 2] = (data >>  8) & 0xff;
		buff[index + 3] = (data      ) & 0xff;
	}
}

#if defined(TCP_SERVER_ENABLED)
static void TCP_OUTPUT_THREAD(void const * argument)
{
	UNUSED(argument);


#if defined(QUEUE_CORE)
	QUEUE_BUFFER *result;

	while(1)
	{
		result = pop_queue();
		if(result != NULL)
		{
			if(result->data[1] == 0)//Configuration GUI
			{
				tcp_server_write(&(result->data[2]), result->data_size - 2, 0);
			}
			else	//User
			{
				tcp_server_write(&(result->data[2]), result->data_size - 2, 1);
			}
		}
	}
#else
	int i;
	while(1)
	{
		if(0 < tmp_message_size)
		{
			for(i = 0; i  < MAX_CLIENT; i++)
			{
				tcp_server_write(tmp_message, tmp_message_size, i);
			}

			tmp_message_size = 0;
		}
	}

#endif
}

static void Server_Thread(void const * argument)
{
	UNUSED(argument);

	//Debug_printf("  State: Ethernet Initialization ...\n\r");

	/* Create tcp_ip stack thread */
	tcpip_init(NULL, NULL);

	/* Initialize the LwIP stack */
	Netif_Config();

	/* Initialize tcp */
	tcp_server_socket_init();

	/* Notify user about the network interface config */
	User_notification(&gnetif);

	tcp_connected = 1;

	for( ;; )
	{
		/* Delete the Init Thread */
		osThreadTerminate(NULL);
	}
}

/**
 * @brief  Initializes the lwIP stack
 * @param  None
 * @retval None
 */
static void Netif_Config(void) {

	ip_addr_t ipaddr;
	ip_addr_t netmask;
	ip_addr_t gw;

#if defined(USE_DHCP)
	ip_addr_set_zero_ip4(&ipaddr);
	ip_addr_set_zero_ip4(&netmask);
	ip_addr_set_zero_ip4(&gw);
#else

	IP_ADDR4(&ipaddr, IP_ADDR[0], IP_ADDR[1], IP_ADDR[2], IP_ADDR[3]);
	IP_ADDR4(&netmask, NETMASK_ADDR[0], NETMASK_ADDR[1], NETMASK_ADDR[2], NETMASK_ADDR[3]);
	IP_ADDR4(&gw, GW_ADDR[0], GW_ADDR[1], GW_ADDR[2], GW_ADDR[3]);

#endif /* USE_DHCP */

	netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_input);

	/*  Registers the default network interface. */
	netif_set_default(&gnetif);

	if (netif_is_link_up(&gnetif)) {
		/* When the netif is fully configured this function must be called.*/
		netif_set_up(&gnetif);
	} else {
		/* When the netif link is down this function must be called */
		netif_set_down(&gnetif);
	}
}
#endif //#if defined(TCP_SERVER_ENABLED)


void mutex_lock(void)
{
#if defined(MUTEX_ENABLED)
	while(osMutexWait(osMutex, osWaitForever) != osOK);
#endif
}
void mutex_unlock(void)
{
#if defined(MUTEX_ENABLED)
	while(osMutexRelease(osMutex) != osOK);
#endif
}


void delay(unsigned int delay_time)
{
	uint32_t PreviousWakeTime = osKernelSysTick();
	osDelayUntil(&PreviousWakeTime, delay_time);
}


/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follow :
 *            System Clock source            = PLL (HSE)
 *            SYSCLK(Hz)                     = 200000000
 *            HCLK(Hz)                       = 200000000
 *            AHB Prescaler                  = 1
 *            APB1 Prescaler                 = 4
 *            APB2 Prescaler                 = 2
 *            HSE Frequency(Hz)              = 25000000
 *            PLL_M                          = 25
 *            PLL_N                          = 432
 *            PLL_P                          = 2
 *            PLL_Q                          = 9
 *            VDD(V)                         = 3.3
 *            Main regulator output voltage  = Scale1 mode
 *            Flash Latency(WS)              = 7
 * @param  None
 * @retval None
 */
static void SystemClock_Config(void) {
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;

	/* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 25;
	RCC_OscInitStruct.PLL.PLLN = 400;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 8;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/* activate the OverDrive */
	if (HAL_PWREx_EnableOverDrive() != HAL_OK) {
		Error_Handler();
	}

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
	   clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	//HAL_RCC_ClockConfig() call HAL_InitTick that in timebase_tim.c
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
void Error_Handler(void) {
	/* User may add here some code to deal with this error */
	while (1) {
		led_toggle(0);
		led_toggle(1);
		led_toggle(2);
		led_toggle(3);
	}
}

/**
 * @brief  Configure the MPU attributes .
 * @param  None
 * @retval None
 */
static void MPU_Config(void) {
	MPU_Region_InitTypeDef MPU_InitStruct;

	/* Disable the MPU */
	HAL_MPU_Disable();

	/* Configure the MPU as Normal Non Cacheable for Ethernet Buffers in the SRAM2 */
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.BaseAddress = 0x2004C000;
	MPU_InitStruct.Size = MPU_REGION_SIZE_16KB;
	MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
	MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER0;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
	MPU_InitStruct.SubRegionDisable = 0x00;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

	HAL_MPU_ConfigRegion(&MPU_InitStruct);

	/* Configure the MPU as Device for Ethernet Descriptors in the SRAM2 */
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.BaseAddress = 0x2004C000;
	MPU_InitStruct.Size = MPU_REGION_SIZE_256B;
	MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
	MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER1;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
	MPU_InitStruct.SubRegionDisable = 0x00;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

	HAL_MPU_ConfigRegion(&MPU_InitStruct);

	/* Configure the MPU attributes as WT for SDRAM */
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.BaseAddress = 0xC0000000;
	MPU_InitStruct.Size = MPU_REGION_SIZE_8MB;
	MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
	MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER2;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
	MPU_InitStruct.SubRegionDisable = 0x00;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

	HAL_MPU_ConfigRegion(&MPU_InitStruct);

	/* Enable the MPU */
	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/**
 * @brief  CPU L1-Cache enable.
 * @param  None
 * @retval None
 */
static void CPU_CACHE_Enable(void) {
	/* Enable I-Cache */
	SCB_EnableICache();

	/* Enable D-Cache */
	SCB_EnableDCache();
}

#if defined(USE_FULL_ASSERT)

/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
ex: Debug_printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1)
	{
	}
}
#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

//by ricky table,
//#define PI 3.14159265358979323846
int Init_Sin_Cos_Table(unsigned char port_num, double tilt)
{
	const double PI = 3.14159265358979323846;
	int i;
	double Init_Degree = 100.0 + tilt;
	double start_degree = 100.0 / 399.0;
	double table_degree = 0.0;

	for (i = 0; i < 400; i += 1)
	{
		table_degree = Init_Degree - (start_degree * i);

		if(install_mode == LEFT_MODE)
		{
			y_table[port_num][i] = sin((table_degree * PI) / 180);
			x_table[port_num][i] = cos((table_degree * PI) / 180);
		}
		else
		{
			y_table[port_num][i] = cos((table_degree * PI) / 180);
			x_table[port_num][i] = sin((table_degree * PI) / 180);
		}
	}
	return 1;
}

__inline__ static int Get_X_Location(unsigned char port_num, double dist, int index)
{
	return (int)(dist	* x_table[port_num][index]);
}

__inline__ static int Get_Y_Location(unsigned char port_num, double dist, int index)
{
	return (int)(dist	* y_table[port_num][index]);
}

#endif
