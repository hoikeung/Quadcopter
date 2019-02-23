
#ifndef __I2C_H__

#define __I2C_H__

#include "stm32f1xx_hal.h"

void I2C1_Init(void);
void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c);
void I2C1_Read_Without_Register(uint16_t device_address, uint8_t *pData, uint16_t size);
void I2C1_Write_With_Register(uint16_t device_address, uint16_t device_register, uint8_t *pData, uint16_t size);

#endif
