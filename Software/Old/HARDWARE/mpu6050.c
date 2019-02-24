


#include "ALL_DATA.h"
#include "mpu6050.h"
#include "I2C.h"
#include "filter.h"
#include <string.h>
#include "LED.h"
#include "myMath.h"
#include "kalman.h"



#define	SMPLRT_DIV		0x19	
#define	CONFIGL			0x1A	
#define	GYRO_CONFIG		0x1B	
#define	ACCEL_CONFIG	0x1C	
#define	ACCEL_ADDRESS	0x3B
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42
#define	GYRO_XOUT_H		0x43
#define GYRO_ADDRESS  0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48
#define	PWR_MGMT_1		0x6B	
#define	WHO_AM_I		  0x75	
#define MPU6050_PRODUCT_ID 0x68
#define MPU6052C_PRODUCT_ID 0x72

//#define   MPU6050_is_DRY()      GPIO_ReadOutBit(HT_GPIOC, GPIO_PIN_0)
	#ifdef  	USE_I2C_HARDWARE
		
		#define MPU6050_ADDRESS 0xD0//0x68
	#else
		#define  MPU6050_ADDRESS 0xD0   
	#endif



int16_t MpuOffset[6] = {0};

static volatile int16_t *pMpu = (int16_t *)&MPU6050;



int8_t mpu6050_rest(void)
{
	if(IIC_Write_One_Byte(MPU6050_ADDRESS, PWR_MGMT_1, 0x80) == FAILED)
		return FAILED;	
	delay_ms(20);
	return SUCCESS;
}



int8_t MpuInit(void)
{
	uint8_t date = SUCCESS;
	do
	{
	date = IIC_Write_One_Byte(MPU6050_ADDRESS, PWR_MGMT_1, 0x80);	
	delay_ms(30);
	date += IIC_Write_One_Byte(MPU6050_ADDRESS, SMPLRT_DIV, 0x02); 
	date += IIC_Write_One_Byte(MPU6050_ADDRESS, PWR_MGMT_1, 0x03);
	date += IIC_Write_One_Byte(MPU6050_ADDRESS, CONFIGL, 0x03);  
	date += IIC_Write_One_Byte(MPU6050_ADDRESS, GYRO_CONFIG, 0x18);
	date += IIC_Write_One_Byte(MPU6050_ADDRESS, ACCEL_CONFIG, 0x09);
	}
	while(date != SUCCESS);
	date = IIC_Read_One_Byte(MPU6050_ADDRESS, 0x75);
	if(date!= MPU6050_PRODUCT_ID)
		return FAILED;
	else 
		MpuGetOffset();
		return SUCCESS;
}




#define  Gyro_Read() IIC_read_Bytes(MPU6050_ADDRESS, 0X3B,buffer,6)
#define  Acc_Read() IIC_read_Bytes(MPU6050_ADDRESS, 0x43,&buffer[6],6)

void MpuGetData(void) 
{
	  uint8_t i;
    uint8_t buffer[12];

		Gyro_Read();
	  Acc_Read();

		for(i=0;i<6;i++)
		{
			pMpu[i] = (((int16_t)buffer[i<<1] << 8) | buffer[(i<<1)+1])-MpuOffset[i];		
			if(i < 3)
			{
				{
					static struct _1_ekf_filter ekf[3] = {{0.02,0,0,0,0.001,0.543},{0.02,0,0,0,0.001,0.543},{0.02,0,0,0,0.001,0.543}};	
					kalman_1(&ekf[i],(float)pMpu[i]);  
					pMpu[i] = (int16_t)ekf[i].out;
				}
		}
		if(i > 2)
		{	
			uint8_t k=i-3;
			const float factor = 0.15f; 		
			static float tBuff[3];		

			pMpu[i] = tBuff[k] = tBuff[k] * (1 - factor) + pMpu[i] * factor;                
		}
	}
}


void MpuGetOffset(void) 
{
	int32_t buffer[6]={0};
	int16_t i;  
	uint8_t k=30;
	const int8_t MAX_GYRO_QUIET = 5;
	const int8_t MIN_GYRO_QUIET = -5;	
/*           wait for calm down    	                                                          */
	int16_t LastGyro[3] = {0};
	int16_t ErrorGyro[3];	
	/*           set offset initial to zero    		*/
	
	memset(MpuOffset,0,12);
	MpuOffset[2] = 8192;   //set offset from the 8192  
	
	TIM_ITConfig( 
		TIM3, 
		TIM_IT_Update ,
		DISABLE  
		);	
	while(k--)
	{
		do
		{
			delay_ms(10);
			MpuGetData();
			for(i=0;i<3;i++)
			{
				ErrorGyro[i] = pMpu[i+3] - LastGyro[i];
				LastGyro[i] = pMpu[i+3];	
			}			
		}while ((ErrorGyro[0] >  MAX_GYRO_QUIET )|| (ErrorGyro[0] < MIN_GYRO_QUIET)
					||(ErrorGyro[1] > MAX_GYRO_QUIET )|| (ErrorGyro[1] < MIN_GYRO_QUIET)
					||(ErrorGyro[2] > MAX_GYRO_QUIET )|| (ErrorGyro[2] < MIN_GYRO_QUIET)
						);
	}	


	for(i=0;i<356;i++)
	{		
		MpuGetData();
		if(100 <= i)
		{
			uint8_t k;
			for(k=0;k<6;k++)
			{
				buffer[k] += pMpu[k];
			}
		}
	}

	for(i=0;i<6;i++)
	{
		MpuOffset[i] = buffer[i]>>8;
	}
	TIM_ITConfig(  
		TIM3,
		TIM_IT_Update ,
		ENABLE 
		);
}

