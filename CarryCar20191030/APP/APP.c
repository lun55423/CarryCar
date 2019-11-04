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
u16 delta_cnt = 0;
u32 last_cnt = 0;
char time_flag = 0;
char task_flag = 0;
char segment_flag = 0;
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
u8 ff = 0;
void APP_Init(void)
{
	if(time_flag==0)
	{
		last_cnt = time_cnt;
		time_flag = 1;
	}
	
	delta_cnt = time_cnt - last_cnt;
//	if(task_flag==0&&delta_cnt<520)
//	{
//		Set_GoForward_ALL();	
//		Motor_Control(600,600,600,600);
//		
//	}
//	else if(task_flag==0&&delta_cnt>520)
//	{
//		static u16 n0 = 0;
//		Set_Break_All();
//  	Motor_Control(0,0,0,0);
//		if(++n0==5000)
//		{
//		  task_flag++;
//			time_flag=0;
//		}
//	}
//	if(task_flag==1&&delta_cnt<580)
//	{
//		Set_Turn_R();	
//		Motor_Control(1000,1000,1000,1000);
//		
//	}
//	else if(task_flag==1&&delta_cnt>580)
//	{
//		static u16 n1 = 0;
//		Set_Break_All();
//  	Motor_Control(0,0,0,0);
//		if(++n1==5000)
//		{
//		  task_flag++;
//			time_flag=0;
//		}
//	}
	if(Openmv.line_parl<6&&task_flag==2)
	{
		if(segment_flag==0&&task_flag==0&&delta_cnt<500)
		{
		PID_General(90,Openmv.theta_vertical,&pid_line_theta);
		Set_GoForward_ALL();
		Motor_Control( pid_line_x.output-pid_line_theta.output + 400,\
								 -pid_line_x.output+pid_line_theta.output + 400,\
								 pid_line_x.output+pid_line_theta.output + 400,\
								 -pid_line_x.output-pid_line_theta.output + 400);
		}			
	  else if(segment_flag==0&&task_flag==0&&delta_cnt>500)
	  {
		  	Set_GoForward_ALL();
		    Motor_Control(300,200,200,200);
	  }
		else if(segment_flag==1)
		{
			segment_flag=0;
			time_flag=0;
		}
	}
	else
	{
		static u16 n2 = 0;
		Set_Break_All();
		Motor_Control(0,0,0,0);
		
	}
//		PID_General(90,Openmv.theta_parallel,&pid_line_x);
//		Set_Shift_L();
//		Motor_Control( pid_line_x.output + 400,\
//								 -pid_line_x.output + 400,\
//								 pid_line_x.output + 400,\
//								 -pid_line_x.output + 400);  

//	if(task_flag==0&&Openmv.line_parl<3)
//	{
//		Set_GoForward_ALL();
//		Motor_Control(200,200,200,200);
//		
//	}
//	else if(Openmv.line_parl>=3&&task_flag==0)
//	{
//		static u16 n0 = 0;
//		Set_Break_All();
//		Motor_Control(0,0,0,0);
//		n0++;
//		if(n0>=1000)
//		{
//			task_flag++;
//			time_flag = 0;
//		}
//		
//	}
//	else if(delta_cnt<550&&task_flag==1)
//	{
//		Set_Turn_R();
//		Motor_Control(1000,1000,1000,1000);
//	}
//	else if(delta_cnt>550&&task_flag==1)
//	{
//		Set_Break_All();
//		Motor_Control(0,0,0,0);
	//}
	
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


