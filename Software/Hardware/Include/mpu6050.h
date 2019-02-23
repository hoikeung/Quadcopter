
#ifndef __MPU6050_H__

#define __MPU6050_H__

#include "stm32f1xx_hal.h"

typedef struct{
	int16_t accRoll;
	int16_t accPitch;
	int16_t accYaw;
	int16_t gyroRoll;
	int16_t gyroPitch;
	int16_t gyroYaw;
}MPU6050_Data;

typedef enum {
	SELF_TEST_X	= 13,
	SELF_TEST_Y,
	SELF_TEST_Z,
	SELF_TEST_A,
	
	SMPLRT_DIV = 25,
	CONFIG,
	GYRO_CONFIG,
	ACCEL_CONFIG,
	
	ACCEL_XOUT_H = 59,
	ACCEL_XOUT_L,
	ACCEL_YOUT_H,
	ACCEL_YOUT_L,
	ACCEL_ZOUT_H,
	ACCEL_ZOUT_L,
	TEMP_OUT_H,
	TEMP_OUT_L,
	GYRO_XOUT_H,
	GYRO_XOUT_L,
	GYRO_YOUT_H,
	GYRO_YOUT_L, 
	GYRO_ZOUT_H, 
	GYRO_ZOUT_L,
	
	PWR_MGMT_1 = 107,
	PWR_MGMT_2,
}MPU6050_Register;

#define NUM_INIT_REGISTER	6

#define ACCL_X_OFFSET	287
#define ACCL_Y_OFFSET -427
#define ACCL_Z_OFFSET	18107
#define GYRO_X_OFFSET -187
#define GYRO_Y_OFFSET	-47
#define GYRO_Z_OFFSET -222

#define ACCEL_LSB	8192.0
#define GYRO_LSB	65.5

#define MPU6050_ADDRESS	0x68

void MPU6050_Init(void);
void MPU6050_Read_Data_Register(void);
MPU6050_Data MPU6050_Get_Data(void);
MPU6050_Data MPU6050_Get_Offset(void);
void MPU6050_Calculate_Offset(void);

#endif

