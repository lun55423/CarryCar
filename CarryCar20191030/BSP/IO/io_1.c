#include "io.h"

void io_init(void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB,PE端口时钟

		//LED0-->PB.5 端口配置
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;		
		GPIO_Init(GPIOA, &GPIO_InitStructure);		
		GPIO_SetBits(GPIOA,GPIO_Pin_4);	//B1_IN1

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	
		GPIO_Init(GPIOA, &GPIO_InitStructure);	
		GPIO_ResetBits(GPIOA,GPIO_Pin_5);//B2_IN2



		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;	
		GPIO_Init(GPIOC, &GPIO_InitStructure);	
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);//B1_IN4

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;	
		GPIO_Init(GPIOC, &GPIO_InitStructure);	
		GPIO_SetBits(GPIOC,GPIO_Pin_14);//B1_IN3

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	
		GPIO_Init(GPIOC, &GPIO_InitStructure);	
		GPIO_ResetBits(GPIOC,GPIO_Pin_15);//B1_IN2



		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	
		GPIO_Init(GPIOB, &GPIO_InitStructure);	
		GPIO_SetBits(GPIOB,GPIO_Pin_2);//B2_IN1


		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	
		GPIO_Init(GPIOB, &GPIO_InitStructure);	
		GPIO_ResetBits(GPIOB,GPIO_Pin_4);//B2_IN3
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	
		GPIO_Init(GPIOB, &GPIO_InitStructure);	
		GPIO_ResetBits(GPIOB,GPIO_Pin_5);//B2_IN4
}

