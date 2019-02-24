
#include "ALL_DATA.h"
#include "nrf24l01.h"
#include "control.h"
#include <math.h>
#include "myMath.h"
#include "LED.h"
#include "Remote.h"


#define SUCCESS 0
#undef FAILED
#define FAILED  1




uint8_t RC_rxData[32];
void remote_unlock(void);	
void RC_Analy(void)  
{
	static uint16_t cnt;

	if(NRF24L01_RxPacket(RC_rxData)==SUCCESS)
	{ 	

		uint8_t i;
		uint8_t CheckSum=0;
		cnt = 0;
		for(i=0;i<31;i++)
		{
			CheckSum +=  RC_rxData[i];
		}
		if(RC_rxData[31]==CheckSum && RC_rxData[0]==0xAA && RC_rxData[1]==0xAF) 
		{
			  Remote.roll = ((uint16_t)RC_rxData[4]<<8) | RC_rxData[5];  
				LIMIT(Remote.roll,1000,2000);
				Remote.pitch = ((uint16_t)RC_rxData[6]<<8) | RC_rxData[7]; 
				LIMIT(Remote.pitch,1000,2000);
				Remote.thr = 	((uint16_t)RC_rxData[8]<<8) | RC_rxData[9];
				LIMIT(Remote.thr,1000,2000);
				Remote.yaw =  ((uint16_t)RC_rxData[10]<<8) | RC_rxData[11];  
				LIMIT(Remote.yaw,1000,2000);
				Remote.AUX1 =  ((uint16_t)RC_rxData[12]<<8) | RC_rxData[13];   
				LIMIT(Remote.AUX1,1000,2000);
				Remote.AUX2 =  ((uint16_t)RC_rxData[14]<<8) | RC_rxData[15]; 
				LIMIT(Remote.AUX2,1000,2000);
				Remote.AUX3 =  ((uint16_t)RC_rxData[16]<<8) | RC_rxData[17]; 
				LIMIT(Remote.AUX1,1000,2000);
				Remote.AUX4 =  ((uint16_t)RC_rxData[18]<<8) | RC_rxData[19];    
				LIMIT(Remote.AUX2,1000,2000);		
				
				{
							const float roll_pitch_ratio = 0.04f;
							const float yaw_ratio =  0.0015f;    
					
							pidPitch.desired =-(Remote.pitch-1500)*roll_pitch_ratio;	
							pidRoll.desired = -(Remote.roll-1500)*roll_pitch_ratio;
					    if(Remote.yaw>1820)
							{
								pidYaw.desired += 0.75f;	
							}
							else if(Remote.yaw <1180)
							{
								pidYaw.desired -= 0.75f;	
							}						
				}
				remote_unlock();
			
		}
  }
	else
	{
		cnt++;
		if(cnt>800)
		{
			cnt = 0;
			ALL_flag.unlock = 0; 
			NRF24L01_init();
		}
	}
}


void remote_unlock(void)
{
	volatile static uint8_t status=WAITING_1;
	static uint16_t cnt=0;

	if(Remote.thr<1200 &&Remote.yaw<1200)                  
	{
		status = EXIT_255;
	}
	
	switch(status)
	{
		case WAITING_1:
			if(Remote.thr<1150)        
			{			 
					 status = WAITING_2;				 
			}		
			break;
		case WAITING_2:
			if(Remote.thr>1800)          
			{		
						static uint8_t cnt = 0;
					 	cnt++;		
						if(cnt>5)
						{	
								cnt=0;
								status = WAITING_3;
						}
			}			
			break;
		case WAITING_3:
			if(Remote.thr<1150)          
			{			 
					 status = WAITING_4;				 
			}			
			break;			
		case WAITING_4:	              
				ALL_flag.unlock = 1;
				status = PROCESS_31;
				LED.status = AlwaysOn;									
				 break;		
		case PROCESS_31:	
				if(Remote.thr<1020)
				{
					if(cnt++ > 2000)                                
					{								
						status = EXIT_255;								
					}
				}
				else if(!ALL_flag.unlock)                 
				{
					status = EXIT_255;				
				}
				else					
					cnt = 0;
			break;
		case EXIT_255:
			LED.status = AllFlashLight;	                        
			cnt = 0;
			LED.FlashTime = 100; 		
			ALL_flag.unlock = 0;
			status = WAITING_1;
			break;
		default:
			status = EXIT_255;
			break;
	}
}



