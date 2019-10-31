#ifndef __USART3_H
#define __USART3_H
#include "sys.h" 
void usart2_init(void);

typedef struct 
{
	u8 theta;
	u8 theta_vertical;
	u8 theta_parallel;
	u16 x;
	u16 y;
	s32 delta_x;
	s32 delta_y;
	s16 delta_theta;
	u8 line_num;
}OPENMV;

extern OPENMV Openmv;
#endif


