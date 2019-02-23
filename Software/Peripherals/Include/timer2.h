
#ifndef __TIMER2_H__

#define __TIMER2_H__

#include "stm32f1xx_hal.h"

#define TIMER2_PRESCALER_VALUE	0
#define TIMER2_PERIOD_VALUE	7199	//10Khz

void TIMER2_Init(void);
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* htim_pwm);
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim);
void TIMER2_Set_Duty_Cycle(uint32_t *ccr);

#endif
