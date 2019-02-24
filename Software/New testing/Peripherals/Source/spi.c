
#include "spi.h"


SPI_HandleTypeDef hspi2;

void SPI2_Init(void)
{
	hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
	
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
	
	__HAL_SPI_ENABLE(&hspi2);
}

void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{

  GPIO_InitTypeDef GPIO_InitStruct;
	
  if(hspi->Instance==SPI2)
  {
    /* 
			Peripheral clock enable 
		*/
    __HAL_RCC_SPI2_CLK_ENABLE();
  
    /**
			SPI2 GPIO Configuration    
			PB13     ------> SPI2_SCK
			PB14     ------> SPI2_MISO
			PB15     ------> SPI2_MOSI 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  }

}

void SPI2_Transmit(uint8_t *pData, uint16_t size)
{
	HAL_StatusTypeDef state;
	state = HAL_SPI_Transmit(&hspi2, pData, size, 100); //Timeout = 100ms
	
	if (state != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
}

void SPI2_Receive(uint8_t *pData, uint16_t size)
{
	HAL_StatusTypeDef state;
	state = HAL_SPI_Receive(&hspi2, pData, size, 100); ////Timeout = 100ms
	
	if (state != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
}

