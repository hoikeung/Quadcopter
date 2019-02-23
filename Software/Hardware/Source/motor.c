
#include "motor.h"
#include "timer2.h"



void Motor_init(void)
{
	TIMER2_Init();
}

/*
	Set motor duty cycle
	duty_cycle: 0 - 100
*/
void Motor_Set_Duty_Cycle(float *duty_cycle)
{
	uint32_t motor[4];
	int32_t ccr;
	
	for (uint8_t i = 0; i < 4; i++)
	{
		ccr = *(duty_cycle + i) / 100.0 * TIMER2_PERIOD_VALUE;
		//ccr = *(duty_cycle + i);
		motor[i] = LIMIT(ccr, 0, TIMER2_PERIOD_VALUE/2);
	}
	
	TIMER2_Set_Duty_Cycle(&motor[0]);
}


