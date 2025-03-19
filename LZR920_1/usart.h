/**
  ******************************************************************************
  * File Name          : USART.h
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************

  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

#define USE_UART_DMA 1
#define UART_BUFFER_SIZE 1

extern uint8_t uart6_rx_buff[UART_BUFFER_SIZE];
extern uint8_t uart7_rx_buff[UART_BUFFER_SIZE];
extern uint8_t uart2_rx_buff[UART_BUFFER_SIZE];
extern __IO ITStatus Uart2_Ready;
extern __IO ITStatus Uart6_Ready;
extern __IO ITStatus Uart7_Ready;
extern UART_HandleTypeDef huart7;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart6;

extern void Error_Handler(void);

void RS485_1_Init(void);
void RS485_2_Init(void);
//void Debug_Uart_Init(void);
void DEBUG_UART_Init(void);
void DEBUG_INT_ENABLE(void);
void UART_LZR_1_INT_ENABLE(void);
void UART_LZR_2_INT_ENABLE(void);
void UART_INT_ENABLE(void);
void RS485_Send_delay(unsigned char port_num, unsigned char *packet, unsigned short size, int ms);
void RS485_Send_nodelay(unsigned char port_num, unsigned char *packet, unsigned short size);


/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
