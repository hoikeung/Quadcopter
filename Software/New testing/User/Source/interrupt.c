
#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "led.h"
#include "interrupt.h"
#include "MPU6050.h"

//volatile static uint32_t ms_count = 0;

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}




void HardFault_Handler(void)
{
  while (1)
  {
  }

}
