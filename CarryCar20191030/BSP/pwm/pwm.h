#ifndef __PWM_H
#define __PWM_H
#include "sys.h"

#define M4_OUT(x) TIM_SetCompare2(TIM1,x)
#define M3_OUT(x) TIM_SetCompare1(TIM1,x)
#define M2_OUT(x) TIM_SetCompare4(TIM1,x)
#define M1_OUT(x) TIM_SetCompare3(TIM1,x)

void TIM1_PWM_Init(u16 arr,u16 psc);
void direction(unsigned char i);

#endif

