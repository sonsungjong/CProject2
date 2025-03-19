/**
  ******************************************************************************
  * File Name          : gpio.h
  * Description        : This file contains all the functions prototypes for 
  *                      the gpio  
  ******************************************************************************
 
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

void GPIO_Init(void);

void beep_on(void);
void beep_off(void);
void led_on(char num);
void led_off(char num);
void led_toggle(char num);
void relay_on(char num);
void relay_off(char num);
void relay_toggle(char num);
GPIO_PinState read_sw(char num);
GPIO_PinState read_ip_selector(char num);

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
