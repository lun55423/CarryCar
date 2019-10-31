#ifndef __PID_H
#define __PID_H
#include "sys.h"

#define 		TAR_MAX 			14399.0f       
#define 		ESC_MAX 			14399.0f
#define   	ENTER_MAX		  20000.0f						
#define 		INTE_MAX			20000.0f			//积分饱和阈值
#define			START_VAL			0.0f

/*
**
**					属性参量结构体，使用时请声明
**/
typedef __packed struct
{
	float k_p;
	float k_i;
	float k_d;
	float error[2];
  float inte;
  float dire;
  u16 output;	
}PID_GENERAL;

extern PID_GENERAL pid_m1;
extern PID_GENERAL pid_m2;
extern PID_GENERAL pid_m3;
extern PID_GENERAL pid_m4;

typedef __packed struct
{
	float k_p;
	float k_i;
	float k_d;
	float error[2];
  float inte;
  float dire;
  s32 output;	
	s32 input_min;
  s32 input_max;
  s32 output_min;
  s32 output_max;
}PID_LINE;

extern PID_LINE pid_line_theta;
extern PID_LINE pid_line_x;

void Wheel_PID(u16 current,u16 target,PID_GENERAL *pid);
float PID_General(float target,float current,PID_LINE *pid);
float sensor(void);
void pid_init(PID_LINE *pid);

#endif
