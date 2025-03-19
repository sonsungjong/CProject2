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

#if defined(MICROSCAN)

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
#define QUEUE_CORE
//#define STACK_CORE

#define RELAY_DELAY	2000

#define PEOPLE	0
#define VEHICLE	1
#define SAFETY	2
#define NOTHING	3

#define MIN_IGNORE_ZONE	300
//#define MERGE_GAP	150

#define IGNORE_SIZE	200
#define PEOPLE_SIZE	850

//#define IGNORE_SIZE	50 //for test
//#define PEOPLE_SIZE	300 //for test

#define ANGLE_START	0
#define ANGLE_END	10000
#define MAX_POINT_SIZE	MAX_POINT*2

//To detect forklift arm, at least we need 20cm offset from ground
#define BACKGROUND_OFFSET 50
#define FORKLIFTARM_Y_LIMIT_OFFSET (300-BACKGROUND_OFFSET)

#define START_IDX (50-15)
#define END_IDX   (50+15)

/* Private functions ---------------------------------------------------------*/
static void SystemClock_Config(void);
static void MPU_Config(void);
static void CPU_CACHE_Enable(void);

//Thread functions 
static void MAIN_THREAD (void const * argument);
static void ETC_THREAD (void const * argument);
static void FLAT_RELAY_THREAD (void const * argument);
static void V_RELAY_THREAD (void const * argument);
static void P_RELAY_THREAD (void const * argument);


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


// Local functions
static int Init_Sin_Cos_Table(unsigned char port_num, double tilt);

__inline__ static int Get_X_Location(unsigned char port_num, double dist, int index);
__inline__ static int Get_Y_Location(unsigned char port_num, double dist, int index);

//inline function
__inline__ static int detect_function(unsigned char port_num, int facet_num, unsigned char *data, int x1, int y1, int x2, int y2);

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
unsigned char IP_VALID = 0;
unsigned char NETMASK_ADDR[4] = {0, };
unsigned char NETMASK_VALID = 0;
unsigned char GW_ADDR[4] = {0, };
unsigned char GW_VALID = 0;

unsigned int SERVER_PORT1 = 0;
unsigned int SERVER_PORT2 = 0;
#endif

double SENSOR_TILT[uSCAN_CNT];
double x_table[uSCAN_CNT][MAX_POINT];
double y_table[uSCAN_CNT][MAX_POINT];
struct DETECT_ZONE detect_zone[uSCAN_CNT];
unsigned char install_mode[uSCAN_CNT] = {CENTER_MODE, CENTER_MODE};
unsigned char configuration_mode[uSCAN_CNT] = {0, };
unsigned char tcp_cmd[uSCAN_CNT] = {0, };
unsigned char error_check[uSCAN_CNT] = {0, };

unsigned char relay_flag[uSCAN_CNT][3] = {0, };

//unsigned char p_flag[uSCAN_CNT] = {0, };

unsigned char flatscan_relay_status = 0;
//unsigned char enter_flag = 0;
//unsigned char exit_flag = 0;

int FORKLIFTARM_Y_LIMIT[uSCAN_CNT][4] = {0, };

enum CASE_STEP {
	v_Step0 = 0,
	v_Step1,
	v_Step2,
	v_Step3,
	v_Step4,

	p_Step0,
	p_Step1,
	p_Step2,
	p_Step3,

	f_Step
};

void print_case(unsigned char port_num, enum CASE_STEP step)
{

	mutex_lock();
	if(port_num == uSCAN_2)
	{
		printf("\t");
	}
	switch(step)
	{
		case v_Step0:
			printf("%d => v_Step0\r\n", port_num);
			break;
		case v_Step1:
			printf("%d => v_Step1\r\n", port_num);
			break;
		case v_Step2:
			printf("%d => v_Step2\r\n", port_num);
			break;
		case v_Step3:
			printf("%d => v_Step3\r\n", port_num);
			break;
		case v_Step4:
			printf("%d => v_Step4\r\n", port_num);
			break;

		case p_Step0:
			printf("%d => p_Step0\r\n", port_num);
			break;
		case p_Step1:
			printf("%d => p_Step1\r\n", port_num);
			break;
		case p_Step2:
			printf("%d => p_Step2\r\n", port_num);
			break;
		case p_Step3:
			printf("%d => p_Step3\r\n", port_num);
			break;

		case f_Step:
			printf("%d => f_Step\r\n", port_num);
			break;
		default:
			break;
	}
	mutex_unlock();
}

int main(void) {

	unsigned short min_mdi = 0xffff;

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
	printf("********************************************************\r\n");
	printf("*** #   # #####  ### ###   ###  ####  ##  ###  #   # ***\r\n");
	printf("*** ## ##   #   #    #  # #   # #    #   #   # ##  # ***\r\n");
	printf("*** # # #   #   #    ###  #   # #### #   ##### # # # ***\r\n");
	printf("*** #   #   #   #    # #  #   #    # #   #   # #  ## ***\r\n");
	printf("*** #   # #####  ### #  #  ###  ####  ## #   # #   # ***\r\n");
	printf("********************************************************\r\n");

	//Alloc memeory for background_data with Flatscan

	printf("++++++++++++++++++++++++++++++++++++++++++++\r\n");
	if(ALLOC_MEMORY() == 0)
	{
		Error_Handler();
	}
	printf("--------------------------------------------\r\n");

#if defined(QUEUE_CORE)
	init_queue(16);
	printf("QUEUE CORE 1.1\n\r");
#endif

#if defined(STACK_CORE)
	init_stack(3);
	printf("STACK CORE 1.0\n\r");
#endif

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

	eeprom_read(EEPROM_PORT(0), tmp_data, 4);
	SERVER_PORT1 = (tmp_data[0] << 24) | (tmp_data[1] << 16) | (tmp_data[2] << 8) | (tmp_data[3]);

	eeprom_read(EEPROM_PORT(1), tmp_data, 4);
	SERVER_PORT2 = (tmp_data[0] << 24) | (tmp_data[1] << 16) | (tmp_data[2] << 8) | (tmp_data[3]);

	free(tmp_data);

	printf("============================================\r\n");
	printf("|| IP  : %3d %3d %3d %3d\r\n", IP_ADDR[0], IP_ADDR[1], IP_ADDR[2], IP_ADDR[3]);
	printf("|| MK  : %3d %3d %3d %3d\r\n", NETMASK_ADDR[0], NETMASK_ADDR[1],NETMASK_ADDR[2],NETMASK_ADDR[3]);
	printf("|| GW  : %3d %3d %3d %3d\r\n", GW_ADDR[0], GW_ADDR[1],GW_ADDR[2],GW_ADDR[3]);
	printf("|| PT1 : %d\r\n", SERVER_PORT1);
	printf("|| PT2 : %d\r\n", SERVER_PORT2);
	printf("|| MAC : %02x-%02x-%02x-%02x-%02x-%02x\r\n", MAC_ADDR0, MAC_ADDR1, MAC_ADDR2, MAC_ADDR3, MAC_ADDR4, MAC_ADDR5);
	printf("============================================\r\n");
#endif

	//Read sensor status
	printf("++++++++++++++++++++++++++++++++++++++++++++\r\n");

	int i,j;
	double tilt_h = 0.0;
	double tilt_l = 0.0;

	unsigned char *tmp = (unsigned char *)malloc(sizeof(unsigned char)*800);

	eeprom_read(EEPROM_MODE(uSCAN_1), tmp, 1);
	install_mode[uSCAN_1] = tmp[0];
	printf(" >>> Sensor(%d) INSTALL MODE : %d\r\n",uSCAN_1, install_mode[uSCAN_1]);

	eeprom_read (EEPROM_DEGREE(uSCAN_1), tmp, 8);
	tilt_h = (double)GET_MSB_DATA(tmp, 0, INTEGER);
	tilt_l = (double)GET_MSB_DATA(tmp, 1, INTEGER)/1000.0;
	SENSOR_TILT[uSCAN_1] = tilt_h + tilt_l;
	printf(" >>> Sensor(%d) tilt status : %0.3f\r\n",uSCAN_1, SENSOR_TILT[uSCAN_1]);
	Init_Sin_Cos_Table(uSCAN_1, SENSOR_TILT[uSCAN_1]);

	//This value have to save in cm unit
	eeprom_read(EEPROM_DETECTZONE_ADDR(uSCAN_1), tmp, 16);
	detect_zone[uSCAN_1].x1 = GET_MSB_DATA(tmp, 0, INTEGER);
	detect_zone[uSCAN_1].y1 = GET_MSB_DATA(tmp, 1, INTEGER);
	detect_zone[uSCAN_1].x2 = GET_MSB_DATA(tmp, 2, INTEGER);
	detect_zone[uSCAN_1].y2 = GET_MSB_DATA(tmp, 3, INTEGER);

	printf(" >>> Detection Zone(%d)     : \r\n ( x1 = %5d : y1 = %5d )\r\n ( x2 = %5d : y2 = %5d ) \r\n",
			uSCAN_1,
			detect_zone[uSCAN_1].x1,
			detect_zone[uSCAN_1].y1,
			detect_zone[uSCAN_1].x2,
			detect_zone[uSCAN_1].y2
		  );	


	eeprom_read(EEPROM_MODE(uSCAN_2), tmp, 1);
	install_mode[uSCAN_2] = tmp[0];
	printf(" >>> Sensor(%d) INSTALL MODE : %d\r\n", uSCAN_2, install_mode[uSCAN_2]);


	eeprom_read (EEPROM_DEGREE(uSCAN_2), tmp, 8);
	tilt_h = (double)GET_MSB_DATA(tmp, 0, INTEGER);
	tilt_l = (double)GET_MSB_DATA(tmp, 1, INTEGER)/1000.0;
	SENSOR_TILT[uSCAN_2] = tilt_h + tilt_l;
	printf(" >>> Sensor(%d) tilt status : %0.3f\r\n", uSCAN_2, SENSOR_TILT[uSCAN_2]);
	Init_Sin_Cos_Table(uSCAN_2, SENSOR_TILT[uSCAN_2]);

	eeprom_read(EEPROM_DETECTZONE_ADDR(uSCAN_2), tmp, 16);
	detect_zone[uSCAN_2].x1 = GET_MSB_DATA(tmp, 0, INTEGER);
	detect_zone[uSCAN_2].y1 = GET_MSB_DATA(tmp, 1, INTEGER);
	detect_zone[uSCAN_2].x2 = GET_MSB_DATA(tmp, 2, INTEGER);
	detect_zone[uSCAN_2].y2 = GET_MSB_DATA(tmp, 3, INTEGER);

	printf(" >>> Detection Zone(%d)     : \r\n ( x1 = %5d : y1 = %5d )\r\n ( x2 = %5d : y2 = %5d ) \r\n",
			uSCAN_2,
			detect_zone[uSCAN_2].x1,
			detect_zone[uSCAN_2].y1,
			detect_zone[uSCAN_2].x2,
			detect_zone[uSCAN_2].y2
		  );
	printf("--------------------------------------------\r\n");

	printf("++++++++++++++++++++++++++++++++++++++++++++\r\n");

	for(j =0; j < 4; j += 1)
	{
		for(i = 0; i < MAX_POINT_SIZE; i += 1)
		{
			//read 800
			eeprom_read(
					EEPROM_BACKGROUND(uSCAN_1) + (j * 200) +i , 
					&tmp[ (j*200) + i], 
					1
					);
		}	

		for(i = 0; i < MAX_POINT; i += 1)
		{
			//read 400
			SET_BACKGROUND_DATA(
					uSCAN_1, //port num
					j, 		//facet_num
					i, 		//index
					GET_LSB_DATA(tmp, ( j * 100) + i, SHORT)
					);
		}
	}

	min_mdi = 0xffff;
	for(i = START_IDX; i < END_IDX; i += 1)
	{
		if( GET_BACKGROUND_DATA(uSCAN_1, FACET1, i) < min_mdi)
		{
			min_mdi = GET_BACKGROUND_DATA(uSCAN_1, FACET1, i);
			FORKLIFTARM_Y_LIMIT[uSCAN_1][0] = Get_Y_Location(uSCAN_1, min_mdi, i);
		}
	}

	min_mdi = 0xffff;
	for(i = START_IDX; i < END_IDX; i += 1)
	{
		if( GET_BACKGROUND_DATA(uSCAN_1, FACET2, i) < min_mdi)
		{
			min_mdi = GET_BACKGROUND_DATA(uSCAN_1, FACET2, i);
			FORKLIFTARM_Y_LIMIT[uSCAN_1][1] = Get_Y_Location(uSCAN_1, min_mdi, i);
		}
	}

	min_mdi = 0xffff;
	for(i = START_IDX; i < END_IDX; i += 1)
	{
		if( GET_BACKGROUND_DATA(uSCAN_1, FACET3, i) < min_mdi)
		{
			min_mdi = GET_BACKGROUND_DATA(uSCAN_1, FACET3, i);
			FORKLIFTARM_Y_LIMIT[uSCAN_1][2] = Get_Y_Location(uSCAN_1, min_mdi, i);
		}
	}

	min_mdi = 0xffff;
	for(i = START_IDX; i < END_IDX; i += 1)
	{
		if( GET_BACKGROUND_DATA(uSCAN_1, FACET4, i) < min_mdi)
		{
			min_mdi = GET_BACKGROUND_DATA(uSCAN_1, FACET4, i);
			FORKLIFTARM_Y_LIMIT[uSCAN_1][3] = Get_Y_Location(uSCAN_1, min_mdi, i);
		}
	}

	for(j =0; j < 4; j += 1)
	{
		for(i = 0; i < MAX_POINT_SIZE; i += 1)
		{
			eeprom_read(
					EEPROM_BACKGROUND(uSCAN_2) + (j*200) + i, 
					&tmp[(j*200) + i], 
					1
					);
		}	

		for(i = 0; i < MAX_POINT; i += 1)
		{
			SET_BACKGROUND_DATA(
					uSCAN_2, 
					j, 
					i, 
					GET_LSB_DATA(tmp, ( j * 100) + i, SHORT)
					);
		}
	}

	min_mdi = 0xffff;
	for(i = START_IDX; i < END_IDX; i += 1)
	{
		if( GET_BACKGROUND_DATA(uSCAN_2, FACET1, i) < min_mdi)
		{
			min_mdi = GET_BACKGROUND_DATA(uSCAN_2, FACET1, i);
			FORKLIFTARM_Y_LIMIT[uSCAN_2][0] = Get_Y_Location(uSCAN_2, min_mdi, i);
		}
	}

	min_mdi = 0xffff;
	for(i = START_IDX; i < END_IDX; i += 1)
	{
		if( GET_BACKGROUND_DATA(uSCAN_2, FACET2, i) < min_mdi)
		{
			min_mdi = GET_BACKGROUND_DATA(uSCAN_2, FACET2, i);
			FORKLIFTARM_Y_LIMIT[uSCAN_2][1] = Get_Y_Location(uSCAN_2, min_mdi, i);
		}
	}

	min_mdi = 0xffff;
	for(i = START_IDX; i < END_IDX; i += 1)
	{
		if( GET_BACKGROUND_DATA(uSCAN_2, FACET3, i) < min_mdi)
		{
			min_mdi = GET_BACKGROUND_DATA(uSCAN_2, FACET3, i);
			FORKLIFTARM_Y_LIMIT[uSCAN_2][2] = Get_Y_Location(uSCAN_2, min_mdi, i);
		}
	}

	min_mdi = 0xffff;
	for(i = START_IDX; i < END_IDX; i += 1)
	{
		if( GET_BACKGROUND_DATA(uSCAN_2, FACET4, i) < min_mdi)
		{
			min_mdi = GET_BACKGROUND_DATA(uSCAN_2, FACET4, i);
			FORKLIFTARM_Y_LIMIT[uSCAN_2][3] = Get_Y_Location(uSCAN_2, min_mdi, i);
		}
	}

	printf("MID Y value limit are :\n\r");
	printf("uSCAN1 0 = %4d\r\n", FORKLIFTARM_Y_LIMIT[uSCAN_1][0]);
	printf("uSCAN1 1 = %4d\r\n", FORKLIFTARM_Y_LIMIT[uSCAN_1][1]);
	printf("uSCAN1 2 = %4d\r\n", FORKLIFTARM_Y_LIMIT[uSCAN_1][2]);
	printf("uSCAN1 3 = %4d\r\n", FORKLIFTARM_Y_LIMIT[uSCAN_1][3]);

	printf("uSCAN2 0 = %4d\r\n", FORKLIFTARM_Y_LIMIT[uSCAN_2][0]);
	printf("uSCAN2 1 = %4d\r\n", FORKLIFTARM_Y_LIMIT[uSCAN_2][1]);
	printf("uSCAN2 2 = %4d\r\n", FORKLIFTARM_Y_LIMIT[uSCAN_2][2]);
	printf("uSCAN2 3 = %4d\r\n", FORKLIFTARM_Y_LIMIT[uSCAN_2][3]);

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

		static const unsigned char port_num_1 = uSCAN_1;
		osThreadDef(uSCAN_1, MAIN_THREAD, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 5);
		if(osThreadCreate(osThread(uSCAN_1), (void *)&port_num_1) != NULL)
		{
			printf("MAIN1_THREAD \t\t: OK\r\n");
		}
		else
		{
			Error_Handler();
		}
		//*/


		static const unsigned char port_num_2 = uSCAN_2;
		osThreadDef(uSCAN_2, MAIN_THREAD, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 5);
		if(osThreadCreate(osThread(uSCAN_2), (void *)&port_num_2) != NULL)
		{
			printf("MAIN1_THREAD \t\t: OK\r\n");
		}
		else
		{
			Error_Handler();
		}
		//*/

		osThreadDef(etc_thread1, ETC_THREAD, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 1);
		if(osThreadCreate(osThread(etc_thread1), NULL) != NULL)
		{
			printf("ETC_THREAD \t\t: OK\r\n");
		}
		else
		{
			Error_Handler();
		}

		osThreadDef(relay1, V_RELAY_THREAD, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 1);
		if(osThreadCreate(osThread(relay1), NULL) != NULL)
		{
			printf("RELAY1_THREAD \t\t: OK\r\n");
		}

		osThreadDef(relay2, P_RELAY_THREAD, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 1);
		if(osThreadCreate(osThread(relay2), NULL) != NULL)
		{
			printf("RELAY2_THREAD \t\t: OK\r\n");
		}
		//*/

		osThreadDef(relay3, FLAT_RELAY_THREAD, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 1);
		if(osThreadCreate(osThread(relay3), NULL) != NULL)
		{
			printf("FLATSCAN_RELAY_THREAD \t: OK\r\n");
		}
		//*/

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

//Height alarm, safety
static void FLAT_RELAY_THREAD (void const * argument)
{
	UNUSED(argument);

	unsigned char d1 = 1;
	unsigned char d2 = 1;

	while(1)
	{
		d1 = read_ip_selector(0);
		d2 = read_ip_selector(1);

		if (
				(d1 == 0) || (d2 == 0)
		   )
		{
			flatscan_relay_status = 1;
			relay_on(SAFETY);
		}
		else
		{
			flatscan_relay_status = 0;
			relay_off(SAFETY);
		}
		delay(100);
	}
}

static void P_RELAY_THREAD (void const * argument)
{
	UNUSED(argument);

	while(1)
	{
		if(
				(relay_flag[uSCAN_1][PEOPLE] == 1) ||
				(relay_flag[uSCAN_2][PEOPLE] == 1) ||
				(relay_flag[uSCAN_1][VEHICLE] == 1) ||
				(relay_flag[uSCAN_2][VEHICLE] == 1) ||
				(flatscan_relay_status == 1)

		  )
		{
			relay_toggle(PEOPLE);
		}
		else
		{
			relay_off(PEOPLE);
		}
		delay(1000);
	}
}
static void V_RELAY_THREAD (void const * argument)
{
	UNUSED(argument);

	//const int time_over_value = 600; 
	const int time_over_value = 100; //for test

	int time_over = 0;

	enum CASE_STEP relay_status = v_Step0;
	//enum CASE_STEP r_status = v_Step0;

	while(1)
	{

		//printf("%d %d\r\n", relay_flag[0][VEHICLE], relay_flag[1][VEHICLE]); 

		switch(relay_status)
		{
			case v_Step0:
				relay_off(VEHICLE);
				time_over = 0; 

				if(flatscan_relay_status == 1)
				{
					relay_status = v_Step0;
				}
				else if(
						(relay_flag[uSCAN_1][PEOPLE] == 0) && 
						(relay_flag[uSCAN_1][VEHICLE] == 0) && 
						(relay_flag[uSCAN_2][VEHICLE] == 1)
						)				  
				{
					//Enter from sensor 2
					relay_status = v_Step1;
					delay(RELAY_DELAY);
				}
				else if(
						(relay_flag[uSCAN_2][PEOPLE] == 0) &&
						(relay_flag[uSCAN_2][VEHICLE] == 0) && 
						(relay_flag[uSCAN_1][VEHICLE] == 1)
						)
				{
					//Enter from sensor 1
					relay_status = v_Step2;
					delay(RELAY_DELAY);
				}
				else
				{
					relay_status = v_Step0;
				}
				break;

			case v_Step1:
				relay_on(VEHICLE);
				
				if(flatscan_relay_status == 1)
				{
					relay_status = v_Step0;
				}
				else if(relay_flag[uSCAN_1][PEOPLE] == 1)
				{
					//Exit to sensor 1
					relay_status = f_Step;
				}
				else if(relay_flag[uSCAN_2][VEHICLE] == 1)
				{
					relay_status = v_Step1;
				}
				else
				{
					delay(100);
					time_over += 1;

					if(time_over_value < time_over)
					{
						printf("Time over\r\n");
						relay_status = v_Step0;
					}
					else
					{
						relay_status = v_Step1;
					}
				}

				break;
			
			case v_Step2:
				relay_on(VEHICLE);

				if(flatscan_relay_status == 1)
				{
					relay_status = v_Step0;
				}
				else if(relay_flag[uSCAN_2][PEOPLE] == 1)
				{
					//Exit to sensor 2
					relay_status = f_Step;
				}
				else if(relay_flag[uSCAN_1][VEHICLE] == 1)
				{
					relay_status = v_Step2;
				}
				else
				{
					delay(100);
					time_over += 1;

					if(time_over_value < time_over)
					{
						printf("Time over\r\n");
						relay_status = v_Step0;
					}
					else
					{
						relay_status = v_Step2;
					}
				}
				break;

			case f_Step:
				relay_on(VEHICLE);
				if(flatscan_relay_status == 1)
				{
					relay_status = v_Step0;
				}
				else if(
						(relay_flag[uSCAN_1][PEOPLE] == 0) && 
						(relay_flag[uSCAN_1][VEHICLE] == 0) && 
						(relay_flag[uSCAN_2][PEOPLE] == 0) && 
						(relay_flag[uSCAN_2][VEHICLE] == 0) 
					   )
				{
					relay_status = v_Step0;
				}
				else
				{
					relay_status = f_Step;
				}
				break;

			default:
				relay_status = v_Step0;
				break;
		}//end of switch
		

		/*
		if(r_status != relay_status)
		{
			print_case(0, relay_status);
		}
		
		r_status = relay_status;
		//*/

	}//End of while(1)
}

static void ETC_THREAD (void const * argument)
{
	UNUSED(argument);

	unsigned char d1, d2 = 0;
	unsigned char eeprom_data[10];

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


			//sever port : SERVER_PORT1
			eeprom_data[0] = 0;
			eeprom_data[1] = 0;
			eeprom_data[2] = 0x13;
			eeprom_data[3] = 0x88;
			eeprom_write(EEPROM_PORT(0), eeprom_data, 4);

			//sever port : SERVER_PORT2
			eeprom_data[0] = 0;
			eeprom_data[1] = 0;
			eeprom_data[2] = 0x13;
			eeprom_data[3] = 0x89;
			eeprom_write(EEPROM_PORT(1), eeprom_data, 4);

			printf("SET default IP, SYSTEM will be resetting\r\n");

			delay(500);

			NVIC_SystemReset();

		}//End of if((d2 ==0 ) && (d1 == 1))
		d2 = d1;

		if((tcp_cmd[0] == CMD_SETIP) || (tcp_cmd[1] == CMD_SETIP))
		{
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

			if(tcp_cmd[0] == CMD_SETIP)
			{
				tcp_cmd[0] = 0;
				eeprom_data[0] = uSCAN_1;
			}
			else
			{
				tcp_cmd[1] = 0;
				eeprom_data[0] = uSCAN_2;
			}

			eeprom_data[1] = 0x00;//Not use
			eeprom_data[2] = 0x52;//header 0
			eeprom_data[3] = 0x44;//header 1
			eeprom_data[4] = 0xff;//header 2
			eeprom_data[5] = ACK_SETIP; //CMD

			mutex_push_queue(eeprom_data, 6);

		}
	} //End of while(1)

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
	enum CASE_STEP case_status = v_Step0;
	enum CASE_STEP pre_status = v_Step0;

	unsigned char *frame_buffer;
	unsigned char *data_buffer1;
	unsigned char *data_buffer2;
	unsigned char *data_buffer3;
	unsigned char *data_buffer4;
	unsigned char eeprom_data[10];
	unsigned char setting_cmd;
	unsigned char *tcp_message = (unsigned char *)malloc(sizeof(unsigned char) * QUEUE_MAX_DATA_SIZE);
	unsigned char result[4];

	unsigned short tmp_short;

	int i,j;
	int address_offset;
	int share, remainder;
	int tilt_h, tilt_l;
	int x1, y1, x2, y2;
	int data_size;

	if(tcp_message == NULL)
	{
		printf("TCP buffer alloc faild\r\n");
		Error_Handler();
	}

	//Check routine for TCP connection
	while(tcp_connected != 0)
	{
		delay(100);
	}

	if(port_num == uSCAN_2)
	{
		//while(sensor_started != 0)
		while(1)
		{
			if(sensor_started == 1)
			{
				break;
			}
			else
			{
				delay(100);
			}
		}
	}

	if(port_num == uSCAN_1)
	{
		RS485_1_Init();
		UART_LZR_1_INT_ENABLE();
	}
	else
	{
		RS485_2_Init();
		UART_LZR_2_INT_ENABLE();
	}


	printf("++++++++++++++++++++++++++++++++++++++++++++\r\n");
	printf("MAIN Thread number %d\r\n", port_num);
	printf("--------------------------------------------\r\n");


	printf("++++++++++++++++++++++++++++++++++++++++++++\r\n");
	while(1)
	{
		i = 0;
		send_command(port_num, GET_IDENTITY);
		printf("GET IDENTITY\r\n");

		while(1)
		{
			analyze_message(port_num, &data_size, &setting_cmd);
			if(setting_cmd == SEND_IDENTITY)
			{
				goto label_a;
			}

			if(100 < i)
			{
				break;
			}
			i += 1;
		}
	}
label_a:
	printf("--------------------------------------------\r\n");

	printf("++++++++++++++++++++++++++++++++++++++++++++\r\n");
	input_uSCAN_parameter(port_num,
			ANGLE_START, ANGLE_END , MAX_POINT , //0 ~ 100, 100 spots
			ANGLE_START, ANGLE_END , MAX_POINT , //0 ~ 100, 100 spots
			ANGLE_START, ANGLE_END , MAX_POINT , //0 ~ 100, 100 spots
			ANGLE_START, ANGLE_END , MAX_POINT , //0 ~ 100, 100 spots
			0, //CTN field
			0, //MDI only
			0, //frame counter
			0, // CAN number
			0, // Heartbeat period
			4 //baudrate 4:921600
			);

	while(1)
	{
		i = 0;
		set_parameter(port_num);
		while(1)	//Wait Send Parameters
		{
			analyze_message(port_num, &data_size, &setting_cmd);
			if(setting_cmd == SEND_PARAMETERS)
			{
				goto label_b;
			}

			if(100 < i)
			{
				break;
			}

			i += 1;
		}
	}
label_b:

	address_offset = 0;

	if(get_can_status(port_num))
	{
		address_offset = address_offset + CAN_SIZE;
	}
	if(get_counter_status(port_num))
	{
		address_offset = address_offset + COUNT_ER_SIZE;
	}
	if(get_ctn_status(port_num))
	{
		address_offset = address_offset + CTN_SIZE;
	}

	printf("Address offset : %d\r\n", address_offset);

	printf("--------------------------------------------\r\n");

	send_command(port_num, STORE_PARAMETERS);
	printf("Send STORE_PARAMETERS\r\n");

	if(port_num == uSCAN_1)
	{
		sensor_started = 1;
	}

	printf("Sensor(%d) program start...\r\n\r\n\r\n", port_num);

	led_on(port_num);


	while(1)
	{
		frame_buffer = analyze_message(port_num, &data_size, &setting_cmd) + address_offset;

		switch(setting_cmd)
		{
			case MDI:

				data_buffer1 = assigned_buffer(port_num, frame_buffer, FACET1);
				data_buffer2 = assigned_buffer(port_num, frame_buffer, FACET2);
				data_buffer3 = assigned_buffer(port_num, frame_buffer, FACET3);
				data_buffer4 = assigned_buffer(port_num, frame_buffer, FACET4);

				if(configuration_mode[port_num] == 1)
				{
					//send buffer
					tcp_message[0] = port_num;//header 0
					tcp_message[1] = 0x00;//Not use

					tcp_message[2] = 0x52;//header 0
					tcp_message[3] = 0x44;//header 1
					tcp_message[4] = 0xff;//header 2
					tcp_message[5] = ACK_CONFIGMODE; //cmd

					tcp_message[6] = 0x00;//Not use

					tcp_message[7] = FACET1;
					memcpy(&tcp_message[8], data_buffer1, MAX_POINT_SIZE); //100point
					mutex_push_queue(tcp_message, 8+MAX_POINT_SIZE);	

					tcp_message[7] = FACET2;
					memcpy(&tcp_message[8], data_buffer2, MAX_POINT_SIZE); //100point
					mutex_push_queue(tcp_message, 8+MAX_POINT_SIZE);

					tcp_message[7] = FACET3;
					memcpy(&tcp_message[8], data_buffer3, MAX_POINT_SIZE); //100point
					mutex_push_queue(tcp_message, 8+MAX_POINT_SIZE);

					tcp_message[7] = FACET4;
					memcpy(&tcp_message[8], data_buffer4, MAX_POINT_SIZE); //100point
					mutex_push_queue(tcp_message, 8+MAX_POINT_SIZE);

				}


				data_buffer1 = assigned_buffer(port_num, frame_buffer, FACET1);
				result[0] = detect_function(
						port_num,
						FACET1, 
						data_buffer1, 
						detect_zone[port_num].x1, 
						detect_zone[port_num].y1, 
						detect_zone[port_num].x2, 
						//Use each curtain y limit value
						FORKLIFTARM_Y_LIMIT[port_num][FACET1]
						);
				//*/

				data_buffer2 = assigned_buffer(port_num, frame_buffer, FACET2);	
				result[1] = detect_function(
						port_num,
						FACET2, 
						data_buffer2, 
						detect_zone[port_num].x1, 
						detect_zone[port_num].y1, 
						detect_zone[port_num].x2, 
						FORKLIFTARM_Y_LIMIT[port_num][FACET2]
						);
				//*/

				data_buffer3 = assigned_buffer(port_num, frame_buffer, FACET3);
				result[2] = detect_function(
						port_num,
						FACET3, 
						data_buffer3, 
						detect_zone[port_num].x1, 
						detect_zone[port_num].y1, 
						detect_zone[port_num].x2, 
						FORKLIFTARM_Y_LIMIT[port_num][FACET3]
						);
				//*/

				data_buffer4 = assigned_buffer(port_num, frame_buffer, FACET4);
				result[3] = detect_function(
						port_num,
						FACET4, 
						data_buffer4, 
						detect_zone[port_num].x1, 
						detect_zone[port_num].y1, 
						detect_zone[port_num].x2, 
						FORKLIFTARM_Y_LIMIT[port_num][FACET4]
						);
				//*/


				//mutex_lock();
				//printf("%d => %d %d %d %d\r\n",port_num, result[0], result[1], result[2], result[3]);
				//mutex_unlock();

				//cur_status = 0;
				//compare = 0;


				switch(case_status)
				{
					case v_Step0: //Find begining

						//Only check the object on 4th curtain
						if( //Right direction
								(result[0] == NOTHING) &&
								(result[1] == NOTHING) &&
								//(result[2] == NOTHING) &&
								(result[3] != NOTHING)
						  )
						{
							//relay_flag[port_num][PEOPLE] = 1;
							case_status = v_Step1; 
						}
						else if( // 4th == nothing but 1st or 2nd or 3rd detected something
								(
								 (result[0] != NOTHING) ||
								 (result[1] != NOTHING) ||
								 (result[2] != NOTHING) 
								) &&
								(result[3] == NOTHING)
							   )
						{
							//relay_flag[port_num][PEOPLE] = 1;
							case_status = p_Step0; 
						}
						else
						{
							relay_flag[port_num][PEOPLE] = 0;
							relay_flag[port_num][VEHICLE] = 0;
							case_status = v_Step0;
						}
						break;

					case v_Step1: 
						relay_flag[port_num][PEOPLE] = 1;
						if(result[2] != NOTHING)
						{
							case_status = v_Step2;
						}
						else
						{
							case_status = v_Step0;
						}
						break;

					case v_Step2:
						relay_flag[port_num][PEOPLE] = 1;
						
						if(result[1] != NOTHING)
						//if(result[1] == VEHICLE)
						{
							case_status = v_Step3;
						}
						else
						{
							case_status = v_Step1;
						}
						break;

					case v_Step3:
						relay_flag[port_num][PEOPLE] = 1;
						
						if(result[0] != NOTHING)
						//if(result[0] == VEHICLE)
						{
							case_status = v_Step4;
						}
						else
						{
							case_status = v_Step2;
						}

						break;

					case v_Step4:
						if(
								/*
								//Insert or not, need to test in field
								(
								(result[2] != NOTHING) &&
								(result[3] != NOTHING)
								) &&
								//*/
								/*
								 (result[0] != NOTHING) &&
								 (result[1] == VEHICLE) &&
								 (result[2] == VEHICLE) &&
								 (result[3] == VEHICLE)
								 */
								
								 (result[1] != NOTHING) &&
								 (result[2] == VEHICLE) &&
								 (result[3] == VEHICLE)

								 /*
								(
								 (result[0] == VEHICLE) ||
								 (result[1] == VEHICLE) ||
								 (result[2] == VEHICLE)
								) &&
								 (result[3] == VEHICLE)
								 */
								
						  )
						{
							case_status = f_Step;
							relay_flag[port_num][VEHICLE] = 1;
							relay_flag[port_num][PEOPLE] = 0;

							printf("%d %d %d %d\r\n", 
									result[0],
									result[1],
									result[2],
									result[3]
								  );

							printf("Fork lift\r\n");
						}
						else
						{	
							case_status = p_Step0;
							relay_flag[port_num][VEHICLE] = 0;
							relay_flag[port_num][PEOPLE] = 1;

							printf("%d %d %d %d\r\n", 
									result[0],
									result[1],
									result[2],
									result[3]
								  );
							printf("People\r\n");

						}
						break;

					case p_Step0: //People
						case_status = f_Step;
						relay_flag[port_num][VEHICLE] = 0;
						relay_flag[port_num][PEOPLE] = 1;
						
						printf("%d %d %d %d\r\n", 
									result[0],
									result[1],
									result[2],
									result[3]
								  );
						printf("People\r\n");

						break;

					case f_Step:
						//Move to Step0 after all clear
						if(
								(result[0] == NOTHING) &&
								(result[1] == NOTHING) &&
								(result[2] == NOTHING) &&
								(result[3] == NOTHING)
						  )
						{
							case_status = v_Step0;
							relay_flag[port_num][PEOPLE] = 0;
							relay_flag[port_num][VEHICLE] = 0;
						}
						else
						{ ;	}
						break;

					default:
						case_status = v_Step0;
						break;
				}// End of switch(case_status)

				break; //End of case MDI:

			default:
				break;
		}

		if(case_status != pre_status)
		{
			print_case(port_num, case_status);
		}

		pre_status = case_status;

		//TCP COMMAND part
		switch(tcp_cmd[port_num])
		{
			case CMD_USERMODE:
				tcp_cmd[port_num] = 0;	
				configuration_mode[port_num] = 0;

				tcp_message[0] = port_num;//header 0
				tcp_message[1] = 0x00;//Not use
				tcp_message[2] = 0x52;//header 0
				tcp_message[3] = 0x44;//header 1
				tcp_message[4] = 0xff;//header 2
				tcp_message[5] = ACK_USERMODE; //CMD
				mutex_push_queue(tcp_message, 6);

				printf("SET USER MODE\r\n");
				break;

			case CMD_CONFIGMODE	:
				tcp_cmd[port_num] = 0;
				configuration_mode[port_num] = 1;

				tcp_message[0] = port_num;//header 0
				tcp_message[1] = 0x00;//Not use
				tcp_message[2] = 0x52;//header 0
				tcp_message[3] = 0x44;//header 1
				tcp_message[4] = 0xff;//header 2
				tcp_message[5] = ACK_CONFIGMODE; //CMD
				mutex_push_queue(tcp_message, 6);

				printf("SET CONFIG MODE\r\n");
				break;

			case CMD_LEFTMODE	:
				tcp_cmd[port_num] = 0;

				install_mode[port_num] = LEFT_MODE;
				eeprom_data[0] = LEFT_MODE;
				Init_Sin_Cos_Table(port_num, SENSOR_TILT[port_num]);
				eeprom_write(EEPROM_MODE(port_num), eeprom_data, 1);

				tcp_message[0] = port_num;//header 0
				tcp_message[1] = 0x00;//Not use
				tcp_message[2] = 0x52;//header 0
				tcp_message[3] = 0x44;//header 1
				tcp_message[4] = 0xff;//header 2
				tcp_message[5] = ACK_LEFTMODE; //CMD
				mutex_push_queue(tcp_message, 6);

				printf("SET LEFT INSTALLATION\r\n");
				break;

			case CMD_CENTERMODE	:
				tcp_cmd[port_num] = 0;

				install_mode[port_num] = CENTER_MODE;
				eeprom_data[0] = CENTER_MODE;
				Init_Sin_Cos_Table(port_num, SENSOR_TILT[port_num]);
				eeprom_write(EEPROM_MODE(port_num), eeprom_data, 1);

				tcp_message[0] = port_num;//header 0
				tcp_message[1] = 0x00;//Not use
				tcp_message[2] = 0x52;//header 0
				tcp_message[3] = 0x44;//header 1
				tcp_message[4] = 0xff;//header 2
				tcp_message[5] = ACK_CENTERMODE; //CMD
				mutex_push_queue(tcp_message, 6);

				printf("SET CENTER INSTALLATION\r\n");
				break;

			case CMD_SETTILT :	
				tcp_cmd[port_num] = 0;

				share = (int)SENSOR_TILT[port_num];
				remainder = (SENSOR_TILT[port_num] - (double)share) * 1000.0;

				SET_MSB_DATA(share, eeprom_data, 0, INTEGER);
				SET_MSB_DATA(remainder, eeprom_data, 4, INTEGER);

				eeprom_write(EEPROM_DEGREE(port_num), eeprom_data, 8);
				Init_Sin_Cos_Table(port_num, SENSOR_TILT[port_num]);


				tmp_short = 0xffff;
				for(i = START_IDX; i < END_IDX; i += 1)
				{
					if( GET_BACKGROUND_DATA(port_num, FACET1, i) < tmp_short)
					{
						tmp_short = GET_BACKGROUND_DATA(port_num, FACET1, i);
						FORKLIFTARM_Y_LIMIT[port_num][0] = Get_Y_Location(port_num, tmp_short, i);
					}
				}

				tmp_short = 0xffff;
				for(i = START_IDX; i < END_IDX; i += 1)
				{
					if( GET_BACKGROUND_DATA(port_num, FACET2, i) < tmp_short)
					{
						tmp_short = GET_BACKGROUND_DATA(port_num, FACET2, i);
						FORKLIFTARM_Y_LIMIT[port_num][1] = Get_Y_Location(port_num, tmp_short, i);
					}
				}

				tmp_short = 0xffff;
				for(i = START_IDX; i < END_IDX; i += 1)
				{
					if( GET_BACKGROUND_DATA(port_num, FACET3, i) < tmp_short)
					{
						tmp_short = GET_BACKGROUND_DATA(port_num, FACET3, i);
						FORKLIFTARM_Y_LIMIT[port_num][2] = Get_Y_Location(port_num, tmp_short, i);
					}
				}

				tmp_short = 0xffff;
				for(i = START_IDX; i < END_IDX; i += 1)
				{
					if( GET_BACKGROUND_DATA(port_num, FACET4, i) < tmp_short)
					{
						tmp_short = GET_BACKGROUND_DATA(port_num, FACET4, i);
						FORKLIFTARM_Y_LIMIT[port_num][3] = Get_Y_Location(port_num, tmp_short, i);
					}
				}

				printf("[%d][0] = %4d\r\n", port_num, FORKLIFTARM_Y_LIMIT[port_num][0]);
				printf("[%d][1] = %4d\r\n", port_num, FORKLIFTARM_Y_LIMIT[port_num][1]);
				printf("[%d][2] = %4d\r\n", port_num, FORKLIFTARM_Y_LIMIT[port_num][2]);
				printf("[%d][3] = %4d\r\n", port_num, FORKLIFTARM_Y_LIMIT[port_num][3]);

				tcp_message[0] = port_num;//header 0
				tcp_message[1] = 0x00;//Not use
				tcp_message[2] = 0x52;//header 0
				tcp_message[3] = 0x44;//header 1
				tcp_message[4] = 0xff;//header 2
				tcp_message[5] = ACK_SETTILT; //CMD
				mutex_push_queue(tcp_message, 6);

				printf("SET TILT %f\r\n", SENSOR_TILT[port_num]);
				break;

			case CMD_SETBGD		:
				tcp_cmd[port_num] = 0;

				beep_on();
				teach_in(port_num, 5, BACKGROUND_OFFSET);
				beep_off();


				//write
				for( j=0; j < 4; j += 1)
				{
					for( i=0; i < MAX_POINT; i += 1)
					{
						tmp_short = GET_BACKGROUND_DATA(port_num, j, i);

						eeprom_data[0] = (tmp_short >>0) & 0xff;
						eeprom_data[1] = (tmp_short >>8) & 0xff;

						eeprom_write(EEPROM_BACKGROUND(port_num) + (j*200) + (i << 1), 
								eeprom_data, 
								2
								);

						//printf("%5d ", tmp_short);
						//if((i%10) == 9)
						//{
						//	printf("\r\n");
						//}

					}
					//printf("\r\n");
				}

				//read
				for(j =0; j < 4; j += 1)
				{
					for(i = 0; i < MAX_POINT_SIZE; i += 2)
					{
						eeprom_read(
								EEPROM_BACKGROUND(port_num) +i +(j*200), 
								eeprom_data, 
								2
								);	

						//printf("%5d ", GET_LSB_DATA(eeprom_data, 0, SHORT));
						//if((i%20) == 18)
						//{
						//	printf("\r\n");
						//}
					}
					//printf("\r\n");
				}


				tmp_short = 0xffff;
				for(i = START_IDX; i < END_IDX; i += 1)
				{
					if( GET_BACKGROUND_DATA(port_num, FACET1, i) < tmp_short)
					{
						tmp_short = GET_BACKGROUND_DATA(port_num, FACET1, i);
						FORKLIFTARM_Y_LIMIT[port_num][0] = Get_Y_Location(port_num, tmp_short, i);
					}
				}

				tmp_short = 0xffff;
				for(i = START_IDX; i < END_IDX; i += 1)
				{
					if( GET_BACKGROUND_DATA(port_num, FACET2, i) < tmp_short)
					{
						tmp_short = GET_BACKGROUND_DATA(port_num, FACET2, i);
						FORKLIFTARM_Y_LIMIT[port_num][1] = Get_Y_Location(port_num, tmp_short, i);
					}
				}

				tmp_short = 0xffff;
				for(i = START_IDX; i < END_IDX; i += 1)
				{
					if( GET_BACKGROUND_DATA(port_num, FACET3, i) < tmp_short)
					{
						tmp_short = GET_BACKGROUND_DATA(port_num, FACET3, i);
						FORKLIFTARM_Y_LIMIT[port_num][2] = Get_Y_Location(port_num, tmp_short, i);
					}
				}

				tmp_short = 0xffff;
				for(i = START_IDX; i < END_IDX; i += 1)
				{
					if( GET_BACKGROUND_DATA(port_num, FACET4, i) < tmp_short)
					{
						tmp_short = GET_BACKGROUND_DATA(port_num, FACET4, i);
						FORKLIFTARM_Y_LIMIT[port_num][3] = Get_Y_Location(port_num, tmp_short, i);
					}
				}

				printf("[%d][0] = %4d\r\n", port_num, FORKLIFTARM_Y_LIMIT[port_num][0]);
				printf("[%d][1] = %4d\r\n", port_num, FORKLIFTARM_Y_LIMIT[port_num][1]);
				printf("[%d][2] = %4d\r\n", port_num, FORKLIFTARM_Y_LIMIT[port_num][2]);
				printf("[%d][3] = %4d\r\n", port_num, FORKLIFTARM_Y_LIMIT[port_num][3]);


				//eeprom write
				tcp_message[0] = port_num;//header 0
				tcp_message[1] = 0x00;//Not use
				tcp_message[2] = 0x52;//header 0
				tcp_message[3] = 0x44;//header 1
				tcp_message[4] = 0xff;//header 2
				tcp_message[5] = ACK_SETBGD; //CMD
				mutex_push_queue(tcp_message, 6);

				printf("SET BACK GROUND DATA\r\n");
				break;

			case CMD_SETZONE	:		
				tcp_cmd[port_num] = 0;

				//EEPROM_WRITE
				SET_MSB_DATA(detect_zone[port_num].x1, eeprom_data,  0, INTEGER);
				SET_MSB_DATA(detect_zone[port_num].y1, eeprom_data,  4, INTEGER);
				SET_MSB_DATA(detect_zone[port_num].x2, eeprom_data,  8, INTEGER);
				SET_MSB_DATA(detect_zone[port_num].y2, eeprom_data, 12, INTEGER);
				eeprom_write(EEPROM_DETECTZONE_ADDR(port_num), eeprom_data, 16);

				tcp_message[0] = port_num;//header 0
				tcp_message[1] = 0x00;//Not use
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
				tcp_cmd[port_num] = 0;	

				tcp_message[0] = port_num;//header 0
				tcp_message[1] = 0x00;//Not use
				tcp_message[2] = 0x52;//header 0
				tcp_message[3] = 0x44;//header 1
				tcp_message[4] = 0xff;//header 2
				tcp_message[5] = ACK_LOADBGD; //CMD
				tcp_message[6] = 0x00;//Not use

				for(j =0; j < 4; j += 1)
				{
					//tcp_message[7] = facet_num
					tcp_message[7] = j;

					for(i = 0; i < MAX_POINT_SIZE; i += 2)
					{
						eeprom_read(
								EEPROM_BACKGROUND(port_num) +(j*200) +i , 
								eeprom_data, 
								2);	

						tcp_message[8 + i + 0] = eeprom_data[0];
						tcp_message[8 + i + 1] = eeprom_data[1];

						/*printf("%5d ", GET_LSB_DATA(eeprom_data, 0, SHORT));
						  if((i%20) == 18)
						  {
						  printf("\r\n");
						  }
						//*/
					}
					//printf("\r\n");
					mutex_push_queue(tcp_message, 8 + MAX_POINT_SIZE);
				}

				printf("GET BACK GROUND DATA\r\n");
				break;

			case CMD_LOADTILT:
				tcp_cmd[port_num] = 0;	

				eeprom_read (EEPROM_DEGREE(port_num), eeprom_data, 8);
				tilt_h = GET_MSB_DATA(eeprom_data, 0, INTEGER);
				tilt_l = GET_MSB_DATA(eeprom_data, 1, INTEGER);

				tcp_message[0] = port_num;//header 0
				tcp_message[1] = 0x00;//Not use
				tcp_message[2] = 0x52;//header 0
				tcp_message[3] = 0x44;//header 1
				tcp_message[4] = 0xff;//header 2
				tcp_message[5] = ACK_LOADTILT; //CMD

				SET_MSB_DATA(tilt_h, tcp_message, 6, INTEGER);
				SET_MSB_DATA(tilt_l, tcp_message, 10, INTEGER);

				mutex_push_queue(tcp_message, 14);
				printf("GET SENSOR TILT\r\n");
				break;

			case CMD_LOADZONE:	
				tcp_cmd[port_num] = 0;

				eeprom_read(EEPROM_DETECTZONE_ADDR(port_num), eeprom_data, 16);

				x1 = GET_MSB_DATA(eeprom_data, 0, INTEGER);
				y1 = GET_MSB_DATA(eeprom_data, 1, INTEGER);
				x2 = GET_MSB_DATA(eeprom_data, 2, INTEGER);
				y2 = GET_MSB_DATA(eeprom_data, 3, INTEGER);

				tcp_message[0] = port_num;//header 0
				tcp_message[1] = 0x00;//Not use
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

			case CMD_REBOOT:	
				printf("SET default IP, SYSTEM will be resetting\r\n");
				NVIC_SystemReset();
				break;

			default :
				break;
		}
		//TCP COMMAND part


	}//End of while(1)

}

__inline__ static int detect_function(unsigned char port_num, int facet_num, unsigned char *data, int x1, int y1, int x2, int y2)
{	
	int i;

	unsigned short mdi[3] = {0, };
	int mx[3] = {0, };
	int my[3] = {0, };

	int x_begin = 0;
	int x_end	= 0; 
	int x_size 	= 0;

	int y_min = 0;

	int arm_cnt = 0;
	int people_cnt = 0;
	int vehicle_cnt = 0;

	enum CASE_STEP status = v_Step0;

	i = 0;
	//while( i < (MAX_POINT-2))
	while( i < MAX_POINT)
	{
		mdi[0] = GET_LSB_DATA(data, i+0, SHORT);
		mx[0] = Get_X_Location(port_num, (double)mdi[0], i+0);
		my[0] = Get_Y_Location(port_num, (double)mdi[0], i+0);

		/*
		mdi[1] = GET_LSB_DATA(data, i+1, SHORT);
		mx[1] = Get_X_Location(port_num, (double)mdi[1], i+1);
		my[1] = Get_Y_Location(port_num, (double)mdi[1], i+1);

		mdi[2] = GET_LSB_DATA(data, i+2, SHORT);
		mx[2] = Get_X_Location(port_num, (double)mdi[2], i+2);
		my[2] = Get_Y_Location(port_num, (double)mdi[2], i+2);
		//*/

		switch(status)
		{
			case v_Step0: //Find begining point
				if( 
						(MIN_IGNORE_ZONE < mdi[0]) && 
						(mdi[0] < GET_BACKGROUND_DATA(port_num, facet_num, i+0)) &&
						((x1 < mx[0]) && (mx[0] < x2)) &&
						((y1 < my[0]) && (my[0] < y2)) 
						
						/*
						&&
						((MIN_IGNORE_ZONE < mdi[1]) && (mdi[1] < GET_BACKGROUND_DATA(port_num, facet_num, i+1))) &&
						((x1 < mx[1]) && (mx[1] < x2)) &&
						((y1 < my[1]) && (my[1] < y2)) &&

						((MIN_IGNORE_ZONE < mdi[2]) && (mdi[2] < GET_BACKGROUND_DATA(port_num, facet_num, i+2))) &&
						((x1 < mx[2]) && (mx[2] < x2)) &&
						((y1 < my[2]) && (my[2] < y2))
						//*/

				  )
				{
					//x_begin = mx[1]; //Start x coordinate
					//y_min = my[1]; //Set first y minimum data 
	
					x_begin = mx[0]; //Start x coordinate
					
					x_end = mx[0]; //Store current x data into x_end variables
					
					y_min = my[0]; //Set first y minimum data 

					status = v_Step1;
					//i -= 1; //to set same start index point for find end-point
				}
				else
				{ ; }
				break;

			case v_Step1: //Find continuous points
				if( 
						(MIN_IGNORE_ZONE < mdi[0]) && 
						(mdi[0] < GET_BACKGROUND_DATA(port_num, facet_num, i+0)) &&
						((x1 < mx[0]) && (mx[0] < x2)) &&
						((y1 < my[0]) && (my[0] < y2)) 

						/*
						&&

						((MIN_IGNORE_ZONE < mdi[1]) && (mdi[1] < GET_BACKGROUND_DATA(port_num, facet_num, i+1))) &&
						((x1 < mx[1]) && (mx[1] < x2)) &&
						((y1 < my[1]) && (my[1] < y2)) &&

						((MIN_IGNORE_ZONE < mdi[2]) && (mdi[2] < GET_BACKGROUND_DATA(port_num, facet_num, i+2))) &&
						((x1 < mx[2]) && (mx[2] < x2)) &&
						((y1 < my[2]) && (my[2] < y2))
						//*/
				  )
				{
					/*
					x_end = mx[1]; //Store current x data into x_end variables

					if(my[1] < y_min) //replace y minimum data
					{
						y_min = my[1];
					}
					else
					{ ; }
					//*/

					x_end = mx[0]; //Store current x data into x_end variables

					if(my[0] < y_min) //replace y minimum data
					{
						y_min = my[0];
					}
					else
					{ ; }

					status = v_Step1;
				}
				else //No continuous case, it means pre loop data was valid, Caculate the x, y 
				{
					//Get x size
					x_size = x_end - x_begin;

					//To do check size
					if( x_size < IGNORE_SIZE)
					{
						people_cnt = people_cnt;
						vehicle_cnt = vehicle_cnt;

						// y2 (offset data from ground), check fork arm between ground and 20cm
						if( (y2-FORKLIFTARM_Y_LIMIT_OFFSET) < y_min)
						{
							arm_cnt += 1;
							//printf("%d >> A :5%d, %d\r\n", facet_num, x_size, arm_cnt);
						}
						else
						{ ; }
					}
					else if(x_size < PEOPLE_SIZE)
					{
						//printf("%d >> P :%5d\r\n", facet_num, x_size);
						
						// x is under people_size, only increase the people_cnt once
						people_cnt += 1;
						vehicle_cnt = vehicle_cnt;
					}
					else
					{
						//printf("%d >> V :%5d\r\n", facet_num, x_size);
						people_cnt = people_cnt;
						vehicle_cnt += 1;
					}

					//always have to go to first step
					status = v_Step0;
				}
				break;

			default:
				status = v_Step0;
				break;
		}// End of switch(status)

		i += 1;

	}//End of while()

	/*
	   if((vehicle_cnt != 0) || (people_cnt != 0))
	   {
	   printf("%d = p%3d, v%3d\r\n", facet_num, people_cnt, vehicle_cnt);
	   }
	//*/

	if( arm_cnt == 2)
	{
		return VEHICLE;
	}
	else if(0 < vehicle_cnt)
	{
		return VEHICLE;
	}
	else if(0 < people_cnt)
	{
		return PEOPLE;
	}
	else
	{
		return NOTHING;
	}
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
			if(result->data[0] == uSCAN_1)
			{
				tcp_server_write(&(result->data[2]), result->data_size - 2, 0);
			}
			else//uSCAN_2
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

	//printf("  State: Ethernet Initialization ...\n\r");

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
//#define PI 3.14159265358979323846
int Init_Sin_Cos_Table(unsigned char port_num, double tilt)
{
	const double PI = 3.14159265358979323846;
	int i;
	double Init_Degree = 100.0 + tilt;
	double start_degree = 100.0 / 99.0;
	double table_degree = 0.0;

	for (i = 0; i < MAX_POINT; i += 1)
	{
		table_degree = Init_Degree - (start_degree * i);

		if(install_mode == LEFT_MODE)
		{
			x_table[port_num][i] = sin((table_degree * PI) / 180);
			y_table[port_num][i] = cos((table_degree * PI) / 180);
		}
		else
		{
			x_table[port_num][i] = cos((table_degree * PI) / 180);
			y_table[port_num][i] = sin((table_degree * PI) / 180);
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
