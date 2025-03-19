#include "stm32f7xx_hal.h"


#define EEPROM_ADDR 0xA0
#define EEPROM_ID_ADDR 0xB0

#define EEPROM_WRITE 0x00
#define EEPROM_READ  0x01

extern I2C_HandleTypeDef I2cHandle;

void EEPROM_Init(void);
void eeprom_wc_on(void);
void eeprom_wc_off(void);

int eeprom_write(unsigned short addr, unsigned char *tx_buff, int data_size);
int eeprom_read(unsigned short addr, unsigned char *rx_buff, int data_size);

