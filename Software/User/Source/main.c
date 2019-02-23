
#include "led.h"
#include "system.h"
#include "i2c.h"
#include "mpu6050.h"
#include "stm32f1xx_hal.h"
#include "my_math.h"
#include "nrf24l01.h"
#include "timer2.h"
#include "motor.h"
#include "control.h"


int main(void) 
{
	System_Clock_Config();
	
	LED_Init();
	MPU6050_Init();
	Motor_init();
	
	Calculate_Error_Angle();
	Calculate_Angle();
	
	NRF24L01_Init();
	
	LED_Flash(LED1 | LED2 | LED3 | LED4, 1, LED_OFF);
	
	Delay_ms(100);
	
	while (1) 
	{
		
		NRF24L01_Receive_Data();
		if (NRF24L01_Check_RX_Data())
		{
			Calculate_Elapsed_Time();
			Calculate_Angle();
			Calculate_PID();
			Control();
		}
		
		
		
//		Calculate_Elapsed_Time();
//		Calculate_Angle();
//		Calculate_PID();
//		Control();
//		
		
		Delay_ms(5);
	}
}
