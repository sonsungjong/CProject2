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

#if defined(LZR_U92x)
#include "LZR_U92x.h"
#elif defined(MICROSCAN)
#include "microscan.h"
#elif defined(FLATSCAN)
#include "FLATSCAN.h"
#endif

#if defined(LZR_U92x)

/* DEFINES -------------------------------------------------------------------*/

//DEFINES options
#define TCP_SERVER_ENABLED
#define MUTEX_ENABLED
#define QUEUE_CORE
//#define STACK_CORE

#define RELAY_DELAY	3000

/* Private functions ---------------------------------------------------------*/
static void SystemClock_Config(void);
static void MPU_Config(void);
static void CPU_CACHE_Enable(void);

//Thread functions 
static void MAIN_THREAD (void const * argument);


#if defined(TCP_SERVER_ENABLED)
static void Netif_Config(void);
static void Server_Thread(void const * argument);
static void TCP_OUTPUT_THREAD(void const *argument);
#endif


void mutex_lock(void);
void mutex_unlock(void);
#if defined(MUTEX_ENABLED)
static osMutexId osMutex;
#endif

static int CONFIG_SENSOR(unsigned char port_num);
static int Init_Sin_Cos_Table(unsigned char port_num, double tilt);
//static int Get_X_Location(unsigned char port_num, double dist, int index);
//static int Get_Y_Location(unsigned char port_num, double dist, int index);

static void all_relay_off(void);

/* Global variables  ---------------------------------------------------------*/

#if !defined(QUEUE_CORE)
char tmp_message[100];
int tmp_message_size;
#endif

#if defined(TCP_SERVER_ENABLED)
struct netif gnetif; /* network interface structure */
unsigned char tcp_connected = 0;
unsigned char sensor_started = 0;
unsigned char IP_ADDR[4] = {0, };
unsigned char NETMASK_ADDR[4] = {0, };
unsigned char GW_ADDR[4] = {0, };    
unsigned int SERVER_PORT1 = 0;
#endif

struct DETECT_ZONE detect_zone[U92x_CNT][MAX_AREA];
struct RELAY_STATUS relay_status[U92x_CNT][MAX_AREA];

unsigned char AREA_NUMBER[U92x_CNT] = {0, };

double x_table[U92x_CNT][274];
double y_table[U92x_CNT][274];
double SENSOR_TILT[U92x_CNT] = {0, };
unsigned char INSTALLATION_DIRECTION[U92x_CNT] = {0, };
unsigned char MIN_OBJECT_SIZE[U92x_CNT] = {0, };
unsigned char MAX_OBJECT_SIZE[U92x_CNT] = {0, };
unsigned char tcp_cmd[U92x_CNT] = {0, };
unsigned char config_mode[U92x_CNT] = {0, };
unsigned char RELAY_ENABLED[2] = {1,1};


#if defined(LZR_U920)
#define FRAME_DIV 1
#define FRAME_ADD_CNT FRAME_DIV * 67
#else
#define FRAME_DIV 2
#define FRAME_ADD_CNT FRAME_DIV * 17
#endif

#if U92x_CNT == 1
unsigned int MIN_DETECT_TIME[U92x_CNT] = {300 };
unsigned int SIG_HOLD_TIME[U92x_CNT] = {10000 };
#else
unsigned int MIN_DETECT_TIME[U92x_CNT] = {300, 300};
unsigned int SIG_HOLD_TIME[U92x_CNT] = {10000, 10000};
#endif


int main(void) {

	unsigned char *tmp_data;

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

	RS485_1_Init();
	UART_LZR_1_INT_ENABLE();
	RS485_2_Init();
	UART_LZR_2_INT_ENABLE();

	/* Custom code start --------------------------------------------------------*/ 

	printf("\033[2J"); //Clear screen
	printf("\r\n");
	printf("***********************************************************\r\n");

#if defined(LZR_U92x)
#if defined(LZR_U920)	

	printf("*** #      #####  #####      #   #  #####  #####   ###  ***\r\n");
	printf("*** #         #   #   #      #   #  #   #      #  #   # ***\r\n");
	printf("*** #        #    ####  ###  #   #  #####   ###   #   # ***\r\n");
	printf("*** #       #     #  #       #   #      #  #      #   # ***\r\n");
	printf("*** #####  #####  #   #       ###   #####  #####   ###  ***\r\n");
#else
	printf("*** #      #####  #####      #   #  #####  #####   ##   ***\r\n");
	printf("*** #         #   #   #      #   #  #   #      #    #   ***\r\n");
	printf("*** #        #    ####  ###  #   #  #####   ###     #   ***\r\n");
	printf("*** #       #     #  #       #   #      #  #        #   ***\r\n");
	printf("*** #####  #####  #   #       ###   #####  #####   ###  ***\r\n");
#endif
#endif
	printf("***********************************************************\r\n");

	all_relay_off();

	printf("++++++++++++++++++++++++++++++++++++++++++++\r\n");	
	beep_on();

	tmp_data = (unsigned char *)malloc(sizeof(unsigned char) * 3000);

	//Allco memeory for background_data with LZR_U92x
	if(ALLOC_MEMORY() == 0)
	{
		printf("%d Failed to allocate the Memory for sensor buffer\r\n", U92x_CNT);
		Error_Handler();
	}
	else
	{
		printf("%d Success to allocate the Memory for sensor buffer\r\n", U92x_CNT);
	}
	printf("--------------------------------------------\r\n");

#if defined(QUEUE_CORE)
	init_queue(12);
	printf("QUEUE CORE 1.1\n\r");
#endif

#if defined(STACK_CORE)
	init_stack(3);
	printf("STACK CORE 1.0\n\r");
#endif

#if defined(TCP_SERVER_ENABLED)

	eeprom_read(EEPROM_IP_ADDR+0, tmp_data, 1);
	IP_ADDR[0] = tmp_data[0];
	eeprom_read(EEPROM_IP_ADDR+1, tmp_data, 1);
	IP_ADDR[1] = tmp_data[0];
	eeprom_read(EEPROM_IP_ADDR+2, tmp_data, 1);
	IP_ADDR[2] = tmp_data[0];
	eeprom_read(EEPROM_IP_ADDR+3, tmp_data, 1);
	IP_ADDR[3] = tmp_data[0];


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

	eeprom_read(EEPROM_PORT(0), tmp_data, 4);
	SERVER_PORT1 = (tmp_data[0] << 24) | (tmp_data[1] << 16) | (tmp_data[2] << 8) | (tmp_data[3]);
	printf("++++++++++++++++++++++++++++++++++++++++++++\r\n");	
	printf(" 0x%4x >> IP : %3d %3d %3d %3d\r\n", EEPROM_IP_ADDR, IP_ADDR[0], IP_ADDR[1], IP_ADDR[2], IP_ADDR[3]);
	printf(" 0x%4x >> MK : %3d %3d %3d %3d\r\n", EEPROM_NETMASK_ADDR, NETMASK_ADDR[0], NETMASK_ADDR[1],NETMASK_ADDR[2],NETMASK_ADDR[3]);
	printf(" 0x%4x >> GW : %3d %3d %3d %3d\r\n", EEPROM_GW_ADDR, GW_ADDR[0], GW_ADDR[1],GW_ADDR[2],GW_ADDR[3]);
	printf(" 0x%4x >> PT : %d\r\n", EEPROM_PORT(0), SERVER_PORT1);
	printf("	>> MAC: %02x-%02x-%02x-%02x-%02x-%02x\r\n", MAC_ADDR0, MAC_ADDR1, MAC_ADDR2, MAC_ADDR3, MAC_ADDR4, MAC_ADDR5);
	printf("--------------------------------------------\r\n");
#endif


	volatile int i;
	double tilt_h = 0.0;
	double tilt_l = 0.0;

	volatile int idx;

	for(idx = 0; idx < U92x_CNT; idx += 1)
	{	
		//Read sensor status
		printf("++++++++++++++++++++++++++++++++++++++++++++\r\n");

		//Load background data
		printf(" 0x%4x >> Load background    : Start\r\n", EEPROM_BACKGROUND(idx) );
		for(i = 0; i < PLANE_SIZE * 4; i += 1)
		{
			//read 548
			eeprom_read(EEPROM_BACKGROUND(idx) +i , &tmp_data[i], 1);
		}	

		for(i = 0; i < U92x_DATA_CNT; i += 1)
		{
			//Set 274
			SET_BACKGROUND_DATA(
					idx, 	//sensor num
					0, 		//Plane num
					i, 		//index
					GET_LSB_DATA(tmp_data, i, SHORT)
					);
		}

		for(i = 0; i < U92x_DATA_CNT; i += 1)
		{
			//Set 274
			SET_BACKGROUND_DATA(
					idx, 	//sensor num
					1, 		//Plane num
					i, 		//index
					GET_LSB_DATA(tmp_data, (274 * 1) + i, SHORT)
					);
		}

		for(i = 0; i < U92x_DATA_CNT; i += 1)
		{
			//Set 274
			SET_BACKGROUND_DATA(
					idx, 	//sensor num
					2, 		//Plane num
					i, 		//index
					GET_LSB_DATA(tmp_data, (274 * 2) + i, SHORT)
					);
		}


		for(i = 0; i < U92x_DATA_CNT; i += 1)
		{
			//Set 274
			SET_BACKGROUND_DATA(
					idx, 	//sensor num
					3, 		//Plane num
					i, 		//index
					GET_LSB_DATA(tmp_data, (274 * 3) + i, SHORT)
					);
		}

		PRINT_BACKGROUND(idx);
		printf(" 0x%4x >> Load background    : OK !!\r\n", EEPROM_BACKGROUND(idx));


		//Load installation direction
		eeprom_read(EEPROM_DIRECTION(idx), tmp_data, 1);
		INSTALLATION_DIRECTION[idx] = tmp_data[0];
		printf(" 0x%4x >> Sensor(%d) Installation Direction : ", EEPROM_DIRECTION(idx), idx);
		if(INSTALLATION_DIRECTION[idx] == 0)
		{
			printf("LEFT\r\n");
		}
		else
		{
			printf("RIGHT\r\n");
		}


		//Load sensor tilt
		eeprom_read(EEPROM_TILT(idx), tmp_data, 8);
		tilt_h = (double)GET_MSB_DATA(tmp_data, 0, INTEGER);
		tilt_l = (double)GET_MSB_DATA(tmp_data, 1, INTEGER)/1000.0;
		SENSOR_TILT[idx] = tilt_h + tilt_l;
		printf(" 0x%4x >> Sensor(%d) Tilt Values : %0.3f\r\n", EEPROM_TILT(idx), idx, SENSOR_TILT[idx]);
		//Update sin cos table
		Init_Sin_Cos_Table(idx, SENSOR_TILT[idx]);


		//Load min, max object_size
		eeprom_read(EEPROM_SIZE(idx), tmp_data, 2);
		MIN_OBJECT_SIZE[idx] = tmp_data[0];
		MAX_OBJECT_SIZE[idx] = tmp_data[1];
		printf(" 0x%4x >> Sensor(%d) Minimum Object Size : %d\r\n",EEPROM_SIZE(idx), idx, MIN_OBJECT_SIZE[idx]);
		printf(" 0x%4x >> Sensor(%d) Maximum Object Size : %d\r\n",EEPROM_SIZE(idx), idx, MAX_OBJECT_SIZE[idx]);


		//Load setup time for detection
		eeprom_read(EEPROM_MINTIME(idx), tmp_data, 4);
		MIN_DETECT_TIME[idx] = GET_MSB_DATA(tmp_data, 0, INTEGER);
		printf(" 0x%4x >> Sensor(%d) Signal triiger min time : %d\r\n",EEPROM_MINTIME(idx), idx, MIN_DETECT_TIME[idx]);


		//Load hold time for detection
		eeprom_read(EEPROM_HOLDTIME(idx), tmp_data, 4);
		SIG_HOLD_TIME[idx] = GET_MSB_DATA(tmp_data, 0, INTEGER);
		printf(" 0x%4x >> Sensor(%d) Signal hold time : %d\r\n",EEPROM_HOLDTIME(idx), idx, SIG_HOLD_TIME[idx]);


		//Load Relay for detection area
		eeprom_read(EEPROM_RELAY(idx), tmp_data, 4);
		for(i=0; i < MAX_AREA; i += 1)
		{
			relay_status[idx][i].relay_assign_num = tmp_data[i]%4;
			printf(" 0x%4x >> Sensor(%d) Area(%d) : Relay_num(%d)\r\n",
					EEPROM_RELAY(idx), idx, i, relay_status[idx][i].relay_assign_num);
		}


		//Load Relay enabled
		eeprom_read(EEPROM_RELAY_EN + idx, tmp_data, 1);
		RELAY_ENABLED[idx] = tmp_data[0];
		printf(" 0x%4x >> Sensor(%d) Relay Enabled : %d \r\n",EEPROM_RELAY_EN, idx, RELAY_ENABLED[idx]);

		//Load detectzone
		for(i=0; i < MAX_AREA; i += 1)
		{
			eeprom_read(EEPROM_DETECTZONE(idx, i), tmp_data, 16);
			detect_zone[idx][i].x1 = GET_MSB_DATA(tmp_data, 0, INTEGER);
			detect_zone[idx][i].y1 = GET_MSB_DATA(tmp_data, 1, INTEGER);
			detect_zone[idx][i].x2 = GET_MSB_DATA(tmp_data, 2, INTEGER);
			detect_zone[idx][i].y2 = GET_MSB_DATA(tmp_data, 3, INTEGER);
			//printf("0x%x \r\n",EEPROM_DETECTZONE(idx, i));
			printf(" 0x%4x >> Detection Zone(%d : %d)\r\n",
					EEPROM_DETECTZONE(idx, i),
					idx,
					i
				  );

			printf("	>> ( x1 = %5d : y1 = %5d )\r\n",
					detect_zone[idx][i].x1,
					detect_zone[idx][i].y1
				  );
			printf("	>> ( x2 = %5d : y2 = %5d )\r\n",
					detect_zone[idx][i].x2,
					detect_zone[idx][i].y2
				  );	
		}


		//Read sensor status
		printf("--------------------------------------------\r\n");
	} // End of	for(idx = 0; idx < U92x_CNT; idx += 1)

	free(tmp_data);
	beep_off();

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

		static const unsigned char port_num_1 = LZR_1;
		osThreadDef(LZR1, MAIN_THREAD, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 10);
		if(osThreadCreate(osThread(LZR1), (void *)&port_num_1) != NULL)
		{
			printf("MAIN_THREAD 1\t\t: OK\r\n");
		}
		else
		{
			Error_Handler();
		}

	}

	/* Start scheduler */
	osKernelStart();
	printf("--------------------------------------------\r\n");

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


static void MAIN_THREAD (void const * argument)
{
	UNUSED(argument);

	volatile unsigned char port_num = 0;
	volatile int i;

#if defined(LZR_U920)
	unsigned char *plane_data[4];
	plane_data[0] = (unsigned char *) malloc( PLANE_SIZE * sizeof(unsigned char));
	plane_data[1] = (unsigned char *) malloc( PLANE_SIZE * sizeof(unsigned char));
	plane_data[2] = (unsigned char *) malloc( PLANE_SIZE * sizeof(unsigned char));
	plane_data[3] = (unsigned char *) malloc( PLANE_SIZE * sizeof(unsigned char));
#else
	unsigned char *plane_data[1];
	plane_data[0]= (unsigned char *) malloc( PLANE_SIZE * sizeof(unsigned char));
#endif

	unsigned char FRAME_CNT_DATA[ID_FRAME_SIZE] = {0, };
	unsigned char INFORMATION_DATA[INFORMATION_SIZE] = {0, };
	unsigned short packet_length;

	unsigned char *return_buffer = (unsigned char *)malloc(sizeof(unsigned char) * QUEUE_MAX_DATA_SIZE);

	int share, remainder;
	char d1, d2 = 0;

	unsigned int internal_frame_cnt[U92x_CNT] = {0, };

	//int relay_num = 0;


	for( port_num = 0; port_num < U92x_CNT; port_num += 1)
	{	
		//Set flag for sensor configuration
		SET_PLANE_NUM_ENABLED(port_num, 1);
		SET_FRAME_COUNTER_ENABLED(port_num, 1);
		SET_INFORMATION_ENABLED(port_num, 0);

		SET_CONFIG_FIELD(port_num, CONFIG_PLAN_NUMBER_OnOff, 1);
		SET_CONFIG_FIELD(port_num, CONFIG_CAN_ID_FRAME_COUNTER, 1);
		SET_CONFIG_FIELD(port_num, CONFIG_LZR_INFORMATION, 0);
	}

	while(1)
	{
		// IP reset part from USER1 switch
		d1 = read_sw(0);

		if((d2 ==0 ) && (d1 == 1))
		{
			return_buffer[0] = 192;
			return_buffer[1] = 168;
			return_buffer[2] = 1;
			return_buffer[3] = MAC_ADDR5;

			eeprom_write(EEPROM_IP_ADDR, return_buffer, 4);

			return_buffer[0] = 255;
			return_buffer[1] = 255;
			return_buffer[2] = 255;
			return_buffer[3] = 0;
			eeprom_write(EEPROM_NETMASK_ADDR, return_buffer, 4);

			return_buffer[0] = 192;
			return_buffer[1] = 168;
			return_buffer[2] = 1;
			return_buffer[3] = 1;
			eeprom_write(EEPROM_GW_ADDR, return_buffer, 4);


			return_buffer[0] = 0;
			return_buffer[1] = 0;
			return_buffer[2] = 0x13;
			return_buffer[3] = 0x88;
			eeprom_write(EEPROM_PORT(0), return_buffer, 4);

			printf("SET default IP, SYSTEM will be resetting\r\n");

			return_buffer[0] = 0;
			return_buffer[1] = 0;
			mutex_push_queue(return_buffer, 2);

			delay(500);

			NVIC_SystemReset();

		}//End of if((d2 ==0 ) && (d1 == 1))

		d2 = d1;


		for( port_num = 0; port_num < U92x_CNT; port_num += 1)
		{

#if defined(LZR_U920)
			packet_length = GET_RX_BUFFER_DISTANCE_DATA(port_num, plane_data[0], plane_data[1], plane_data[2],plane_data[3], FRAME_CNT_DATA, INFORMATION_DATA);
#else
			packet_length = GET_RX_BUFFER_DISTANCE_DATA(port_num, plane_data[0], NULL, NULL, NULL, FRAME_CNT_DATA, INFORMATION_DATA);
#endif

			//Rx data valid part
			if(packet_length != 0)
			{

				//printf("%d \r\n", packet_length);

				if(port_num == LZR_1)
				{
					led_on(0);
				}
				else
				{
					led_on(2);
				}

				internal_frame_cnt[port_num] += 1;

				if(config_mode[port_num] == 1)
				{
#if defined(LZR_U920)
					if((internal_frame_cnt[port_num] % 1) == 0)
					{
						return_buffer[0] = 0x53;
						return_buffer[1] = 0x52;
						return_buffer[2] = port_num;
						return_buffer[3] = ACK_CONFIGMODE;

						memcpy( (return_buffer+4), plane_data[0], 548 );
						mutex_push_queue(return_buffer, 552);

						memcpy( (return_buffer+0), plane_data[1], 548 );
						mutex_push_queue(return_buffer, 548);

						memcpy( (return_buffer+0), plane_data[2], 548 );
						mutex_push_queue(return_buffer, 548);

						memcpy( (return_buffer+0), plane_data[3], 548 );
						mutex_push_queue(return_buffer, 548);
					}
#else
					if((internal_frame_cnt[port_num] % 4) == 0)
					{
						return_buffer[0] = 0x53;
						return_buffer[1] = 0x52;
						return_buffer[2] = port_num;
						return_buffer[3] = ACK_CONFIGMODE;

						memcpy( (return_buffer+4), plane_data[0], 548 );
						mutex_push_queue(return_buffer, 552);
					}
#endif
				}
				else //Use mode
				{
#if defined(LZR_U920)
					//15 * 5 seconds = 75 frame
					if((internal_frame_cnt[port_num] % 75) == 0)
#else
						//60 * 5 seconds = 300 frame
						if((internal_frame_cnt[port_num] % 300) == 0)
#endif
						{
							return_buffer[0] = 0x53;
							return_buffer[1] = 0x52;
							return_buffer[2] = port_num;
							return_buffer[3] = HEART_BEAT;
							return_buffer[4] = 0x00;

							mutex_push_queue(return_buffer, 5);

							if(port_num == LZR_1)
							{
								led_toggle(1);
							}
							else
							{
								led_toggle(3);
							}
							//printf("%d Heart beat %d\r\n",port_num, internal_frame_cnt);
						}
				}
				
			}//End of Rx data valid part
			else
			{
				//delay(1);
				;
			}

			switch(tcp_cmd[port_num])
			{
				case 0:
					break;

				case CMD_USEMODE:
					config_mode[port_num] = 0;
					tcp_cmd[port_num] = 0;
					//printf("%d Use mode\r\n", port_num);
					break;

				case CMD_CONFIGMODE:
					config_mode[port_num] = 1;
					tcp_cmd[port_num] = 0;
					//printf("%d Config mode\r\n", port_num);
					break;

				case CMD_LEFTMODE:
					INSTALLATION_DIRECTION[port_num] = 0;
					eeprom_write(EEPROM_DIRECTION(port_num), &INSTALLATION_DIRECTION[port_num], 1);
					return_buffer[0] = 0x53;
					return_buffer[1] = 0x52;
					return_buffer[2] = port_num;
					return_buffer[3] = ACK_LEFTMODE;
					mutex_push_queue(return_buffer, 4);
					tcp_cmd[port_num] = 0;
					//printf("ACK %d SET LEFTMODE\r\n", port_num);
					break;	

				case CMD_RIGHTMODE:
					INSTALLATION_DIRECTION[port_num] = 1;
					eeprom_write(EEPROM_DIRECTION(port_num), &INSTALLATION_DIRECTION[port_num], 1);
					return_buffer[0] = 0x53;
					return_buffer[1] = 0x52;
					return_buffer[2] = port_num;
					return_buffer[3] = ACK_RIGHTMODE;
					mutex_push_queue(return_buffer, 4);
					tcp_cmd[port_num] = 0;
					//printf("ACK %d SET RIGHTMODE\r\n", port_num);
					break;

				case CMD_SETTILT:

					Init_Sin_Cos_Table(port_num, SENSOR_TILT[port_num]);

					share = (int)SENSOR_TILT[port_num];
					remainder = (SENSOR_TILT[port_num] - (double)share) * 1000.0;

					SET_MSB_DATA(share,     return_buffer, 0, INTEGER);
					SET_MSB_DATA(remainder, return_buffer, 4, INTEGER);

					eeprom_write(EEPROM_TILT(port_num), return_buffer , 8);

					return_buffer[0] = 0x53;
					return_buffer[1] = 0x52;
					return_buffer[2] = port_num;
					return_buffer[3] = ACK_SETTILT;

					mutex_push_queue(return_buffer, 4);
					tcp_cmd[port_num] = 0;

					//printf("ACK %d SET TILT %d.%d\r\n", port_num, share, remainder);
					break;

				case CMD_LOADTILT:

					eeprom_read(EEPROM_TILT(port_num), return_buffer, 8);
					share 		= GET_MSB_DATA(return_buffer, 0, INTEGER);
					remainder 	= GET_MSB_DATA(return_buffer, 1, INTEGER);

					return_buffer[0] = 0x53;
					return_buffer[1] = 0x52;
					return_buffer[2] = port_num;
					return_buffer[3] = ACK_LOADTILT;

					SET_MSB_DATA(share,     return_buffer, 4, INTEGER);
					SET_MSB_DATA(remainder, return_buffer, 8, INTEGER);

					mutex_push_queue(return_buffer, 12);
					tcp_cmd[port_num] = 0;
					//printf("ACK %d GET TILT %d.%d\r\n", port_num, share, remainder);
					break;

				case CMD_SETBGD:
					SET_BACKGROUND_DATA_FLAG(port_num);
					//TEACH_IN(port_num, 10, 200);
					TEACH_IN(port_num, 10, 0);

					//Plane 0
					for(i = 0; i < 274; i += 1)
					{
						SET_LSB_DATA(GET_BACKGROUND_DATA(port_num, 0, i), return_buffer, i<<1, SHORT);
					}

					for(i = 0; i < 548; i += 1)
					{
						eeprom_write(EEPROM_BACKGROUND(port_num) + i, return_buffer+i, 1);
					}


					//Plane 1
					for(i = 0; i < 274; i += 1)
					{
						SET_LSB_DATA(GET_BACKGROUND_DATA(port_num, 1, i), return_buffer, i<<1, SHORT);
					}

					for(i = 0; i < 548; i += 1)
					{
						eeprom_write(EEPROM_BACKGROUND(port_num) + (548*1) + i, return_buffer+i, 1);
					}

					//Plane 2
					for(i = 0; i < 274; i += 1)
					{
						SET_LSB_DATA(GET_BACKGROUND_DATA(port_num, 2, i), return_buffer, i<<1, SHORT);
					}

					for(i = 0; i < 548; i += 1)
					{
						eeprom_write(EEPROM_BACKGROUND(port_num) + (548*2) + i, return_buffer+i, 1);
					}

					//Plane 3
					for(i = 0; i < 274; i += 1)
					{
						SET_LSB_DATA(GET_BACKGROUND_DATA(port_num, 3, i), return_buffer, i<<1, SHORT);
					}

					for(i = 0; i < 548; i += 1)
					{
						eeprom_write(EEPROM_BACKGROUND(port_num) + (548*3) + i, return_buffer+i, 1);
					}



					return_buffer[0] = 0x53;
					return_buffer[1] = 0x52;
					return_buffer[2] = port_num;
					return_buffer[3] = ACK_SETBGD;

					mutex_push_queue(return_buffer, 4);
					tcp_cmd[port_num] = 0;
					//printf("ACK %d SET_BACKGROUND\r\n", port_num);
					break;

				case CMD_LOADBGD:

					return_buffer[0] = 0x53;
					return_buffer[1] = 0x52;
					return_buffer[2] = port_num;
					return_buffer[3] = ACK_LOADBGD;

					//Plane 0
					for(i = 0; i < 548; i += 1)
					{
						eeprom_read(EEPROM_BACKGROUND(port_num) +i, (return_buffer+4)+i, 1);
					}
					mutex_push_queue(return_buffer, 552);

					//Plane 1
					for(i = 0; i < 548; i += 1)
					{
						eeprom_read(EEPROM_BACKGROUND(port_num) + (548*1) +i, (return_buffer)+i, 1);
					}
					mutex_push_queue(return_buffer, 548);

					//Plane 2
					for(i = 0; i < 548; i += 1)
					{
						eeprom_read(EEPROM_BACKGROUND(port_num) + (548*2) +i, (return_buffer)+i, 1);
					}
					mutex_push_queue(return_buffer, 548);

					//Plane 3
					for(i = 0; i < 548; i += 1)
					{
						eeprom_read(EEPROM_BACKGROUND(port_num) + (548*3) +i, (return_buffer)+i, 1);
					}
					mutex_push_queue(return_buffer, 548);


					tcp_cmd[port_num] = 0;

					//printf("ACK %d GET_BACKGROUND\r\n", port_num);
					break;

				case CMD_SETZONE:

					SET_MSB_DATA(detect_zone[port_num][AREA_NUMBER[port_num]].x1, return_buffer,  0, INTEGER);
					SET_MSB_DATA(detect_zone[port_num][AREA_NUMBER[port_num]].y1, return_buffer,  4, INTEGER);
					SET_MSB_DATA(detect_zone[port_num][AREA_NUMBER[port_num]].x2, return_buffer,  8, INTEGER);
					SET_MSB_DATA(detect_zone[port_num][AREA_NUMBER[port_num]].y2, return_buffer, 12, INTEGER);

					eeprom_write(EEPROM_DETECTZONE(port_num, AREA_NUMBER[port_num]), return_buffer, 16);

					return_buffer[0] = 0x53;
					return_buffer[1] = 0x52;
					return_buffer[2] = port_num;
					return_buffer[3] = ACK_SETZONE;

					mutex_push_queue(return_buffer, 4);
					tcp_cmd[port_num] = 0;

					//printf("ACK %d %d SET_DETECTZONE (%d, %d, %d, %d)\r\n", 
					//		port_num,
					//		AREA_NUMBER,
					//		detect_zone[port_num][AREA_NUMBER].x1,
					//		detect_zone[port_num][AREA_NUMBER].y1,
					//		detect_zone[port_num][AREA_NUMBER].x2,
					//		detect_zone[port_num][AREA_NUMBER].y2
					//	  );
					break;

				case CMD_LOADZONE:

					return_buffer[0] = 0x53;
					return_buffer[1] = 0x52;
					return_buffer[2] = port_num;
					return_buffer[3] = ACK_LOADZONE;

					for(i = 0; i < MAX_AREA; i += 1)
					{
						eeprom_read(EEPROM_DETECTZONE(port_num, i), return_buffer+4 + (i*16), 16);

						detect_zone[port_num][i].x1 = GET_MSB_DATA(return_buffer, (i*4)+1, INTEGER);
						detect_zone[port_num][i].y1 = GET_MSB_DATA(return_buffer, (i*4)+2, INTEGER);
						detect_zone[port_num][i].x2 = GET_MSB_DATA(return_buffer, (i*4)+3, INTEGER);
						detect_zone[port_num][i].y2 = GET_MSB_DATA(return_buffer, (i*4)+4, INTEGER);

						//printf("ACK %d %d GET_DETECTZONE (%d, %d, %d, %d)\r\n", 
						//		port_num,
						//		i,
						//		detect_zone[port_num][i].x1,
						//		detect_zone[port_num][i].y1,
						//		detect_zone[port_num][i].x2,
						//		detect_zone[port_num][i].y2
						//		);

					}

					mutex_push_queue(return_buffer, 68);
					tcp_cmd[port_num] = 0;

					break;

				case CMD_SETSIZE:

					eeprom_write(EEPROM_SIZE(port_num)+0, &MIN_OBJECT_SIZE[port_num], 1);	
					eeprom_write(EEPROM_SIZE(port_num)+1, &MAX_OBJECT_SIZE[port_num], 1);	

					return_buffer[0] = 0x53;
					return_buffer[1] = 0x52;
					return_buffer[2] = port_num;
					return_buffer[3] = ACK_SETSIZE;

					mutex_push_queue(return_buffer, 4);
					tcp_cmd[port_num] = 0;

					//printf("ACK %d SET MINIMUM OBJECT SIZE %d \r\n", port_num, MIN_OBJECT_SIZE[port_num]);
					//printf("ACK %d SET MAXIMUM OBJECT SIZE %d \r\n", port_num, MAX_OBJECT_SIZE[port_num]);

					break;

				case CMD_LOADSIZE:	
					//eeprom_read(EEPROM_SIZE(port_num), return_buffer, 1);
					eeprom_read(EEPROM_SIZE(port_num), return_buffer, 2);
					MIN_OBJECT_SIZE[port_num] = return_buffer[0];
					MAX_OBJECT_SIZE[port_num] = return_buffer[1];

					return_buffer[0] = 0x53;
					return_buffer[1] = 0x52;
					return_buffer[2] = port_num;
					return_buffer[3] = ACK_LOADSIZE;
					return_buffer[4] = MIN_OBJECT_SIZE[port_num];
					return_buffer[5] = MAX_OBJECT_SIZE[port_num];

					mutex_push_queue(return_buffer, 6);
					tcp_cmd[port_num] = 0;

					//printf("ACK %d GET MINIMUM OBJECT SIZE %d \r\n", port_num, MIN_OBJECT_SIZE[port_num]);
					//printf("ACK %d GET MAXIMUM OBJECT SIZE %d \r\n", port_num, MAX_OBJECT_SIZE[port_num]);

					break;

				case CMD_SETIP:

					return_buffer[0] = 0x53;
					return_buffer[1] = 0x52;
					return_buffer[2] = port_num;
					return_buffer[3] = ACK_SETIP;

					tcp_cmd[port_num] = 0;

					eeprom_write(EEPROM_IP_ADDR, IP_ADDR, 4);
					eeprom_write(EEPROM_NETMASK_ADDR, NETMASK_ADDR, 4);
					eeprom_write(EEPROM_GW_ADDR, GW_ADDR, 4);

					mutex_push_queue(return_buffer, 4);

					//printf("%3d.%3d.%3d.%3d\r\n", IP_ADDR[0], IP_ADDR[1], IP_ADDR[2], IP_ADDR[3]);
					//printf("%3d.%3d.%3d.%3d\r\n", NETMASK_ADDR[0], NETMASK_ADDR[1], NETMASK_ADDR[2], NETMASK_ADDR[3]);
					//printf("%3d.%3d.%3d.%3d\r\n", GW_ADDR[0], GW_ADDR[1], GW_ADDR[2], GW_ADDR[3]);

					delay(500);

					NVIC_SystemReset();

					break;

				case CMD_IGNOREDETECTION:

					return_buffer[0] = RELAY_ENABLED[port_num];

					eeprom_write(EEPROM_RELAY_EN + port_num, return_buffer, 1);	

					return_buffer[0] = 0x53;
					return_buffer[1] = 0x52;
					return_buffer[2] = port_num;
					return_buffer[3] = ACK_IGNOREDETECTION;
					return_buffer[4] = RELAY_ENABLED[port_num];

					mutex_push_queue(return_buffer, 5);
					tcp_cmd[port_num] = 0;

					//printf("Relay enabled : %d \r\n", RELAY_ENABLED[port_num]);

					break;

				case CMD_REBOOT:
					return_buffer[0] = 0x53;
					return_buffer[1] = 0x52;

					return_buffer[2] = port_num;
					return_buffer[3] = ACK_REBOOT;

					tcp_cmd[port_num] = 0;

					mutex_push_queue(return_buffer, 4);

					delay(500);

					NVIC_SystemReset();
					break;

				case CMD_SETMINTIME	:
					SET_MSB_DATA(MIN_DETECT_TIME[port_num], return_buffer,  0, INTEGER);
					eeprom_write(EEPROM_MINTIME(port_num), return_buffer, 4);

					return_buffer[0] = 0x53;
					return_buffer[1] = 0x52;
					return_buffer[2] = port_num;
					return_buffer[3] = ACK_SETMINTIME;

					mutex_push_queue(return_buffer, 4);
					tcp_cmd[port_num] = 0;

					//printf("ACK %d SET_MINTIME (%d)\r\n", port_num,	MIN_DETECT_TIME[port_num]);

					break;

				case CMD_LOADMINTIME:

					return_buffer[0] = 0x53;
					return_buffer[1] = 0x52;
					return_buffer[2] = port_num;
					return_buffer[3] = ACK_LOADMINTIME;

					eeprom_read(EEPROM_MINTIME(port_num), return_buffer+4, 4);

					MIN_DETECT_TIME[port_num] = GET_MSB_DATA(return_buffer, 1, INTEGER);

					mutex_push_queue(return_buffer, 8);
					tcp_cmd[port_num] = 0;

					//printf("ACK %d GET_MINTIME (%d)\r\n", port_num,	MIN_DETECT_TIME[port_num]);

					break;

				case CMD_SETHOLDTIME:
					SET_MSB_DATA(SIG_HOLD_TIME[port_num], return_buffer,  0, INTEGER);
					eeprom_write(EEPROM_HOLDTIME(port_num), return_buffer, 4);

					return_buffer[0] = 0x53;
					return_buffer[1] = 0x52;
					return_buffer[2] = port_num;
					return_buffer[3] = ACK_SETHOLDTIME;

					mutex_push_queue(return_buffer, 4);
					tcp_cmd[port_num] = 0;

					//printf("ACK %d SET_HOLDTIME (%d)\r\n", port_num, SIG_HOLD_TIME[port_num]);

					break;


				case CMD_LOADHOLDTIME:
					return_buffer[0] = 0x53;
					return_buffer[1] = 0x52;
					return_buffer[2] = port_num;
					return_buffer[3] = ACK_LOADHOLDTIME;

					eeprom_read(EEPROM_HOLDTIME(port_num), return_buffer+4, 4);

					SIG_HOLD_TIME[port_num] = GET_MSB_DATA(return_buffer, 1, INTEGER);

					mutex_push_queue(return_buffer, 8);
					tcp_cmd[port_num] = 0;

					//printf("ACK %d GET_HOLDTIME (%d)\r\n", port_num, SIG_HOLD_TIME[port_num]);

					break;

				case CMD_SETRELAY:

					return_buffer[0] = relay_status[port_num][0].relay_assign_num;
					return_buffer[1] = relay_status[port_num][1].relay_assign_num;
					return_buffer[2] = relay_status[port_num][2].relay_assign_num;
					return_buffer[3] = relay_status[port_num][3].relay_assign_num;

					eeprom_write(EEPROM_RELAY(port_num), return_buffer, 4);

					return_buffer[0] = 0x53;
					return_buffer[1] = 0x52;
					return_buffer[2] = port_num;
					return_buffer[3] = ACK_SETRELAY;

					mutex_push_queue(return_buffer, 4);
					tcp_cmd[port_num] = 0;

					//printf("ACK %d SET_RELAY (%d)\r\n", port_num);
					break;

				case CMD_LOADRELAY:

					return_buffer[0] = 0x53;
					return_buffer[1] = 0x52;
					return_buffer[2] = port_num;
					return_buffer[3] = ACK_LOADRELAY;

					eeprom_read(EEPROM_RELAY(port_num), return_buffer+4, 4);

					mutex_push_queue(return_buffer, 8);
					tcp_cmd[port_num] = 0;

					//printf("ACK %d GET_RELAY (%d)\r\n", port_num);
					break;


				default :
					//printf("Wrong command 0x%x %d\r\n", tcp_cmd[port_num], port_num);
					tcp_cmd[port_num] = 0;
					break;

			} // End of switch(tcp_cmd[port_num])

		}// End of for( port_num = 0; port_num < U92x_CNT; port_num += 1)

	}//End of while(1)
}

// */


int CONFIG_SENSOR(unsigned char port_num)
{

#if defined(LZR_U92x)

	//For once working for configuring the sensor.
	//if user call the SET_SENSOR_CONFIG_FLAG(port_num) function, this program will call CONFIG_SENSOR function again.

	if(CHECK_SENSOR_CONFIG_FLAG(port_num) == 0)
	{
		return 0;
	}
	printf("Begin Sensor configuration\r\n");

	//1. User need to change sensor mode from measure mode to configuration mode
	printf(">> SET_CONFIGURATION_MODE\r\n");
	SET_CONFIG_SENSOR(port_num, SET_CONFIG_MODE, 0);

	//2. User need to know about sernsor configuration field data
	printf(">> GET SENSOR CONFIG FIELD\r\n");
	SET_CONFIG_SENSOR(port_num, GETRAWDATACONFIG, 0);

	//3. User Options, if user want to set config field, call SET_CONFIG_FIELD() with option value
	printf(">> SET SENSOR CONFIG FIELD\r\n");

	//Example, See config field address part in LZR_U92x.h and protocol doc
	/*Config field address part (refer from getrawdataconfig part)
#define CONFIG_BIT_MASK							0
#define CONFIG_COMMUNICATION_CHARGE				4
#define CONFIG_BAUD_RATE						6
#define CONFIG_LZR_INFORMATION					8
#define CONFIG_RED_LASER_TIMEOUT				9
#define CONFIG_TEST_FRAME						10
#define CONFIG_PLANE_0_OnOff					11
#define CONFIG_PLANE_1_OnOff					12
#define CONFIG_PLANE_2_OnOff					13
#define CONFIG_PLANE_3_OnOff					14
#define CONFIG_PULSE_WIDTH						15
#define CONFIG_NUMBER_OF_DISTANCE_VALUES		16
#define CONFIG_STARTING_SPOT_NUMBER				18
#define CONFIG_JUMP_BETWEEN_2_SUCCESSIVE_SPOTS	20
#define CONFIG_APD_DISTANCE_RANGE				22
#define CONFIG_CAN_ID_FRAME_COUNTER				23
#define CONFIG_DIODE_LIFETIME_MANAGEMENT		24
#define CONFIG_POLARITY_OF_INPUT				25
#define CONFIG_DELAY_OF_HEARTBEAT				26
#define CONFIG_LED_1							27
#define CONFIG_LED_2							28
#define CONFIG_LED_BLUE							29
#define CONFIG_LED_ERROR						30
#define CONFIG_DURATION_OF_LED					31
#define CONFIG_MAXIMUM_DISTANCE_RANGE			32
#define CONFIG_PLAN_NUMBER_OnOff				34
#define	CONFIG_IMMUNITY							35
#define	CONFIG_HOT_RESET_TIMER					36
#define	CONFIG_HOT_RESET_COUNTER				38
*/

	//SET_CONFIG_FIELD(port_num, CONFIG_MAXIMUM_DISTANCE_RANGE, 40000);
	SET_CONFIG_FIELD(port_num, CONFIG_RED_LASER_TIMEOUT, 255);
	SET_CONFIG_SENSOR(port_num, SETRAWDATACONFIG, 0);

	//4. User Options, if user wants to turn on the visable beam, call this function, 
	//   or not want it. User can erase to pass this function.
	//
	printf(">> SET RED LASER ON\r\n");
	SET_CONFIG_SENSOR(port_num, SETRAWDATAREDLASER, 1);

	//5. User must change the sensor mode from configuration mode to measure mode to work for user algorithm
	printf(">> SET MEASURE_MODE\r\n");
	SET_CONFIG_SENSOR(port_num, SETRAWDATAMODE, MEASURE_MODE);
#endif
	printf("End Sensor configuration\r\n");
	return 1;
}


#if defined(TCP_SERVER_ENABLED)

static void TCP_OUTPUT_THREAD(void const * argument)
{
	UNUSED(argument);

	volatile int i;
	//int tcp_result = 0;

#if defined(QUEUE_CORE)
	QUEUE_BUFFER *result;

	while(1)
	{
		result = pop_queue();
		if(result != NULL)
		{
			for(i = 0; i  < MAX_CLIENT; i++)
			{
				tcp_server_write(result->data, result->data_size, i);
				//tcp_result = tcp_server_write(result->data, result->data_size, i);
				//printf("%d\r\n", tcp_result);
			}
		}
	}
#else
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

	printf("  State: Ethernet Initialization ...\n\r");

	//delay(30000); // For suho
	delay(1000); // For test

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
ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1)
	{
	}
}
#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/



//by ricky table,
#define PI 3.14159265358979323846
int Init_Sin_Cos_Table(unsigned char port_num, double tilt)
{
	double Init_Degree = 96.0 + tilt;
	double start_degree = 96.0 / 273.0;
	double table_degree = 0.0;

	for (int i = 0; i < U92x_DATA_CNT; i += 1)
	{
		table_degree = Init_Degree - (start_degree * i);
		y_table[port_num][i] = sin((table_degree * PI) / 180);//sin
		x_table[port_num][i] = cos((table_degree * PI) / 180);//cos
	}
	return 1;
}

//static int Get_X_Location(unsigned char port_num, double dist, int index)
//{
//	return (int)(dist	* x_table[port_num][index]);
//}
//
//static int Get_Y_Location(unsigned char port_num, double dist, int index)
//{
//	return (int)(dist	* y_table[port_num][index]);
//}


static void all_relay_off(void)
{
	relay_off(0);
	relay_off(1);
	relay_off(2);
	relay_off(3);
}

#endif
