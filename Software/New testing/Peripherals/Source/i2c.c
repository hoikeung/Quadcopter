
#include "i2c.h"
#include "system.h"

I2C_HandleTypeDef hi2c1;

/*
	Initialize I2C1
*/
void I2C1_Init(void)
{
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
}

void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{

  GPIO_InitTypeDef GPIO_InitStruct;
	
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
  if(hi2c->Instance==I2C1)
  {
    /*
			I2C1 GPIO Configuration    
			PB6     ------> I2C1_SCL
			PB7     ------> I2C1_SDA 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* 
			Peripheral clock enable 
		*/
    __HAL_RCC_I2C1_CLK_ENABLE();
  }

}

void I2C1_Read_Without_Register(uint16_t device_address, uint8_t *pData, uint16_t size)
{
	HAL_StatusTypeDef state;
	
	state = HAL_I2C_Master_Receive(&hi2c1, device_address, pData, size, 100); //Timeout = 100ms
	
	if (state != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
}

void I2C1_Write_With_Register(uint16_t device_address, uint16_t device_register, uint8_t *pData, uint16_t size)
{
	HAL_StatusTypeDef state;
	
	state = HAL_I2C_Mem_Write(&hi2c1, device_address, device_register, I2C_MEMADD_SIZE_8BIT, pData, size, 100); //Timeout = 100ms
	
	if (state != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
}
