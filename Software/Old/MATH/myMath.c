
 
#include "myMath.h"
#include <math.h>
#include "sys.h"

const float M_PI = 3.1415926535;
const float RtA = 57.2957795f;
const float AtR = 0.0174532925f;
const float Gyro_G = 0.03051756f*2;	 	
const float Gyro_Gr = 0.0005326f*2;  



#ifndef TAPYOR
float sine(float x)       
{
	const float Q = 0.775;
	const float P = 0.225;
	const float B =  4 / M_PI;  
	const float C = -4 /(M_PI*M_PI);
	float y = B * x + C * x * fabs(x); 
	return (Q * y + P * y * fabs(y));
}
#else 
//sinx= x- x^3/3! + x^5/5! - x^7/7!+ x^9/9! . =?(-1)^n x^(2n+1)/(2n+1)!
float sine(float x)
{
	float t=x;
	float result = x;
	float X2 = x*x;
	uint8_t cnt = 1;

	do
	{
		t=-t;
		t *= X2;
		result += t/((cnt<<1)+1);
		cnt++;
	}	while(cnt<5);//6½×

	return result;
} 
#endif




float cosine(float x)
{
	return sine(x+M_PI/2);
}



float arctan(float x) 
{
	float t = x;
	float result = 0;
	float X2 = x * x;
	unsigned char cnt = 1;
	do
	{
		result += t / ((cnt << 1) - 1);
		t = -t;
		t *= X2;
		cnt++;
	}while(cnt <= 6);
	return result;
}





const float PI_2 = 1.570796f;
float arcsin(float x)  
{
	float d=1;
	float t=x;
	unsigned char cnt = 1;
	float result = 0;	
	float X2 = x*x;
	
	if (x >= 1.0f) 
	{
		return PI_2;
	}
	if (x <= -1.0f) 
	{
		return -PI_2;
	}
	do
	{
		result += t / (d * ((cnt << 1) - 1));
		t *= X2 * ((cnt << 1) - 1);//
		d *= (cnt << 1);//2 4 6 8 10 ...
		cnt++;
	}while(cnt <= 6);

	return result;
}


float safe_asin(float v)
{
    if (isnan(v)) {
        return 0.0;
    }
    if (v >= 1.0f) {
        return M_PI/2;
    }
    if (v <= -1.0f) {
        return -M_PI/2;
    }
    return asinf(v);
}




float Q_rsqrt(float number)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;
 
	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;                      
	i  = 0x5f3759df - ( i >> 1 );               
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   
	return y;
} 


void array_astrict(int16_t *array,int16_t lower,int16_t upper)
{
   int16_t length = sizeof(array); 
	 uint16_t i = 0;
   for(i=0;i<length;i++)
   {
     if(*(array+i)<lower)  *(array+i) = lower;
     else if(*(array+i)>upper)  *(array+i) = upper;
   } 
}

void array_assign(int16_t *array,int16_t value)
{
   uint16_t length = sizeof(array); 
	 uint16_t i=0;
   for(i=0;i<length;i++)
   {
     *(array+i) = value;
   } 
}

float data_limit(float data,float toplimit,float lowerlimit)
{
  if(data > toplimit)  data = toplimit;
  else if(data < lowerlimit) data = lowerlimit;
	return data;
}


float VariableParameter(float error)
{
	float  result = 0;
	
	if(error < 0)
	{
	   error = -error;
	}
  if(error >0.6f)
	{
	   error = 0.6f;
	}
	result = 1 - 1.667f * error;
	if(result < 0)
	{
	   result = 0;
	}
	return result;
}


float middle_3(float input) 
{ 

  int a,b,c,t; 


  if(a<b)

  { 

     t=a;a=b;b=t; 

  } 

 if(b<c)//9 8 7 

 { 

  t=b;b=c;c=t;      

 } 

 if(a<b)//9 8 7 

 { 

  t=a;a=b;b=t; 

 } 

 return b; 

}




float my_deathzoom_2(float x,float zoom)
{
	float t;
	
	if( x> -zoom && x < zoom )
	{
		t = 0;
	}
	else
	{
		t = x;
	}
  return (t);
}


float my_deathzoom(float x,float zoom)
{
	float t;
	if(x>0)
	{
		t = x - zoom;
		if(t<0)
		{
			t = 0;
		}
	}
	else
	{
		t = x + zoom;
		if(t>0)
		{
			t = 0;
		}
	}
  return (t);
}
