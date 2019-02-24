
#ifndef __MY_MATH_H__

#define __MY_MATH_H__

#include "mpu6050.h"


typedef struct
{
	float roll_angle;
	float pitch_angle;
	float roll_velocity;
	float pitch_velocity;
	float yaw_velocity;
} PID_Result;

void Calculate_Angle(void);
void Calculate_Error_Angle(void);
void Calculate_PID(void);
PID_Result Get_PID_Result(void);
void Calculate_Elapsed_Time(void);

/*
void Calculate_Quaternions(const MPU6050_Data *pMpu,MPU6050_Data *pAngE, float dt);
*/
#endif
