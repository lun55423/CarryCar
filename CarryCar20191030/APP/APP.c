#include "APP.h"
#include "driver.h" 
#include "pwm.h"
#include "usart2.h"
//int16_t theta;
#define abs(x) (x>0)?(x):(-x)
/*
**					控制流程及PID调用写在APP_Init里
**
**/

extern u32 time_cnt;
u16 a=0,b=0,c=0,d=0;
void Motor_Control(u16 m1_tar,u16 m2_tar,u16 m3_tar,u16 m4_tar)
{
	Wheel_PID(Tim2_Capture_1.freq,m1_tar,&pid_m1);
	Wheel_PID(Tim2_Capture_3.freq,m2_tar,&pid_m2);
	Wheel_PID(Tim3_Capture_3.freq,m3_tar,&pid_m3);	
	Wheel_PID(Tim3_Capture_1.freq,m4_tar,&pid_m4);

	M1_OUT(pid_m1.output);
	M2_OUT(pid_m2.output);
	M3_OUT(pid_m3.output);
	M4_OUT(pid_m4.output);
}
u32 time_cc = 0;
static u8 is_first = 0;
static u16 parallel[2] = {0};
u32 last_time = 0;
u8 flag_1 = 0;
void APP_Init(void)
{
	parallel[0] = parallel[1];
	parallel[1] = Openmv.theta_parallel;
	if((abs(parallel[1] - parallel[0]) > 50) && is_first == 0)
	{
		Openmv.line_num++;
		is_first = 1;
		last_time = time_cnt;
		parallel[0] = parallel[1] = 0;
		Openmv.theta_parallel = 0;
	}
	else if((abs(parallel[1] - parallel[0]) > 50) && is_first == 1)
	{
//		if(flag_1 == 0)
//		{
//			last_time = time_cnt;
//			flag_1 = 1;
//		}
		if((time_cnt - last_time) > 900) 
		{
			Openmv.line_num++;
			last_time = time_cnt;
			parallel[1] = parallel[0] = 0;
			Openmv.theta_parallel = 0;
		}else if((time_cnt - last_time) < 300)
		{
			last_time = time_cnt;
			parallel[1] = parallel[0] = 0;
			Openmv.theta_parallel = 0;
		}
		
			
		//parallel[1] = parallel[0] = 0;
		//is_first = 0;
//		if(is_first == 0) 
//		{
//			time_cc = time_cnt;
//			is_first = 1;
//		}
//		if( (time_cnt-time_cc) > 500) 
//		{
//			Openmv.line_num++;
//			is_first = 0;
//		}
	}
	
	PID_General(90,Openmv.theta_vertical,&pid_line_theta);
	//PID_General(Openmv.x,80,&pid_line_x);
	Set_GoForward_ALL();
	Motor_Control( pid_line_x.output-pid_line_theta.output + 1000,\
								 -pid_line_x.output+pid_line_theta.output + 1000,\
								 pid_line_x.output+pid_line_theta.output + 1000,\
								 -pid_line_x.output-pid_line_theta.output + 1000);  
	

}



//	time_cnt = 0;
//	if(time_cnt < 300)
//	{
//		Set_GoForward_ALL();	
//		Motor_Control(600,600,600,600);
//	}
//	else if(time_cnt <400)
//	{
//		Motor_Control(0,0,0,0);
//	}
//	else if(time_cnt<700)
//	{
//		Set_Shift_R();
//		Motor_Control(1000,1000,1000,1000);
//	}
//	else if(time_cnt<1000)
//	{
//		Motor_Control(0,0,0,0);
//	}else if(time_cnt<1100)
//	{
//		Set_Turn_L();
//		Motor_Control(1000,1000,1000,1000);
//	}else if(time_cnt<1300)
//	{
//		Set_GoBack_ALL();
//		Motor_Control(600,600,600,600);
//	}else 
//	{
//		Set_Break_All();
//		Motor_Control(0,0,0,0);
//	}
		

	
	//Motor_Control(a,b,c,d);
	//u16 offset = 500;
	 //Motor_Control(1000,1000,1000,1000);
	//PID_General(Openmv.theta_vertical,90,&pid_line_theta);
	//Motor_Control(pid_line_theta.output+offset,pid_line_theta.output+offset,pid_line_theta.output+offset,pid_line_theta.output+offset);
	
