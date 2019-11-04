#ifndef __CAP_H
#define __CAP_H
#include "sys.h"

//ÆµÂÊ
typedef struct 
{
	u8 state;
	u16 count_pre;
	u16 count_last;
	u16 freq;
	u16 test;
}CAPTURE;

extern CAPTURE Tim3_Capture_1;
extern CAPTURE Tim3_Capture_2;
extern CAPTURE Tim3_Capture_3;
extern CAPTURE Tim3_Capture_4;

void TIM3_Cap_Init(u16 arr,u16 psc);

u16 freq_limit(u16 freq);
#endif

