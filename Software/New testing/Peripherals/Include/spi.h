
#ifndef __SPI_H__

#define __SPI_H__

#include "stm32f1xx_hal.h"
#include "spi.h"
#include "system.h"

void SPI2_Init(void);
void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi);
void SPI2_Transmit(uint8_t *pData, uint16_t size);
void SPI2_Receive(uint8_t *pData, uint16_t size);

#endif
