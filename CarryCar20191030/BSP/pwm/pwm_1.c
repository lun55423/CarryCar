#include "pwm.h"

void TIM1_PWM_Init(u16 arr,u16 psc)
{
  GPIO_InitTypeDef 					GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  				TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO , ENABLE);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM1_CH1 M4_OUT
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //TIM1_CH2 M3_OUT
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //TIM1_CH3 M2_OUT
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //TIM1_CH4 M1_OUT
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx

  TIM_CtrlPWMOutputs(TIM1,ENABLE);	//MOE 主输出使能	
	
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH3_CH1 预装载使能	 
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH3_CH2 预装载使能	 
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH3_CH3 预装载使能	 
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH3_CH4 预装载使能	 
	
	TIM_ARRPreloadConfig(TIM1, ENABLE); //使能TIMx在ARR上的预装载寄存器
	
	TIM_Cmd(TIM1, ENABLE);  //使能TIM3
}


void direction(unsigned char i)
{
	/*
	正转
	M1
	B1_IN1 = 1  B1_IN2 = 0
	PA4 = 1       PC15 = 0
	
	M2
	B1_IN3 = 1  B1_IN4 = 0
	PC14 = 1       PC13 = 0
	
	M3
	B2_IN1 = 1  B2_IN2 = 0
	PB2 = 1       PA5 = 0
	
	M4
	B2_IN3 = 1  B2_IN2 = 0
	PB4 = 1       PB5 = 0
	*/
	if(i == 0)
	{
		//B1_IN1
		GPIO_SetBits(GPIOA,GPIO_Pin_4);
		//B1_IN2
		GPIO_ResetBits(GPIOC,GPIO_Pin_15);
		
		//B1_IN3
		GPIO_SetBits(GPIOC,GPIO_Pin_14);
		//B1_IN4
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
		
		//B2_IN1
		GPIO_SetBits(GPIOB,GPIO_Pin_2);
		//B2_IN2
		GPIO_ResetBits(GPIOA,GPIO_Pin_5);
		
		//B2_IN3
		GPIO_SetBits(GPIOB,GPIO_Pin_4);
		//B2_IN4
		GPIO_ResetBits(GPIOB,GPIO_Pin_5);
	}
		
	/*
	反转
	M1
	B1_IN1 = 1  B1_IN2 = 0
	PA4 = 0       PC15 = 1
	
	M2
	B1_IN3 = 1  B1_IN4 = 0
	PC14 = 0       PC13 = 1
	
	M3
	B2_IN1 = 1  B2_IN2 = 0
	PB2 = 0       PA5 = 1
	
	M4
	B2_IN3 = 1  B2_IN2 = 0
	PB4 = 0       PB5 = 1
	*/
	if(i == 1)
	{
		//B1_IN1
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
		//B1_IN2
		GPIO_SetBits(GPIOC,GPIO_Pin_15);
		
		//B1_IN3
		GPIO_ResetBits(GPIOC,GPIO_Pin_14);
		//B1_IN4
		GPIO_SetBits(GPIOC,GPIO_Pin_13);
		
		//B2_IN1
		GPIO_ResetBits(GPIOB,GPIO_Pin_2);
		//B2_IN2
		GPIO_SetBits(GPIOA,GPIO_Pin_5);
		
		//B2_IN3
		GPIO_ResetBits(GPIOB,GPIO_Pin_4);
		//B2_IN4
		GPIO_SetBits(GPIOB,GPIO_Pin_5);
	}
		
	/*
	左平移
	M1M4反
	M2M3正
	
	M1
	B1_IN1 = 1  B1_IN2 = 0
	PA4 = 0       PC15 = 1
	
	M2
	B1_IN3 = 1  B1_IN4 = 0
	PC14 = 1       PC13 = 0
	
	M3
	B2_IN1 = 1  B2_IN2 = 0
	PB2 = 1       PA5 = 0
	
	M4
	B2_IN3 = 1  B2_IN2 = 0
	PB4 = 0       PB5 = 1
	*/
	if(i == 2)
	{
		//B1_IN1
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
		//B1_IN2
		GPIO_SetBits(GPIOC,GPIO_Pin_15);
		
		//B1_IN3
		GPIO_SetBits(GPIOC,GPIO_Pin_14);
		//B1_IN4
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
		
		//B2_IN1
		GPIO_SetBits(GPIOB,GPIO_Pin_2);
		//B2_IN2
		GPIO_ResetBits(GPIOA,GPIO_Pin_5);
		
		//B2_IN3
		GPIO_ResetBits(GPIOB,GPIO_Pin_4);
		//B2_IN4
		GPIO_SetBits(GPIOB,GPIO_Pin_5);
	}
		
/*
  右平移
	M1M4正
	M2M3反
	
	M1
	B1_IN1 = 1  B1_IN2 = 0
	PA4 = 1       PC15 = 0
	
	M2
	B1_IN3 = 0  B1_IN4 = 1
	PC14 = 0       PC13 = 1
	
	M3
	B2_IN1 = 0  B2_IN2 = 1
	PB2 = 0       PA5 = 1
	
	M4
	B2_IN3 = 1  B2_IN2 = 0
	PB4 = 1       PB5 = 0
	*/
	if(i == 3)
	{
		//B1_IN1
		GPIO_SetBits(GPIOA,GPIO_Pin_4);
		//B1_IN2
		GPIO_ResetBits(GPIOC,GPIO_Pin_15);
		
		//B1_IN3
		GPIO_ResetBits(GPIOC,GPIO_Pin_14);
		//B1_IN4
		GPIO_SetBits(GPIOC,GPIO_Pin_13);
		
		//B2_IN1
		GPIO_ResetBits(GPIOB,GPIO_Pin_2);
		//B2_IN2
		GPIO_SetBits(GPIOA,GPIO_Pin_5);
		
		//B2_IN3
		GPIO_SetBits(GPIOB,GPIO_Pin_4);
		//B2_IN4
		GPIO_ResetBits(GPIOB,GPIO_Pin_5);
	}
}

