
#include "control.h"
#include "nrf24l01.h"
#include "motor.h"
#include "my_math.h"
#include "timer2.h"

uint8_t contorl_rx_data[32];

float motor_control_input[4] = {0, 0, 0, 0};


void Control(void)
{
	
	NRF24L01_Get_RX_Data(&contorl_rx_data[0]);
	PID_Result pid = Get_PID_Result();
	
	if (contorl_rx_data[6] > 0x05)
	{
		motor_control_input[0] = motor_control_input[0] + pid.roll_angle - pid.pitch_angle + 1;
		motor_control_input[1] = motor_control_input[1] + pid.roll_angle + pid.pitch_angle + 1;
		motor_control_input[2] = motor_control_input[2] - pid.roll_angle + pid.pitch_angle + 1;
		motor_control_input[3] = motor_control_input[3] - pid.roll_angle - pid.pitch_angle + 1;
		
	}
	else if ((contorl_rx_data[6] < 0x05) & (contorl_rx_data[6] > 0))
	{
//		motor_control_input[0] = motor_control_input[0] + pid.roll_angle - pid.pitch_angle - 1;
//		motor_control_input[1] = motor_control_input[1] + pid.roll_angle + pid.pitch_angle - 1;
//		motor_control_input[2] = motor_control_input[2] - pid.roll_angle + pid.pitch_angle - 1;
//		motor_control_input[3] = motor_control_input[3] - pid.roll_angle - pid.pitch_angle - 1;
		
			motor_control_input[0] = 0;
			motor_control_input[1] = 0;
			motor_control_input[2] = 0;
			motor_control_input[3] = 0;
	}
	else 
	{
		motor_control_input[0] = motor_control_input[0] + pid.roll_angle - pid.pitch_angle;
		motor_control_input[1] = motor_control_input[1] + pid.roll_angle + pid.pitch_angle;
		motor_control_input[2] = motor_control_input[2] - pid.roll_angle + pid.pitch_angle;
		motor_control_input[3] = motor_control_input[3] - pid.roll_angle - pid.pitch_angle;
	}
	
	for (uint8_t i = 0; i < 4; i++)
	{
		motor_control_input[i] = LIMIT(motor_control_input[i], 0, TIMER2_PERIOD_VALUE);
	}
	

	
	Motor_Set_Duty_Cycle(&motor_control_input[0]);
}




