
#include "ALL_DEFINE.h"




int main(void)
{	
	cycleCounterInit();  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	SysTick_Config(SystemCoreClock / 1000);	

	ALL_Init();

	while(1)
	{
		  ANTO_polling(); 
			PilotLED(); 		
	}
}










