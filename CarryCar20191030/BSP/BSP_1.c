#include "BSP.h"
#include "driver.h"
void BSP_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	driver_init();
	delay_init();
	TIM2_Cap_Init(0XFFFF,72-1);											//TIM2输入捕获初始化
	TIM3_Cap_Init(0XFFFF,72-1);											//TIM3输入捕获初始化
	delay_ms(20000);
	delay_ms(20000);
	delay_ms(20000);
	//delay_ms(20000);
	//delay_ms(20000);
	//delay_ms(20000);
	//delay_ms(20000);
	usart2_init();
	timer_Init(); 																	//系统Tick初始化
	TIM1_PWM_Init(14400-1,0);												//电机PWM初始化
}

