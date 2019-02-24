
#include "pid.h"
#include "myMath.h"	


void pidRest(PidObject **pid,const uint8_t len)
{
	uint8_t i;
	for(i=0;i<len;i++)
	{
	  	pid[i]->integ = 0;
	    pid[i]->prevError = 0;
	    pid[i]->out = 0;
		pid[i]->offset = 0;
	}
}


void pidUpdate(PidObject* pid,const float dt)
{
	 float error;
	 float deriv;
	
    error = pid->desired - pid->measured; 

    pid->integ += error * dt;	 
	
	//  pid->integ = LIMIT(pid->integ,pid->IntegLimitLow,pid->IntegLimitHigh); 

    deriv = (error - pid->prevError)/dt;  
	
    pid->out = pid->kp * error + pid->ki * pid->integ + pid->kd * deriv;
	
		//pid->out = LIMIT(pid->out,pid->OutLimitLow,pid->OutLimitHigh); 
		
    pid->prevError = error;  
		
}



void CascadePID(PidObject* pidRate,PidObject* pidAngE,const float dt) 
{	 
	pidUpdate(pidAngE,dt);  
	pidRate->desired = pidAngE->out;
	pidUpdate(pidRate,dt);   	
}



