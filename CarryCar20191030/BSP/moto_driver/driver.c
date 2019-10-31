#include "driver.h"

void driver_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_8|GPIO_Pin_5; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_2|GPIO_Pin_8|GPIO_Pin_5);					
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_4|GPIO_Pin_5);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);

}

/*
**
**					以下是电机控制器方向控制引脚，具体使用参考手册
**/
void Set_GoForward_ALL(void)
{
	B1_IN1 = 0;
	B1_IN2 = 1;
	
	B1_IN3 = 0;
	B1_IN4 = 1;
	
	B2_IN1 = 1;
	B2_IN2 = 0;
	
	B2_IN3 = 1;
	B2_IN4 = 0;
}
void Set_Break_All(void)
{
	B1_IN1 = 0;
	B1_IN2 = 0;
	
	B2_IN1 = 0;
	B2_IN2 = 0;
	
	B1_IN3 = 0;
	B1_IN4 = 0;
	
	B2_IN3 = 0;
	B2_IN4 = 0;
}
void Set_GoBack_ALL(void)
{
	B1_IN1 = 1;
	B1_IN2 = 0;
	
	B1_IN3 = 1;
	B1_IN4 = 0;
	
	B2_IN1 = 0;
	B2_IN2 = 1;
	
	B2_IN3 = 0;
	B2_IN4 = 1;
}

void Set_GoBack_M2(void)
{
	B1_IN1 = 1;
	B1_IN2 = 0;
}

void Set_GoBack_M4(void)
{
	B2_IN1 = 0;
	B2_IN2 = 1;
}

void Set_GoBack_M1(void)
{
	B1_IN3 = 1;
	B1_IN4 = 0;
}

void Set_GoBack_M3(void)
{
	B2_IN3 = 0;
	B2_IN4 = 1;
}

void Set_GoForward_M2(void)
{
	B1_IN1 = 0;
	B1_IN2 = 1;
}

void Set_GoForward_M4(void)
{
	B2_IN1 = 1;
	B2_IN2 = 0;
}

void Set_GoForward_M1(void)
{
	B1_IN3 = 0;
	B1_IN4 = 1;
}

void Set_GoForward_M3(void)
{
	B2_IN3 = 1;
	B2_IN4 = 0;
}
void Set_Break_FL(void)
{
		B1_IN1 = 0;
		B1_IN2 = 0;
}
void Set_Break_BL(void)
{
		B1_IN3 = 0;
		B1_IN4 = 0;
}

void Set_Break_FR(void)
{
		B2_IN1 = 0;
		B2_IN2 = 0;
}

void Set_Break_BR(void)
{
		B2_IN3 = 0;
		B2_IN4 = 0;
}
void Set_Turn_R(void)
{
	Set_GoForward_M1();
	Set_GoForward_M4();
	Set_GoBack_M2();
	Set_GoBack_M3();
}
void Set_Turn_L(void)
{
	Set_GoForward_M2();
	Set_GoForward_M3();
	Set_GoBack_M1();
	Set_GoBack_M4();
}
void Set_Shift_L(void)
{
	Set_GoBack_M1();
	Set_GoForward_M4();
	Set_GoBack_M3();
	Set_GoForward_M2();
}
void Set_Shift_R(void)
{
	Set_GoBack_M4();
	Set_GoForward_M1();
	Set_GoBack_M2();
	Set_GoForward_M3();
}
