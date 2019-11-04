#ifndef __DRIVER_H
#define __DRIVER_H
#include "sys.h"

#define B1_IN1 PAout(4)
#define B1_IN2 PCout(15)
#define B1_IN3 PCout(14)
#define B1_IN4 PCout(13)

#define B2_IN1 PBout(2)
#define B2_IN2 PAout(5)
#define B2_IN3 PBout(8)
#define B2_IN4 PBout(5)

void driver_init(void);

void Set_GoForward_ALL(void);
void Set_Break_All(void);
void Set_GoBack_ALL(void);
void Set_GoBack_M2(void);
void Set_GoBack_M4(void);
void Set_GoBack_M1(void);
void Set_GoBack_M3(void);
void Set_GoForward_M2(void);
void Set_GoForward_M4(void);
void Set_GoForward_M1(void);
void Set_GoForward_M3(void);
void Set_Shift_R(void);
void Set_Shift_L(void);
void Set_Turn_L(void);
void Set_Turn_R(void);
void Set_Break_FL(void);
void Set_Break_BL(void);
void Set_Break_FR(void);
void Set_Break_BR(void);

#endif 
