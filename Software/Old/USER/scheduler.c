



#include "ALL_DEFINE.h"
#include "scheduler.h"


void TIM3_IRQHandler(void)  
{
	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) 
	{
		static uint8_t cnt_3ms = 0;
		static uint8_t cnt_6ms = 0;	 
		SysTick_count++;
		cnt_3ms++;
		cnt_6ms++;
		
		if(cnt_3ms == 1) 
		{
			cnt_3ms = 0;
			MpuGetData();
			RC_Analy();			
			FlightPidControl(0.003f);
			MotorControl();
		}		
		if(cnt_6ms == 2) 
		{
			cnt_6ms = 0;
			GetAngle(&MPU6050,&Angle,0.00626f);
		}		

		TIM_ClearITPendingBit(TIM3, TIM_IT_Update );   
	}
}


