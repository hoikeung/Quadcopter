
#include "mpu6050.h"
#include "i2c.h"
#include "system.h"
#include "led.h"

#include <string.h>


//static MPU6050_Data MPU6050;
//static int16_t *pMPU6050 = &MPU6050.accX;
//static MPU6050_Data MPU6050_offset;
//static int16_t *pMPU6050_offset = &MPU6050_offset.accX;

MPU6050_Data MPU6050;
int16_t *pMPU6050 = &MPU6050.accRoll;
MPU6050_Data MPU6050_offset;
int16_t *pMPU6050_offset = &MPU6050_offset.accRoll;




const uint16_t MPU6050_register_init[NUM_INIT_REGISTER][2] = {
	{PWR_MGMT_1, 0x80}, //reset device
	{SMPLRT_DIV, 0x02}, //set sample rate
	{PWR_MGMT_1, 0x03}, //PLL with Z axis gyroscope reference
	{CONFIG, 0x00}, //set the low pass filter to 44Hz for accelerometer and 42Hz for gyroscope
	{GYRO_CONFIG, 0x08}, //set gyroscope full scale range to ± 500 °/s
	{ACCEL_CONFIG, 0x08}, //set the accelerometer full scale range to ± 4g
};













/*
	Initialize MPU6050
*/
void MPU6050_Init()
{
	uint16_t addr = MPU6050_ADDRESS << 1;
	
	I2C1_Init();
	
	/*
		Initialize register
	*/
	for (uint8_t i = 0; i < NUM_INIT_REGISTER; i++)
	{
		Delay_ms(10);
		I2C1_Write_With_Register(addr, MPU6050_register_init[i][0], (uint8_t *) &MPU6050_register_init[i][1], 1);
	}
	
	Delay_ms(10);
	MPU6050_Calculate_Offset();
}





















/*
	Get MPU6050 data,
	Read 6 acceleration and 6 gyroscope register
	Convert it to 3 axis acceleration and 3 axis gyroscope data 
	Save these data in pMPU6050
*/
void MPU6050_Read_Data_Register(void)
{
	uint8_t accel_buffer[6];
	uint8_t gyro_buffer[6];
	uint8_t *p_buffer;
	uint16_t addr;
	
	uint8_t gyro_data_offset = 3;
	uint16_t temp_accel_data;
	uint16_t temp_gyro_data;
	
	/*
		Read 6 acceleration register
		Send write command then read command to MPU6050
	*/
	p_buffer = &accel_buffer[0];
	addr = MPU6050_ADDRESS << 1;
	I2C1_Write_With_Register(addr, ACCEL_XOUT_H, NULL, 0);
	addr = addr + 1;
	I2C1_Read_Without_Register(addr, p_buffer, 6);
	
	/*
		Read 6 gyroscope register
		Send write command then read command to MPU6050
	*/
	p_buffer = &gyro_buffer[0];
	addr = MPU6050_ADDRESS << 1;
	I2C1_Write_With_Register(addr, GYRO_XOUT_H, NULL, 0);
	addr = addr + 1;
	I2C1_Read_Without_Register(addr, p_buffer, 6);
	
	/*
		Convert it to 3 axis acceleration and 3 axis gyroscope data 
		Save these data in pMPU6050
	*/
	for (uint8_t i = 0; i < 3; i++)
	{
		temp_accel_data = (accel_buffer[i*2] << 8) | accel_buffer[i*2 + 1];
		*(pMPU6050 + i) = temp_accel_data;
		
		temp_gyro_data = (gyro_buffer[i*2] << 8) | gyro_buffer[i*2 + 1];
		*(pMPU6050 + i + gyro_data_offset) = temp_gyro_data;
	}
}



MPU6050_Data MPU6050_Get_Data(void)
{
	MPU6050_Read_Data_Register();
	return MPU6050;
}




MPU6050_Data MPU6050_Get_Offset(void)
{
	return MPU6050_offset;
}





/*
	Calculate MPU6050 offset
*/
void MPU6050_Calculate_Offset(void)
{
	uint16_t buf_size = 1000;
	uint16_t dump_data_size = 100;
	int32_t offset_buffer[6] = {0, 0, 0, 0, 0, 0};
	
	/*
		Forget the first group of data
	*/
	for (uint16_t i = 0; i < dump_data_size; i++)
	{
		MPU6050_Read_Data_Register();
		Delay_ms(2);
	}
	
	/*
		Get offset data
	*/
	for (uint16_t i = 0; i < (buf_size-dump_data_size); i++)
	{
		MPU6050_Read_Data_Register();
		
		/*
			Save accelX, accelY, accelZ, gyroX, gyroY, gyroZ
		*/
		offset_buffer[0] += *(pMPU6050);
		offset_buffer[1] += *(pMPU6050 + 1);
		offset_buffer[2] += *(pMPU6050 + 2);
		offset_buffer[3] += *(pMPU6050 + 3);
		offset_buffer[4] += *(pMPU6050 + 4);
		offset_buffer[5] += *(pMPU6050 + 5);
		
		Delay_ms(2);

	}
	/*
		Calculate average offset
		Save in MPU6050_offset
	*/
	*(pMPU6050_offset) = offset_buffer[0] / (buf_size-dump_data_size);
	*(pMPU6050_offset + 1) = offset_buffer[1] / (buf_size-dump_data_size);
	*(pMPU6050_offset + 2) = offset_buffer[2] / (buf_size-dump_data_size);
	*(pMPU6050_offset + 3) = offset_buffer[3] / (buf_size-dump_data_size);
	*(pMPU6050_offset + 4) = offset_buffer[4] / (buf_size-dump_data_size);
	*(pMPU6050_offset + 5) = offset_buffer[5] / (buf_size-dump_data_size);
}

