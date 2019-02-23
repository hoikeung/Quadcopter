
#ifndef __MOTOR_H__

#define __MOTOR_H__

#include "stm32f1xx_hal.h"

#define LIMIT(x, min, max) ( (x) < (min) ? (min) : ( (x) > (max) ? (max) : (x) ) )


void Motor_init(void);
void Motor_Set_Duty_Cycle(float *duty_cycle);

#endif
