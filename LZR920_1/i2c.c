#include "i2c.h"
#include "gpio.h"
#include "main.h"


I2C_HandleTypeDef I2cHandle;


void EEPROM_Init(void)
{
	I2cHandle.Instance             = I2C1;
	I2cHandle.Init.Timing          = 0x40912732;//0x00D00E28;
	I2cHandle.Init.OwnAddress1     = EEPROM_ADDR;//0x30F;
	I2cHandle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;//I2C_ADDRESSINGMODE_10BIT;
	I2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	I2cHandle.Init.OwnAddress2     = 0xFF;
	I2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	I2cHandle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;

	if(HAL_I2C_Init(&I2cHandle) != HAL_OK)
	{
	  /* Initialization Error */
	  Error_Handler();
	}

	/* Enable the Analog I2C Filter */
	HAL_I2CEx_ConfigAnalogFilter(&I2cHandle,I2C_ANALOGFILTER_ENABLE);
}

/**
  * @brief I2C MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - DMA configuration for transmission request by peripheral
  *           - NVIC configuration for DMA interrupt request enable
  * @param hi2c: I2C handle pointer
  * @retval None
  */
void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  RCC_PeriphCLKInitTypeDef  RCC_PeriphCLKInitStruct;

  /*##-1- Configure the I2C clock source. The clock is derived from the SYSCLK #*/
  RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
  RCC_PeriphCLKInitStruct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);

  //EEPROM_WC, PG10 extern pull up
  __HAL_RCC_GPIOG_CLK_ENABLE();
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  GPIO_InitStruct.Pin = GPIO_PIN_10;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);


  /*##-2- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  /* Enable I2Cx clock */
  __HAL_RCC_I2C1_CLK_ENABLE();

  /*##-3- Configure peripheral GPIO ##########################################*/
  /* I2C TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = GPIO_PIN_8;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* I2C RX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = GPIO_PIN_9;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*##-4- Configure the NVIC for I2C ########################################*/
  /* NVIC for I2Cx */
  //HAL_NVIC_SetPriority(I2C1_ER_IRQn, 0, 1);
  //HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
  //HAL_NVIC_SetPriority(I2C1_EV_IRQn, 0, 2);
  //HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
}

/**
  * @brief I2C MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO, DMA and NVIC configuration to their default state
  * @param hi2c: I2C handle pointer
  * @retval None
  */
void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c)
{

  /*##-1- Reset peripherals ##################################################*/
	__HAL_RCC_I2C1_FORCE_RESET();
	__HAL_RCC_I2C1_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks #################################*/
  /* Configure I2C Tx as alternate function  */
  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8);
  /* Configure I2C Rx as alternate function  */
  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_9);

  /*##-3- Disable the NVIC for I2C ##########################################*/
  HAL_NVIC_DisableIRQ(I2C1_ER_IRQn);
  HAL_NVIC_DisableIRQ(I2C1_EV_IRQn);
}

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{
  /* Toggle LED1: Transfer in transmission process is correct */
	led_toggle(0);
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{
  /* Toggle LED1: Transfer in reception process is correct */
	led_toggle(1);
}


void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *I2cHandle)
{
  /** Error_Handler() function is called when error occurs.
    * 1- When Slave don't acknowledge it's address, Master restarts communication.
    * 2- When Master don't acknowledge the last data transferred, Slave don't care in this example.
    */
  if (HAL_I2C_GetError(I2cHandle) != HAL_I2C_ERROR_AF)
  {
    Error_Handler();
  }
}

int eeprom_write(unsigned short addr, unsigned char *tx_buff, int data_size)
{
	int i;
	unsigned char tmp[34];

	int size = data_size+2;

	tmp[0] = (addr >> 8) & 0x00ff;
	tmp[1] = addr & 0x00ff;

	for(i = 0; i < data_size; i++)
	{
		tmp[i+2] = tx_buff[i];
	}

	eeprom_wc_off();

	while(HAL_I2C_Master_Transmit(&I2cHandle, (uint16_t)(EEPROM_ADDR | EEPROM_WRITE), (uint8_t*)tmp, size, 10000)!= HAL_OK)
	{
		if (HAL_I2C_GetError(&I2cHandle) != HAL_I2C_ERROR_AF)
		{
			return 0;
			//Error_Handler();
		}
	}
	eeprom_wc_on();

	return 1;
}

int eeprom_read(unsigned short addr, unsigned char *rx_buff, int data_size)
{
	unsigned char tmp[2];

	tmp[0] = (addr >> 8) & 0x00ff;
	tmp[1] = addr & 0x00ff;

	//eeprom_wc_on();

	while(HAL_I2C_Master_Transmit(&I2cHandle, (uint16_t)(EEPROM_ADDR | EEPROM_WRITE), tmp, 2, 10000)!= HAL_OK)
	{
		/* Error_Handler() function is called when Timeout error occurs.
		   When Acknowledge failure occurs (Slave don't acknowledge its address)
		   Master restarts communication */
		if (HAL_I2C_GetError(&I2cHandle) != HAL_I2C_ERROR_AF)
		{
			return 0;
			//Error_Handler();
		}
	}


	while(HAL_I2C_Master_Receive(&I2cHandle, (uint16_t)(EEPROM_ADDR | EEPROM_READ), rx_buff, data_size, 10000) != HAL_OK)
	{
		/* Error_Handler() function is called when Timeout error occurs.
		   When Acknowledge failure occurs (Slave don't acknowledge it's address)
		   Master restarts communication */
		if (HAL_I2C_GetError(&I2cHandle) != HAL_I2C_ERROR_AF)
		{
			return 0;
			//Error_Handler();
		}
	}

	return 1;

}

void eeprom_wc_on(void)
{
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, GPIO_PIN_SET);
}

void eeprom_wc_off(void)
{
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, GPIO_PIN_RESET);
}
