
#include "ALL_DEFINE.h"


volatile uint32_t SysTick_count; 
_st_Mpu MPU6050;  
_st_Mag AK8975;
_st_AngE Angle;    
_st_Remote Remote; 


_st_ALL_flag ALL_flag; 


PidObject pidRateX; 
PidObject pidRateY;
PidObject pidRateZ;

PidObject pidPitch; 
PidObject pidRoll;
PidObject pidYaw;

PidObject pidHeightRate;
PidObject pidHeightHigh;

void pid_param_Init(void); 


void ALL_Init(void)
{


	IIC_Init();            
	
	pid_param_Init();      
	 
	LEDInit();             

	MpuInit();         
	
	USART3_Config();       

	NRF24L01_init();			
	
	TIM2_PWM_Config();		
	
	TIM3_Config();				
}



void pid_param_Init(void)
{
	

	pidRateX.kp = 2.0f;
	pidRateY.kp = 2.0f;
	pidRateZ.kp = 4.0f;
	
	pidRateX.ki = 0.0f;
	pidRateY.ki = 0.0f;
	pidRateZ.ki = 0.0f;	
	
	pidRateX.kd = 0.08f;
	pidRateY.kd = 0.08f;
	pidRateZ.kd = 0.5f;	
	
	pidPitch.kp = 7.0f;
	pidRoll.kp = 7.0f;
	pidYaw.kp = 7.0f;	

	
//	pidRateX.kp = 2.0f;
//	pidRateY.kp = 2.0f;
//	pidRateZ.kp = 4.0f;
//	
//	pidRateX.ki = 0.0f;
//	pidRateY.ki = 0.0f;
//	pidRateZ.ki = 0.0f;	
//	
//	pidRateX.kd = 0.28f;
//	pidRateY.kd = 0.28f;
//	pidRateZ.kd = 0.4f;	
//	
//	pidPitch.kp = 7.0f;
//	pidRoll.kp = 7.0f;
//	pidYaw.kp = 7.0f;	

}










