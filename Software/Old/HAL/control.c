


#include "ALL_DATA.h" 
#include "ALL_DEFINE.h" 
#include "control.h"
#include "pid.h"

#undef NULL
#define NULL 0
#undef DISABLE 
#define DISABLE 0
#undef ENABLE 
#define ENABLE 1
#undef REST
#define REST 0
#undef SET 
#define SET 1 
#undef EMERGENT
#define EMERGENT 0

PidObject *(pPidObject[])={&pidRateX,&pidRateY,&pidRateZ,&pidRoll,&pidPitch,&pidYaw 
		,&pidHeightRate
		,&pidHeightHigh
};




void FlightPidControl(float dt)
{
	volatile static uint8_t status=WAITING_1;

	switch(status)
	{		
		case WAITING_1:
			if(ALL_flag.unlock)
			{
				status = READY_11;	
			}			
			break;
		case READY_11:  
			pidRest(pPidObject,8); 

			Angle.yaw = pidYaw.desired =  pidYaw.measured = 0;   
		
			status = PROCESS_31;
		
			break;			
		case PROCESS_31:
			
      pidRateX.measured = MPU6050.gyroX * Gyro_G; 
			pidRateY.measured = MPU6050.gyroY * Gyro_G;
			pidRateZ.measured = MPU6050.gyroZ * Gyro_G;
		
			pidPitch.measured = Angle.pitch; 
		  pidRoll.measured = Angle.roll;
			pidYaw.measured = Angle.yaw;
		
		 	pidUpdate(&pidRoll,dt);    
			pidRateX.desired = pidRoll.out; 
			pidUpdate(&pidRateX,dt);  

		 	pidUpdate(&pidPitch,dt);   
			pidRateY.desired = pidPitch.out;  
			pidUpdate(&pidRateY,dt);

			CascadePID(&pidRateZ,&pidYaw,dt);	
			break;
		case EXIT_255:  
			pidRest(pPidObject,8);
			status = WAITING_1;
		  break;
		default:
			status = EXIT_255;
			break;
	}
	if(ALL_flag.unlock == EMERGENT)
		status = EXIT_255;
}


int16_t motor[4];
#define MOTOR1 motor[0] 
#define MOTOR2 motor[1] 
#define MOTOR3 motor[2] 
#define MOTOR4 motor[3] 

void MotorControl(void)
{	
	volatile static uint8_t status=WAITING_1;
	
	
	if(ALL_flag.unlock == EMERGENT)
		status = EXIT_255;	
	switch(status)
	{		
		case WAITING_1: 	
			MOTOR1 = MOTOR2 = MOTOR3 = MOTOR4 = 0;  
			if(ALL_flag.unlock)
			{
				status = WAITING_2;
			}
		case WAITING_2: 
			if(Remote.thr>1100)
			{
				status = PROCESS_31;
			}
			break;
		case PROCESS_31:
			{
				int16_t temp;
				temp = Remote.thr -1000 + pidHeightRate.out; 
				if(Remote.thr<1020)													
				{
					MOTOR1 = MOTOR2 = MOTOR3 = MOTOR4=0;
					break;
				}
				MOTOR1 = MOTOR2 = MOTOR3 = MOTOR4 = LIMIT(temp,0,900); 

				MOTOR1 +=    + pidRateX.out - pidRateY.out - pidRateZ.out;
				MOTOR2 +=    + pidRateX.out + pidRateY.out + pidRateZ.out ;
				MOTOR3 +=    - pidRateX.out + pidRateY.out - pidRateZ.out;
				MOTOR4 +=    - pidRateX.out - pidRateY.out + pidRateZ.out;
			}	
			break;
		case EXIT_255:
			MOTOR1 = MOTOR2 = MOTOR3 = MOTOR4 = 0;  
			status = WAITING_1;	
			break;
		default:
			break;
	}
	
	
	TIM2->CCR1 = LIMIT(MOTOR1,0,1000); 
	TIM2->CCR2 = LIMIT(MOTOR2,0,1000);
	TIM2->CCR3 = LIMIT(MOTOR3,0,1000);
	TIM2->CCR4 = LIMIT(MOTOR4,0,1000);
} 
