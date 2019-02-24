


#include "stm32f10x.h"
#include "LED.h"
#include "ALL_DATA.h"
		 
#define fLED_H()  GPIOB->BSRR = GPIO_Pin_1
#define fLED_L()  GPIOB->BRR  = GPIO_Pin_1
#define fLED_Toggle()  GPIOB->ODR ^= GPIO_Pin_1

#define hLED_H()  GPIOB->BSRR = GPIO_Pin_2
#define hLED_L()  GPIOB->BRR  = GPIO_Pin_2
#define hLED_Toggle()  GPIOB->ODR ^= GPIO_Pin_2

		 
#define aLED_H()  GPIOB->BSRR = GPIO_Pin_8
#define aLED_L()  GPIOB->BRR  = GPIO_Pin_8
#define aLED_Toggle()  GPIOB->ODR ^= GPIO_Pin_8

#define bLED_H()  GPIOB->BSRR = GPIO_Pin_9
#define bLED_L()  GPIOB->BRR  = GPIO_Pin_9
#define bLED_Toggle()  GPIOB->ODR ^= GPIO_Pin_9


sLED LED = {300,AllFlashLight};  


void LEDInit(void)	
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	AFIO->MAPR = 0X02000000; 
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  , ENABLE);
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2|GPIO_Pin_8 | GPIO_Pin_9;		 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);	
}


void PilotLED() //flash 300MS interval
{
	static uint32_t LastTime = 0;

	if(SysTick_count - LastTime < LED.FlashTime)
	{

		return;
	}
	else
		LastTime = SysTick_count;
	switch(LED.status)
	{
		case AlwaysOff:    
			bLED_H();
			fLED_H();
			bLED_H();
			hLED_H();
			break;
		case AllFlashLight:				
			fLED_Toggle();			
			bLED_Toggle();
			hLED_Toggle();			
			aLED_Toggle();		
		  break;
		case AlwaysOn: 
			
		  bLED_L();
			fLED_L();
			aLED_L();
			hLED_L();
		  break;
		case AlternateFlash: 
			bLED_H();
			fLED_L();
			aLED_H();
			hLED_L();
			LED.status = AllFlashLight;
			break;
		case WARNING:
			fLED_L();
			hLED_L();
			bLED_Toggle();
			LED.FlashTime = 100;
			break;
		case DANGEROURS:
			bLED_L();
		  aLED_L();
			fLED_Toggle();
		  hLED_Toggle();
			LED.FlashTime = 70;
			break;
		default:
			LED.status = AlwaysOff;
			break;
	}
}




