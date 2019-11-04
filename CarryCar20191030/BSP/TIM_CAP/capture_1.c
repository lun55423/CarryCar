#include "capture.h"

CAPTURE Tim3_Capture_1={0};
CAPTURE Tim3_Capture_2={0};
CAPTURE Tim3_Capture_3={0};
CAPTURE Tim3_Capture_4={0};

void TIM3_Cap_Init(u16 arr,u16 psc)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	TIM_TimeBaseInitTypeDef  TIM3_TimeBaseStructure;

  NVIC_InitTypeDef NVIC1_InitStructure;

	TIM_ICInitTypeDef  TIM3_ICInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);  
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6|GPIO_Pin_7;  	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_6|GPIO_Pin_7);						 	

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1;  	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1);	
	//初始化定时器 TIM3 
	TIM3_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值 
	TIM3_TimeBaseStructure.TIM_Prescaler =psc; 	//预分频器   
	TIM3_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM3_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM3_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM3输入捕获参数
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	选择输入端 IC1映射到TI1上
  TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;						//上升沿捕获
  TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 				//映射
  TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 								//配置输入分频,不分频 
  TIM3_ICInitStructure.TIM_ICFilter = 0x0f;																//IC1F=0000 配置输入滤波器 不滤波
  TIM_ICInit(TIM3, &TIM3_ICInitStructure);
	
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01 	选择输入端 IC1映射到TI1上
  TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;						//上升沿捕获
  TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 				//映射
  TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 								//配置输入分频,不分频 
  TIM3_ICInitStructure.TIM_ICFilter = 0x0f;																//IC1F=0000 配置输入滤波器 不滤波
  TIM_ICInit(TIM3, &TIM3_ICInitStructure);
	
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_3; //CC1S=01 	选择输入端 IC1映射到TI1上
  TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;						//上升沿捕获
  TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 				//映射
  TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 								//配置输入分频,不分频 
  TIM3_ICInitStructure.TIM_ICFilter = 0x0f;																//IC1F=0000 配置输入滤波器 不滤波
  TIM_ICInit(TIM3, &TIM3_ICInitStructure);
	
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_4; //CC1S=01 	选择输入端 IC1映射到TI1上
  TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;						//上升沿捕获
  TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 				//映射
  TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 								//配置输入分频,不分频 
  TIM3_ICInitStructure.TIM_ICFilter = 0x0f;																//IC1F=0000 配置输入滤波器 不滤波
  TIM_ICInit(TIM3, &TIM3_ICInitStructure);

	//中断分组初始化
	NVIC1_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  						//TIM3 中断
	NVIC1_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  	//先占优先级2级
	NVIC1_InitStructure.NVIC_IRQChannelSubPriority = 0;  				//从优先级0级
	NVIC1_InitStructure.NVIC_IRQChannelCmd = ENABLE; 						//IRQ通道被使能
	NVIC_Init(&NVIC1_InitStructure);  														//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
	
	TIM_ITConfig(TIM3,TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4,ENABLE);					//允许更新中断 ,允许CC1IE捕获中断	

  TIM_Cmd(TIM3,ENABLE ); 	//使能定时器1

}


void TIM3_IRQHandler(void)
{ 
		if(TIM_GetITStatus(TIM3,TIM_IT_CC1)!=RESET)  
		{  
      // TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);     
       if(Tim3_Capture_1.state==0)           
       {  
          Tim3_Capture_1.state=1;  
          Tim3_Capture_1.count_pre=TIM_GetCapture1(TIM3);            
       }  
       else if(Tim3_Capture_1.state==1)         
       {  
          Tim3_Capture_1.state=0;  
          Tim3_Capture_1.count_last=TIM_GetCapture1(TIM3);       
          if(Tim3_Capture_1.count_pre<Tim3_Capture_1.count_last)  
          {  
                Tim3_Capture_1.test=Tim3_Capture_1.count_last-Tim3_Capture_1.count_pre;   
          }  
          else if(Tim3_Capture_1.count_pre>Tim3_Capture_1.count_last)  
          {  
                Tim3_Capture_1.test=(0xffff-Tim3_Capture_1.count_pre)+Tim3_Capture_1.count_last; 
          }  
          else Tim3_Capture_1.test=1;  
          Tim3_Capture_1.freq=1000000/Tim3_Capture_1.test;   					
											if(Tim3_Capture_1.freq<100)
						{
							Tim3_Capture_1.freq = 0;
						}
				}  
		}
	 if(TIM_GetITStatus(TIM3,TIM_IT_CC2)!=RESET)  
   {  
      // TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);     
       if(Tim3_Capture_2.state==0)           
       {  
          Tim3_Capture_2.state=1;  
          Tim3_Capture_2.count_pre=TIM_GetCapture2(TIM3);            
       }  
       else if(Tim3_Capture_2.state==1)         
       {  
          Tim3_Capture_2.state=0;  
          Tim3_Capture_2.count_last=TIM_GetCapture2(TIM3);       
          if(Tim3_Capture_2.count_pre<Tim3_Capture_2.count_last)  
          {  
                Tim3_Capture_2.test=Tim3_Capture_2.count_last-Tim3_Capture_2.count_pre;   
          }  
          else if(Tim3_Capture_2.count_pre>Tim3_Capture_2.count_last)  
          {  
                Tim3_Capture_2.test=(0xffff-Tim3_Capture_2.count_pre)+Tim3_Capture_2.count_last; 
          }  
          else Tim3_Capture_2.test=1;  
          Tim3_Capture_2.freq=1000000/Tim3_Capture_2.test;      
						if(Tim3_Capture_2.freq<100)
						{
							Tim3_Capture_2.freq = 0;
						}
				}  
		}
		if(TIM_GetITStatus(TIM3,TIM_IT_CC3)!=RESET)  
		{  
      // TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);     
       if(Tim3_Capture_3.state==0)           
       {  
          Tim3_Capture_3.state=1;  
          Tim3_Capture_3.count_pre=TIM_GetCapture3(TIM3);            
       }  
       else if(Tim3_Capture_3.state==1)         
       {  
          Tim3_Capture_3.state=0;  
          Tim3_Capture_3.count_last=TIM_GetCapture3(TIM3);       
          if(Tim3_Capture_3.count_pre<Tim3_Capture_3.count_last)  
          {  
                Tim3_Capture_3.test=Tim3_Capture_3.count_last-Tim3_Capture_3.count_pre;   
          }  
          else if(Tim3_Capture_3.count_pre>Tim3_Capture_3.count_last)  
          {  
                Tim3_Capture_3.test=(0xffff-Tim3_Capture_3.count_pre)+Tim3_Capture_3.count_last; 
          }  
          else Tim3_Capture_3.test=1;  
          Tim3_Capture_3.freq=1000000/Tim3_Capture_3.test;               
						if(Tim3_Capture_3.freq<100)
						{
							Tim3_Capture_3.freq = 0;
						}					
				}  
		}
		if(TIM_GetITStatus(TIM3,TIM_IT_CC4)!=RESET)  
		{  
      // TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);     
       if(Tim3_Capture_4.state==0)           
       {  
          Tim3_Capture_4.state=1;  
          Tim3_Capture_4.count_pre=TIM_GetCapture4(TIM3);            
       }  
       else if(Tim3_Capture_4.state==1)         
       {  
          Tim3_Capture_4.state=0;  
          Tim3_Capture_4.count_last=TIM_GetCapture4(TIM3);       
          if(Tim3_Capture_4.count_pre<Tim3_Capture_4.count_last)  
          {  
                Tim3_Capture_4.test=Tim3_Capture_4.count_last-Tim3_Capture_4.count_pre;   
          }  
          else if(Tim3_Capture_4.count_pre>Tim3_Capture_4.count_last)  
          {  
                Tim3_Capture_4.test=(0xffff-Tim3_Capture_4.count_pre)+Tim3_Capture_4.count_last; 
          }  
          else  Tim3_Capture_4.test=1;  
          Tim3_Capture_4.freq=1000000/Tim3_Capture_4.test; 
											if(Tim3_Capture_4.freq<100)
						{
							Tim3_Capture_4.freq = 0;
						}
				}  
			}
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4);  
}


u16 freq_limit(u16 freq)
{
	if(freq>500) return 500;
	else if(freq<50)  return 0;
	else return freq;
}

