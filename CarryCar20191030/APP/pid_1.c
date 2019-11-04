#include "pid.h"
#include "string.h"

#define PID_M1_KP 50.00
#define PID_M1_KI 0.60
#define PID_M1_KD 0.00

#define PID_M2_KP 49.0
#define PID_M2_KI 0.50
#define PID_M2_KD 0.00

#define PID_M3_KP 49.0
#define PID_M3_KI 0.50
#define PID_M3_KD 0.00

#define PID_M4_KP 50.0
#define PID_M4_KI 0.60
#define PID_M4_KD 0.00

#define PID_LINE_THETA_KP 0.50
#define PID_LINE_THETA_KI 0.80
#define PID_LINE_THETA_KD 0.80

#define PID_LINE_X_KP 5.00
#define PID_LINE_X_KI 0.50
#define PID_LINE_X_KD 0.50

#define PID_INTE_MAX 400

PID_GENERAL pid_m1={PID_M1_KP,PID_M1_KI,PID_M1_KD,0};
PID_GENERAL pid_m2={PID_M2_KP,PID_M2_KI,PID_M2_KD,0};
PID_GENERAL pid_m3={PID_M3_KP,PID_M3_KI,PID_M3_KD,0};
PID_GENERAL pid_m4={PID_M4_KP,PID_M4_KI,PID_M4_KD,0};

PID_LINE pid_line_theta = {	PID_LINE_THETA_KP,\
														PID_LINE_THETA_KI,\
														PID_LINE_THETA_KD,\
														{0,0},\
														0,0,0,\
														0,180,\
														-400,400};
PID_LINE pid_line_x = { PID_LINE_X_KP,\
												PID_LINE_X_KI,\
												PID_LINE_X_KD,\
												{0,0},\
												0,0,0,\
												0,160,\
												-400,400};
/*
**								底盘电机PID
*/

/*当前的current为从openmv返回的数据theta即直线在镜头内的角度，theta+=90,当theta>180时，theta-=180,
如果最终的theta>90,线在镜头中右偏，M2加速，否则M1加速
*/
void Wheel_PID(u16 current,u16 target,PID_GENERAL *pid)
{
	volatile float 	Kp = pid->k_p,\
									Ki = pid->k_i,\
									Kd = pid->k_d;
	//输入限幅
	if (current > ENTER_MAX) { current = ENTER_MAX; }
	//if (current < 0) { current = 0; }
	
	if (target > TAR_MAX) { target = TAR_MAX; }
	//if (target < 0) { target = 0; }
	
	pid->error[0] = pid->error[1];
	pid->error[1] = target - current;
	
	pid->inte += pid->error[1];
	
	//抗积分饱和
	if (pid->inte > INTE_MAX) { pid->inte = INTE_MAX; }
	if (pid->inte < -INTE_MAX) { pid->inte = INTE_MAX; }
	
	pid->dire = pid->error[1] - pid->error[0];
	
	pid->output = pid->error[1] * Kp + pid->inte * Ki + pid->dire * Kd;
	
	if (pid->output > ESC_MAX) { pid->output = ESC_MAX; }
	//if (pid->output < 0) { pid->output = 0; }
	//输出限幅


}

float PID_General(float target,float current,PID_LINE *pid)
{
  if(target>pid->input_max){target=pid->input_max;}
  if(target<pid->input_min){target=pid->input_min;}

  pid->error[0] = pid->error[1];
  pid->error[1] = target - current;
		
  pid->inte += pid->error[1];
  pid->dire = pid->error[1] - pid->error[0];
	
	if (pid->inte > PID_INTE_MAX) { pid->inte = PID_INTE_MAX; }
	if (pid->inte < -PID_INTE_MAX) { pid->inte = -PID_INTE_MAX; }
	
  pid->output = pid->error[1] * pid->k_p + pid->inte * pid->k_i + pid->dire * pid->k_d; 
	
  if(pid->output > pid->output_max)	pid->output = pid->output_max;
  if(pid->output < pid->output_min)	pid->output = pid->output_min;
	
  return pid->output;
}

