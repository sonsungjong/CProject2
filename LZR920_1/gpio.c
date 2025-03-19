/**
 ******************************************************************************
 * File Name          : gpio.c
 * Description        : This file provides code for the configuration
 *                      of all used GPIO pins.
 ******************************************************************************
 
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"
#include "main.h"

#define MAX_LED 4
#define MAX_RELAY 4
#define MAX_SWITCH 2

//LED
uint32_t GPIO_PIN[] = { GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15};
GPIO_TypeDef* GPIO_BUS[] = { GPIOB};

//RELAY
uint32_t RELAY_PIN[] = { GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_11 };
GPIO_TypeDef* RELAY_BUS[] = { GPIOA};

//SW
uint32_t SWITCH_PIN[] = { GPIO_PIN_13, GPIO_PIN_12};
GPIO_TypeDef* SWITCH_BUS[] = { GPIOD};

//IP SELECTOR
uint32_t IP_SELECTOR_PIN[] = { GPIO_PIN_4, GPIO_PIN_2};
GPIO_TypeDef* IPSELECTOR_BUS[] = {GPIOE};


/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/

void GPIO_Init(void) {

	GPIO_InitTypeDef GPIO_InitStruct;

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5,  GPIO_PIN_RESET);		//BEEP

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);		//LED0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);		//LED1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);		//LED2
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);		//LED3

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);	//RELAY1
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);	//RELAY2
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);	//RELAY3
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);	//RELAY4

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);	//IP selector 3v3 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);	//IP selector 3v3 2


//#ifdef FULL_DUPLEX ==> Move to MspInit function in Usart.c
/*
#if defined(MICROSCAN) || defined(FLATSCAN)
	__HAL_RCC_GPIOF_CLK_ENABLE();
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_SET);	//RS485_RE2
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_8, GPIO_PIN_SET);	//RS485_RE1

	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	GPIO_InitStruct.Pin = GPIO_PIN_4;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct); //RS485_RE2

	GPIO_InitStruct.Pin = GPIO_PIN_8;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct); //RS485_RE1
#endif
*/

	//+++++++++ GPIO OUT PUT +++++++++++++++++++//

	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	GPIO_InitStruct.Pin = GPIO_PIN_5;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct); //BEEP

	GPIO_InitStruct.Pin = GPIO_PIN_12;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct); //LED0

	GPIO_InitStruct.Pin = GPIO_PIN_13;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct); //LED1

	GPIO_InitStruct.Pin = GPIO_PIN_14;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct); //LED2

	GPIO_InitStruct.Pin = GPIO_PIN_15;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct); //LED3

	GPIO_InitStruct.Pin = GPIO_PIN_8;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); //RELAY1

	GPIO_InitStruct.Pin = GPIO_PIN_9;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); //RELAY2

	GPIO_InitStruct.Pin = GPIO_PIN_10;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); //RELAY3

	GPIO_InitStruct.Pin = GPIO_PIN_11;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); //RELAY4


	//IP SELECTOR
	GPIO_InitStruct.Pin = GPIO_PIN_6;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct); //IP selector 3v3
	GPIO_InitStruct.Pin = GPIO_PIN_7;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct); //IP selector 3v3
	//IP SELECTOR
	//--------- GPIO OUT PUT -------------------//

	//+++++++++ GPIO INPUT +++++++++++++++++++//
	///GPIO INPUT PART
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	GPIO_InitStruct.Pin = GPIO_PIN_12;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct); //SW2

	GPIO_InitStruct.Pin = GPIO_PIN_13;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct); //SW1

	//IP SELECTOR input
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Pin = GPIO_PIN_4;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct); //IP SELECTOR option1
	GPIO_InitStruct.Pin = GPIO_PIN_2;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct); //IP SELECTOR option2
	//--------- GPIO INPUT -------------------//
}

void beep_on(void){
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
}

void beep_off(void){
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
}

void led_on(char num) {
	HAL_GPIO_WritePin(GPIO_BUS[0], GPIO_PIN[num%MAX_LED], GPIO_PIN_RESET);
}

void led_off(char num) {
	HAL_GPIO_WritePin(GPIO_BUS[0], GPIO_PIN[num%MAX_LED], GPIO_PIN_SET);
}

void led_toggle(char num) {
	HAL_GPIO_TogglePin(GPIO_BUS[0], GPIO_PIN[num%MAX_LED]);
}

void relay_on(char num) {
	HAL_GPIO_WritePin(RELAY_BUS[0], RELAY_PIN[num%MAX_RELAY], GPIO_PIN_SET);
}

void relay_off(char num) {
	HAL_GPIO_WritePin(RELAY_BUS[0], RELAY_PIN[num%MAX_RELAY], GPIO_PIN_RESET);
}

void relay_toggle(char num) {
	HAL_GPIO_TogglePin(RELAY_BUS[0], RELAY_PIN[num%MAX_RELAY]);
}

GPIO_PinState read_sw(char num)
{
	return HAL_GPIO_ReadPin(SWITCH_BUS[0], SWITCH_PIN[num%MAX_SWITCH]);
}

GPIO_PinState read_ip_selector(char num)
{
	return HAL_GPIO_ReadPin(IPSELECTOR_BUS[0], IP_SELECTOR_PIN[num%2]);
}
