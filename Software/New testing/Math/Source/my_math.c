

#include "my_math.h"


#include <stdio.h>
#include <math.h>


const float rad_to_deg = 180/3.141592654;

float accel_angle_error_x, accel_angle_error_y, gyro_angle_error_x, gyro_angle_error_y;
float total_angle_roll, total_angle_pitch;

float time_prev;
float time_current;
float elapsed_time;


const float roll_pid_kp = 0.0005;
const float roll_pid_ki = 0.0;
const float roll_pid_kd = 0.01;

const float pitch_pid_kp = 0.005;
const float pitch_pid_ki = 0.0;
const float pitch_pid_kd = 0.05;
	
PID_Result pid_result;

//float angle_roll, angle_pitch;

/*
	Calculate acceleration angle using euler equation
*/
void Calculate_Angle(void)
{

	MPU6050_Data mpu_value = MPU6050_Get_Data();
	MPU6050_Data mpu_offset = MPU6050_Get_Offset();
	
	float accel_angle_roll, accel_angle_pitch, gyro_angle_roll, gyro_angle_pitch;
	
	float accel_roll = mpu_value.accRoll/ACCEL_LSB;
	float accel_pitch = mpu_value.accPitch/ACCEL_LSB;
	float accel_yaw = mpu_value.accYaw/ACCEL_LSB;
	float accel_norm;
	
	float gyro_roll = mpu_value.gyroRoll/GYRO_LSB - gyro_angle_error_x;
	float gyro_pitch = mpu_value.gyroPitch/GYRO_LSB - gyro_angle_error_y;
	
	accel_norm = sqrt(accel_roll*accel_roll + accel_pitch*accel_pitch + accel_yaw*accel_yaw);
	accel_roll = accel_roll*accel_norm;
	accel_pitch = accel_pitch*accel_norm;
	accel_yaw = accel_yaw*accel_norm;

	/*
		Euler equation to calculate angle 
	*/
	accel_angle_roll = atan((accel_pitch)/sqrt(pow((accel_roll),2) + pow((accel_yaw),2)))*rad_to_deg - accel_angle_error_x;
	accel_angle_pitch = atan(-1*(accel_roll)/sqrt(pow((accel_pitch),2) + pow((accel_yaw),2)))*rad_to_deg - accel_angle_error_y;
	
	/*
		Apply complementary filter to the angle
	*/
	//gyro_angle_roll = gyro_roll*elapsed_time;
	//gyro_angle_pitch = gyro_pitch*elapsed_time;
	gyro_angle_roll = gyro_roll*0.005;
	gyro_angle_pitch = gyro_pitch*0.005;
	total_angle_roll = 1*(total_angle_roll + gyro_angle_roll) + 0*accel_angle_roll;
	total_angle_pitch = 1*(total_angle_pitch + gyro_angle_pitch) + 0*accel_angle_pitch;
}





void Calculate_Error_Angle(void)
{
	MPU6050_Data mpu_offset = MPU6050_Get_Offset();
	
	float accel_x = mpu_offset.accRoll/ACCEL_LSB;
	float accel_y = mpu_offset.accPitch/ACCEL_LSB;
	float accel_z = mpu_offset.accYaw/ACCEL_LSB;
	
	accel_angle_error_x = atan((accel_y)/sqrt(pow((accel_x),2) + pow((accel_z),2)))*rad_to_deg;
	accel_angle_error_y = atan(-1*(accel_x)/sqrt(pow((accel_y),2) + pow((accel_z),2)))*rad_to_deg;
	
	gyro_angle_error_x = mpu_offset.gyroRoll/GYRO_LSB;
	gyro_angle_error_y = mpu_offset.gyroPitch/GYRO_LSB;
	
	time_prev = HAL_GetTick(); // initialize time
}





void Calculate_PID(void)
{
	float roll_desired_angle = 0;
	float pitch_desired_angle = 0;
	
	float roll_angle_error = roll_desired_angle - total_angle_roll;
	float pitch_angle_error = pitch_desired_angle - total_angle_pitch;
	
	
	
	static float roll_angle_error_previous;
	static float pitch_angle_error_previous;
	
	float roll_adjusted_p, roll_adjusted_i, roll_adjusted_d, pitch_adjusted_p, pitch_adjusted_i, pitch_adjusted_d;
	
	//Get ajusted P value
	roll_adjusted_p = roll_pid_kp*roll_angle_error;
	pitch_adjusted_p = pitch_pid_kp*pitch_angle_error;
	
	//Get adjusted I value when the error is less than +-3
	if ((roll_angle_error < 3) & (roll_angle_error > -3)) roll_adjusted_i = roll_pid_ki*roll_angle_error;
	if ((pitch_angle_error < 3) & (pitch_angle_error > -3)) pitch_adjusted_i = pitch_pid_ki*pitch_angle_error;
	
	//Get adjusted D value
	roll_adjusted_d = roll_pid_kd*((roll_angle_error - roll_angle_error_previous)/elapsed_time);
	pitch_adjusted_d = pitch_pid_kd*((pitch_angle_error - pitch_angle_error_previous)/elapsed_time);
	
	pid_result.roll_angle = roll_adjusted_p + roll_adjusted_i + roll_adjusted_d;
	pid_result.pitch_angle = pitch_adjusted_p + pitch_adjusted_i + pitch_adjusted_d;
	
	roll_angle_error_previous = roll_angle_error;
	pitch_angle_error_previous = pitch_angle_error;

}


PID_Result Get_PID_Result(void)
{
	return pid_result;
}



void Calculate_Elapsed_Time(void)
{
	time_prev = time_current;
	time_current = HAL_GetTick();
	elapsed_time = (time_current - time_prev)/1000;
}



//typedef volatile struct {
//  float q0;
//  float q1;
//  float q2;
//  float q3;
//} Quaternion;

//void Calculate_Quaternions(const MPU6050_Data *pMpu,MPU6050_Data *pAngE, float dt)
//{
//	
//	volatile struct V{
//				float x;
//				float y;
//				float z;
//				} Gravity,Acc,Gyro,AccGravity;

//	static struct V GyroIntegError = {0};
//	static  float KpDef = 0.8f ;
//	static  float KiDef = 0.0003f;
//	static Quaternion NumQ = {1, 0, 0, 0};
//	float q0_t,q1_t,q2_t,q3_t;
//  float NormAcc;	
//	float NormQuat; 
//	float HalfTime = dt * 0.5f;
//	

//	Gravity.x = 2*(NumQ.q1 * NumQ.q3 - NumQ.q0 * NumQ.q2);								
//	Gravity.y = 2*(NumQ.q0 * NumQ.q1 + NumQ.q2 * NumQ.q3);						  
//	Gravity.z = 1-2*(NumQ.q1 * NumQ.q1 + NumQ.q2 * NumQ.q2);	

//	NormAcc = Q_rsqrt(squa(MPU6050.accX)+ squa(MPU6050.accY) +squa(MPU6050.accZ));
//	
//  Acc.x = pMpu->accX * NormAcc;
//  Acc.y = pMpu->accY * NormAcc;
//  Acc.z = pMpu->accZ * NormAcc;	

//	AccGravity.x = (Acc.y * Gravity.z - Acc.z * Gravity.y);
//	AccGravity.y = (Acc.z * Gravity.x - Acc.x * Gravity.z);
//	AccGravity.z = (Acc.x * Gravity.y - Acc.y * Gravity.x);

//  GyroIntegError.x += AccGravity.x * KiDef;
//  GyroIntegError.y += AccGravity.y * KiDef;
//  GyroIntegError.z += AccGravity.z * KiDef;

//  Gyro.x = pMpu->gyroX * Gyro_Gr + KpDef * AccGravity.x  +  GyroIntegError.x;
//  Gyro.y = pMpu->gyroY * Gyro_Gr + KpDef * AccGravity.y  +  GyroIntegError.y;
//  Gyro.z = pMpu->gyroZ * Gyro_Gr + KpDef * AccGravity.z  +  GyroIntegError.z;		

//	q0_t = (-NumQ.q1*Gyro.x - NumQ.q2*Gyro.y - NumQ.q3*Gyro.z) * HalfTime;
//	q1_t = ( NumQ.q0*Gyro.x - NumQ.q3*Gyro.y + NumQ.q2*Gyro.z) * HalfTime;
//	q2_t = ( NumQ.q3*Gyro.x + NumQ.q0*Gyro.y - NumQ.q1*Gyro.z) * HalfTime;
//	q3_t = (-NumQ.q2*Gyro.x + NumQ.q1*Gyro.y + NumQ.q0*Gyro.z) * HalfTime;
//	
//	NumQ.q0 += q0_t;
//	NumQ.q1 += q1_t;
//	NumQ.q2 += q2_t;
//	NumQ.q3 += q3_t;

//	NormQuat = Q_rsqrt(squa(NumQ.q0) + squa(NumQ.q1) + squa(NumQ.q2) + squa(NumQ.q3));
//	NumQ.q0 *= NormQuat;
//	NumQ.q1 *= NormQuat;
//	NumQ.q2 *= NormQuat;
//	NumQ.q3 *= NormQuat;	
//	


//	{
//		 
//			#ifdef	YAW_GYRO
//			*(float *)pAngE = atan2f(2 * NumQ.q1 *NumQ.q2 + 2 * NumQ.q0 * NumQ.q3, 1 - 2 * NumQ.q2 *NumQ.q2 - 2 * NumQ.q3 * NumQ.q3) * RtA;  //yaw
//			#else
//				float yaw_G = pMpu->gyroZ * Gyro_G;
//				if((yaw_G > 3.0f) || (yaw_G < -3.0f))
//				{
//					pAngE->yaw  += yaw_G * dt;			
//				}
//			#endif
//			pAngE->pitch  =  asin(2 * NumQ.q0 *NumQ.q2 - 2 * NumQ.q1 * NumQ.q3) * RtA;						
//		
//			pAngE->roll	= atan2(2 * NumQ.q2 *NumQ.q3 + 2 * NumQ.q0 * NumQ.q1, 1 - 2 * NumQ.q1 *NumQ.q1 - 2 * NumQ.q2 * NumQ.q2) * RtA;	//PITCH 			
//	}
//}

