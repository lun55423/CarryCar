#ifndef __CAP2_H
#define __CAP2_H
#include "sys.h"
#include "capture.h"

void TIM2_Cap_Init(u16 arr,u16 psc);
extern CAPTURE Tim2_Capture_1;
extern CAPTURE Tim2_Capture_2;
extern CAPTURE Tim2_Capture_3;
extern CAPTURE Tim2_Capture_4;
#endif

