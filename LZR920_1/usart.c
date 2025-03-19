/**
 ******************************************************************************
 * File Name          : USART.c
 * Description        : This file provides code for the configuration
 *                      of the USART instances.
 ******************************************************************************
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"
#include "gpio.h"
#include "main.h"
#include "tcpserver_socket.h"
#include <struct_data.h>

#if defined(LZR_U92x)
#include "LZR_U92x.h"
#elif defined(FLATSCAN)
#include "FLATSCAN.h"
#elif defined(MICROSCAN)
#include "microscan.h"
#endif


uint8_t uart6_rx_buff[UART_BUFFER_SIZE];
uint8_t uart7_rx_buff[UART_BUFFER_SIZE];
uint8_t uart2_rx_buff[UART_BUFFER_SIZE];

__IO ITStatus Uart2_Ready = RESET;
__IO ITStatus Uart6_Ready = RESET;
__IO ITStatus Uart7_Ready = RESET;

UART_HandleTypeDef huart7;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart6;

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

PUTCHAR_PROTOTYPE
{
	while(HAL_UART_Transmit_DMA(&huart6, (uint8_t *)&ch, 1) != HAL_OK);
	return ch;
}

void RS485_1_Init(void)
{
	huart7.Instance = UART7;
#if defined(MICROSCAN) || defined(FLATSCAN)
	huart7.Init.BaudRate = 921600;
	huart7.Init.HwFlowCtl = UART_HWCONTROL_NONE;
#else
	huart7.Init.BaudRate = 460800;
	huart7.Init.HwFlowCtl = UART_HWCONTROL_RTS;
#endif
	huart7.Init.WordLength = UART_WORDLENGTH_8B;
	huart7.Init.StopBits = UART_STOPBITS_1;
	huart7.Init.Parity = UART_PARITY_NONE;
	huart7.Init.Mode = UART_MODE_TX_RX;
	huart7.Init.OverSampling = UART_OVERSAMPLING_16;
	huart7.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart7.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	//#ifdef FULL_DUPLEX
#if defined(MICROSCAN) || defined(FLATSCAN)
	if (HAL_UART_Init(&huart7) != HAL_OK)
	{
		Error_Handler();
	}

#else
	if (HAL_RS485Ex_Init(&huart7, UART_DE_POLARITY_HIGH, 1, 1) != HAL_OK)
	{
		Error_Handler();
	}
#endif
}

void RS485_2_Init(void)
{
	huart2.Instance = USART2;

#if defined(MICROSCAN) || defined(FLATSCAN)
	huart2.Init.BaudRate = 921600;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
#else
	huart2.Init.BaudRate = 460800;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_RTS;
#endif
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

	//#ifdef FULL_DUPLEX
#if defined(MICROSCAN) || defined(FLATSCAN)
	if (HAL_UART_Init(&huart2) != HAL_OK)
	{
		Error_Handler();
	}
#else
	if (HAL_RS485Ex_Init(&huart2, UART_DE_POLARITY_HIGH, 1, 1) != HAL_OK)
	{
		Error_Handler();
	}
#endif
}

void DEBUG_UART_Init(void)
{

	huart6.Instance = USART6;
#if defined(MICROSCAN) || defined(FLATSCAN)
	huart6.Init.BaudRate = 921600;
#else
	huart6.Init.BaudRate = 460800;
#endif
	huart6.Init.WordLength = UART_WORDLENGTH_8B;
	huart6.Init.StopBits = UART_STOPBITS_1;
	huart6.Init.Parity = UART_PARITY_NONE;
	huart6.Init.Mode = UART_MODE_TX_RX;
	huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart6.Init.OverSampling = UART_OVERSAMPLING_16;
	huart6.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart6.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart6) != HAL_OK)
	{
		Error_Handler();
	}
}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{
	static DMA_HandleTypeDef hdma_tx;
	static DMA_HandleTypeDef hdma_rx;

	RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;

	GPIO_InitTypeDef GPIO_InitStruct;

	if(uartHandle->Instance==UART7)
	{
		__HAL_RCC_GPIOF_CLK_ENABLE(); 

		RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_UART7;
		RCC_PeriphClkInit.Uart7ClockSelection = RCC_UART7CLKSOURCE_SYSCLK;
		HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);

		__HAL_RCC_UART7_CLK_ENABLE();

		/**UART7 GPIO Configuration    
		  PF7     ------> UART7_TX
		  PF6     ------> UART7_RX
		  PF8     ------> UART7_RTS 
		  */
		GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_6;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF8_UART7;
		HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

#if defined(LZR_U92x) || defined(LZR_U920) || defined(LZR_U921)
		GPIO_InitStruct.Pin = GPIO_PIN_8;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF8_UART7;
		HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
#else
		__HAL_RCC_GPIOF_CLK_ENABLE();
		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_8, GPIO_PIN_SET);	//RS485_RE1

		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

		GPIO_InitStruct.Pin = GPIO_PIN_8;
		HAL_GPIO_Init(GPIOF, &GPIO_InitStruct); //RS485_RE1
#endif

		HAL_NVIC_SetPriority(UART7_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(UART7_IRQn);		
	}
	else if(uartHandle->Instance==USART2)
	{
		__HAL_RCC_GPIOD_CLK_ENABLE(); 

		RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
		RCC_PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_SYSCLK;
		HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);		

		__HAL_RCC_USART2_CLK_ENABLE();

		/**USART2 GPIO Configuration    
		  PD6     ------> USART2_RX
		  PD5     ------> USART2_TX
		  PD4     ------> USART2_RTS 
		  */
		GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_5;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
		HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

#if defined(LZR_U92x) || defined(LZR_U920) || defined(LZR_U921)
		GPIO_InitStruct.Pin = GPIO_PIN_4;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
		HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
#else
		__HAL_RCC_GPIOF_CLK_ENABLE();
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_SET);	//RS485_RE2

		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

		GPIO_InitStruct.Pin = GPIO_PIN_4;
		HAL_GPIO_Init(GPIOD, &GPIO_InitStruct); //RS485_RE2
#endif

		HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(USART2_IRQn);		
	}
	else if(uartHandle->Instance==USART6)
	{
		__HAL_RCC_GPIOC_CLK_ENABLE(); 

		RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART6;
		RCC_PeriphClkInit.Usart6ClockSelection = RCC_USART6CLKSOURCE_SYSCLK;
		HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);

		__HAL_RCC_USART6_CLK_ENABLE();

#if USE_UART_DMA
		__HAL_RCC_DMA2_CLK_ENABLE();
#endif

		/**USART6 GPIO Configuration    
		  PC7     ------> USART6_RX
		  PC6     ------> USART6_TX 
		  */
		GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_6;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

#if USE_UART_DMA
		hdma_tx.Instance                 = DMA2_Stream6;
		hdma_tx.Init.Channel             = DMA_CHANNEL_5;
		hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
		hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
		hdma_tx.Init.MemInc              = DMA_MINC_DISABLE;
		hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
		hdma_tx.Init.Mode                = DMA_NORMAL;
		hdma_tx.Init.Priority            = DMA_PRIORITY_LOW;
		HAL_DMA_Init(&hdma_tx);
		__HAL_LINKDMA(uartHandle, hdmatx, hdma_tx);

		hdma_rx.Instance                 = DMA2_Stream1;
		hdma_rx.Init.Channel             = DMA_CHANNEL_5;
		hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
		hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
		hdma_rx.Init.MemInc              = DMA_MINC_DISABLE;
		hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
		hdma_rx.Init.Mode                = DMA_CIRCULAR;
		hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;
		HAL_DMA_Init(&hdma_rx);
		__HAL_LINKDMA(uartHandle, hdmarx, hdma_rx);

		HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 0, 1);
		HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);
		HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);
#endif

		HAL_NVIC_SetPriority(USART6_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(USART6_IRQn);
	}
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

	if(uartHandle->Instance==UART7)
	{
		__HAL_RCC_UART7_CLK_DISABLE();

		/**UART7 GPIO Configuration    
		  PF7     ------> UART7_TX
		  PF6     ------> UART7_RX
		  PF8     ------> UART7_RTS 
		  */
		HAL_GPIO_DeInit(GPIOF, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_8);

		HAL_NVIC_DisableIRQ(UART7_IRQn);

		/* USER CODE END UART7_MspDeInit 1 */
	}
	else if(uartHandle->Instance==USART2)
	{
		__HAL_RCC_USART2_CLK_DISABLE();

		/**USART2 GPIO Configuration    
		  PD6     ------> USART2_RX
		  PD5     ------> USART2_TX
		  PD4     ------> USART2_RTS 
		  */
		HAL_GPIO_DeInit(GPIOD, GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4);
		HAL_NVIC_DisableIRQ(USART2_IRQn);

		HAL_NVIC_DisableIRQ(DMA1_Stream3_IRQn);
	}
	else if(uartHandle->Instance==USART6)
	{
		__HAL_RCC_USART6_CLK_DISABLE();

		/**USART6 GPIO Configuration    
		  PC7     ------> USART6_RX
		  PC6     ------> USART6_TX 
		  */
		HAL_GPIO_DeInit(GPIOC, GPIO_PIN_7|GPIO_PIN_6);

#if USE_UART_DMA		
		if(uartHandle->hdmarx != 0)
		{
			HAL_DMA_DeInit(uartHandle->hdmarx);
		}
		if(uartHandle->hdmatx != 0)
		{
			HAL_DMA_DeInit(uartHandle->hdmatx);
		}  

		HAL_NVIC_DisableIRQ(DMA2_Stream6_IRQn);
		HAL_NVIC_DisableIRQ(DMA2_Stream1_IRQn);
#endif
		HAL_NVIC_DisableIRQ(USART6_IRQn);
	}
} 

void DEBUG_INT_ENABLE(void)
{
	while(HAL_UART_Receive_DMA(&huart6, (uint8_t *)uart6_rx_buff, 1) != HAL_OK);
}

void UART_LZR_1_INT_ENABLE(void)
{
	while(HAL_UART_Receive_IT(&huart7, (uint8_t *)uart7_rx_buff, 1) != HAL_OK);
}

void UART_LZR_2_INT_ENABLE(void)
{
	while(HAL_UART_Receive_IT(&huart2, (uint8_t *)uart2_rx_buff, 1) != HAL_OK);
}
void UART_INT_ENABLE(void)
{
	while(HAL_UART_Receive_IT(&huart7, (uint8_t *)uart7_rx_buff, 1) != HAL_OK);
	while(HAL_UART_Receive_IT(&huart2, (uint8_t *)uart2_rx_buff, 1) != HAL_OK);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{

	if(UartHandle->Instance==USART6)
	{
		//usb A type debug port
		Uart6_Ready = SET;
		HAL_UART_Transmit_DMA(&huart6, (uint8_t*)uart6_rx_buff, 1);
	}
	else if(UartHandle->Instance==UART7)
	{
		Uart7_Ready = SET;
		//led_toggle(0);

#if defined(LZR_U92x)
		rx_function(LZR_1, uart7_rx_buff[0]);
#elif defined(LZR_U920) || defined(LZR_U921)
		LZR_U920_buff_change(LZR_1, uart7_rx_buff[0]);
#elif defined(MICROSCAN)
		buff_changer(uSCAN_1, uart7_rx_buff[0]);
#elif defined(FLATSCAN)
		buff_changer(U951_1, uart7_rx_buff[0]);
#endif
		//Enable interrupt part, stm32 uart needs a function below always for making next interrupt
		HAL_UART_Receive_IT(&huart7, (uint8_t *)uart7_rx_buff, 1);

		//Echo for test
		//HAL_UART_Transmit_IT(&huart7, (uint8_t *)uart7_rx_buff, 1);

	}
	else if(UartHandle->Instance==USART2)
	{
		Uart2_Ready = SET;
		//led_toggle(1);

#if defined(LZR_U92x)
		rx_function(LZR_2, uart2_rx_buff[0]);
#elif defined(LZR_U920) || defined(LZR_U921)
		LZR_U920_buff_change(LZR_2, uart2_rx_buff[0]);
#elif defined(MICROSCAN)
		buff_changer(uSCAN_2, uart2_rx_buff[0]);
#elif defined(FLATSCAN)
		buff_changer(U951_2, uart2_rx_buff[0]);
#endif
		//Enable interrupt part, stm32 uart needs a function below always for making next interrupt
		HAL_UART_Receive_IT(&huart2, (uint8_t *)uart2_rx_buff, 1);

		//Echo for test
		//HAL_UART_Transmit_IT(&huart2, (uint8_t *)uart2_rx_buff, 1);
	}	
}


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	if(UartHandle->Instance==USART6)
	{
	}
	else if(UartHandle->Instance==UART7)
	{
	}
	else if(UartHandle->Instance==USART2)
	{
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{

	if(UartHandle->Instance==USART6)
	{
		if(UartHandle->ErrorCode == HAL_UART_ERROR_ORE)
		{
			while(HAL_UART_Receive_DMA(&huart6, (uint8_t *)uart6_rx_buff, 1) != HAL_OK);
		}
	}
	else if(UartHandle->Instance==UART7)
	{
		if(UartHandle->ErrorCode == HAL_UART_ERROR_ORE)
		{
			while(HAL_UART_Receive_IT(&huart7, (uint8_t *)uart7_rx_buff, 1)!= HAL_OK);
		}
	}
	else if(UartHandle->Instance==USART2)
	{
		if(UartHandle->ErrorCode == HAL_UART_ERROR_ORE)
		{
			while(HAL_UART_Receive_IT(&huart2, (uint8_t *)uart2_rx_buff, 1)!= HAL_OK);
		}
	}

}

void RS485_Send_delay(unsigned char port_num, unsigned char *packet, unsigned short size, int ms)
{
	int i;
	switch(port_num)
	{
		case 0 :
			for(i = 0; i < size; i++)
			{
				HAL_UART_Transmit_IT(&huart7, (packet+i), 1);
				//delay(50);
				delay(ms);
			}
			break;
		case 1 :
			for(i = 0; i < size; i++)
			{
				HAL_UART_Transmit_IT(&huart2, (packet+i), 1);
				//delay(50);
				delay(ms);
			}
			break;
		default : break;
	}
}

void RS485_Send_nodelay(unsigned char port_num, unsigned char *packet, unsigned short size)
{
	switch(port_num)
	{
		case 0 :	HAL_UART_Transmit_IT(&huart7, packet, size);
					break;
		case 1 :	HAL_UART_Transmit_IT(&huart2, packet, size);
					break;
		default : break;
	}
}

