/**
  ******************************************************************************
  * @file    LwIP/LwIP_HTTP_Server_Netconn_RTOS/Inc/main.h 
  * @author  MCD Application Team
  * @brief   Header for main.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/* Exported types ------------------------------------------------------------*/

#define MAX_AREA 4


struct DETECT_ZONE{
	int x1;
	int y1;
	int x2;
	int y2;
};
extern struct DETECT_ZONE detect_zone[][MAX_AREA];


struct RELAY_STATUS{
	__IO ITStatus DETECT_FLAG;
	__IO ITStatus RELAY_TRIGGER_MATRIX[4];
	unsigned char sig_status;
	unsigned char relay_assign_num;

	unsigned int min_detect_cnt;
	unsigned int hold_time_cnt;

};
extern struct RELAY_STATUS relay_status[][MAX_AREA];


extern unsigned char AREA_NUMBER[];
extern double SENSOR_TILT[];

extern unsigned char IP_ADDR[];
extern unsigned char NETMASK_ADDR[];
extern unsigned char GW_ADDR[];

extern unsigned int SERVER_PORT1;

//Custome code
extern unsigned char tcp_cmd[];
extern unsigned char INSTALLATION_DIRECTION[];
extern unsigned char MIN_OBJECT_SIZE[];
extern unsigned char MAX_OBJECT_SIZE[];
extern unsigned char config_mode[];
extern unsigned char RELAY_ENABLED[];

extern unsigned int MIN_DETECT_TIME[];
extern unsigned int SIG_HOLD_TIME[];



/* Exported constants --------------------------------------------------------*/ 

#define LZR_U92x
//#define LZR_U920
//#define MICROSCAN
//#define FLATSCAN

#define EEPROM_ADDR_OFFSET				0x20
#define EEPROM_BACKGROUND_OFFSET		0x240

//0x00
#define EEPROM_BACKGROUND(x)			0x00+(0x890*x) //2192 bytes, for920
//0x1120

#define EEPROM_IP_ADDR 					0x1130 //4 bytes
#define EEPROM_NETMASK_ADDR				0x1134 //4 bytes
#define EEPROM_GW_ADDR 					0x1138 //4 bytes
#define EEPROM_PORT(x)					0x113c + (0x04*x) //8 bytes
#define EEPROM_SIZE(x)					0x1144 + (0x04*x) //8 bytes
#define EEPROM_TILT(x)					0x114c + (0x08*x) //16 bytes
#define EEPROM_DIRECTION(x)				0x115c + (0x04*x) //8 bytes
#define EEPROM_MINTIME(x)				0x1164 + (0x04*x) //8 bytes, Min time(frame)
#define EEPROM_HOLDTIME(x)				0x116c + (0x04*x) //8 bytes, Hold time(frame)
#define EEPROM_RELAY(x)					0x1180 + (0x04*x) //8 bytes, relay 
#define EEPROM_RELAY_EN					0x1188

//Zone 128 bytes, 2 sensor * 4 area * 16 bytes
#define EEPROM_DETECTZONE(x, y)			0x1190+(0x40*x)+(0x10*y) 

#define LEFT_MODE	0
#define CENTER_MODE	1
#define RIGHT_MODE	2

#define CMD_USEMODE			0x01
#define CMD_CONFIGMODE		0x02
#define CMD_LEFTMODE		0x03
#define CMD_RIGHTMODE		0x04
#define CMD_SETTILT			0x05
#define CMD_LOADTILT		0x06
#define CMD_SETBGD			0x07
#define CMD_LOADBGD			0x08
#define CMD_SETZONE			0x09
#define CMD_LOADZONE		0x0A
#define CMD_SETSIZE			0x0B
#define CMD_LOADSIZE		0x0C
#define CMD_SETIP			0x0D
#define CMD_IGNOREDETECTION	0x0E
#define CMD_REBOOT			0x0F
#define CMD_SETMINTIME		0x10
#define CMD_LOADMINTIME		0x11
#define CMD_SETHOLDTIME		0x12
#define CMD_LOADHOLDTIME	0x13
#define CMD_SETRELAY		0x14
#define CMD_LOADRELAY		0x15

#define ACK				0xA0
#define HEART_BEAT		ACK
#define ACK_USEMODE			CMD_USEMODE				+ACK
#define ACK_CONFIGMODE		CMD_CONFIGMODE			+ACK
#define ACK_LEFTMODE		CMD_LEFTMODE			+ACK
#define ACK_RIGHTMODE		CMD_RIGHTMODE			+ACK
#define ACK_SETTILT			CMD_SETTILT				+ACK
#define ACK_LOADTILT		CMD_LOADTILT			+ACK
#define ACK_SETBGD			CMD_SETBGD				+ACK
#define ACK_LOADBGD			CMD_LOADBGD				+ACK
#define ACK_SETZONE			CMD_SETZONE				+ACK
#define ACK_LOADZONE		CMD_LOADZONE			+ACK
#define ACK_SETSIZE			CMD_SETSIZE				+ACK
#define ACK_LOADSIZE		CMD_LOADSIZE			+ACK
#define ACK_SETIP			CMD_SETIP				+ACK
#define ACK_IGNOREDETECTION	CMD_IGNOREDETECTION		+ACK
#define ACK_REBOOT			CMD_REBOOT				+ACK
#define ACK_SETMINTIME		CMD_SETMINTIME			+ACK	
#define ACK_LOADMINTIME	    CMD_LOADMINTIME	        +ACK
#define ACK_SETHOLDTIME	    CMD_SETHOLDTIME	        +ACK
#define ACK_LOADHOLDTIME    CMD_LOADHOLDTIME        +ACK
#define ACK_SETRELAY		CMD_SETRELAY			+ACK	
#define ACK_LOADRELAY		CMD_LOADRELAY			+ACK

/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */  
void Error_Handler(void);
void mutex_lock(void);
void mutex_unlock(void);
void delay(unsigned int delay_time);
int mutex_push_queue(unsigned char *data, int data_size);
int mutex_push_stack(unsigned char *data, int data_size);


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
