
#include "led.h"
#include "system.h"

/**
	Initialize LED1 to LED4
**/
void LED_Init(void) 
{
	GPIO_InitTypeDef GPIO_InitStruct;

	HAL_GPIO_WritePin(LED_PORT, LED1 | LED2 | LED3 | LED4, GPIO_PIN_RESET);

	__HAL_RCC_GPIOB_CLK_ENABLE();

	GPIO_InitStruct.Pin = LED1 | LED2 | LED3 | LED4;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);
	
	LED_Write(LED1 | LED2 | LED3 | LED4, LED_OFF);
}




void LED_Write(uint16_t GPIO_Pin, GPIO_PinState PinState) 
{
	HAL_GPIO_WritePin(LED_PORT, GPIO_Pin, PinState);
}




void LED_Toogle(uint16_t GPIO_Pin) 
{
	HAL_GPIO_TogglePin(LED_PORT, GPIO_Pin);
}







void LED_Flash(uint16_t GPIO_Pin, uint8_t num, GPIO_PinState final_state)
{
	for (uint8_t i = 0; i < 2*num; i++)
	{
		HAL_GPIO_TogglePin(LED_PORT, GPIO_Pin);
		Delay_ms(500);
	}
	
	HAL_GPIO_WritePin(LED_PORT, GPIO_Pin, final_state);
}
