#include "capture2.h"

CAPTURE Tim2_Capture_1={0};
CAPTURE Tim2_Capture_2={0};
CAPTURE Tim2_Capture_3={0};
CAPTURE Tim2_Capture_4={0};


void TIM2_Cap_Init(u16 arr,u16 psc)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM2_TimeBaseStructure;
  NVIC_InitTypeDef NVIC2_InitStructure;
	TIM_ICInitTypeDef  TIM2_ICInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);  
	GPIO_PinRemapConfig(GPIO_PartialRemap2_TIM2, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1;  	//TIM4_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1);						 	

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10|GPIO_Pin_11;  	//TIM4_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11);		
	//初始化定时器 TIM2
	TIM2_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值 
	TIM2_TimeBaseStructure.TIM_Prescaler =psc; 	//预分频器   
	TIM2_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM2_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM2_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM2输入捕获参数
	TIM2_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	选择输入端 IC1映射到TI1上
  TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;						//上升沿捕获
  TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 				//映射
  TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 								//配置输入分频,不分频 
  TIM2_ICInitStructure.TIM_ICFilter = 0x0f;																//IC1F=0000 配置输入滤波器 不滤波
  TIM_ICInit(TIM2, &TIM2_ICInitStructure);
	
	TIM2_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01 	选择输入端 IC1映射到TI1上
  TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;						//上升沿捕获
  TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 				//映射
  TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 								//配置输入分频,不分频 
  TIM2_ICInitStructure.TIM_ICFilter = 0x0f;																//IC1F=0000 配置输入滤波器 不滤波
  TIM_ICInit(TIM2, &TIM2_ICInitStructure);
	
	TIM2_ICInitStructure.TIM_Channel = TIM_Channel_3; //CC1S=01 	选择输入端 IC1映射到TI1上
  TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;						//上升沿捕获
  TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 				//映射
  TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 								//配置输入分频,不分频 
  TIM2_ICInitStructure.TIM_ICFilter = 0x0f;																//IC1F=0000 配置输入滤波器 不滤波
  TIM_ICInit(TIM2, &TIM2_ICInitStructure);
	
	TIM2_ICInitStructure.TIM_Channel = TIM_Channel_4; //CC1S=01 	选择输入端 IC1映射到TI1上
  TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;						//上升沿捕获
  TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 				//映射
  TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 								//配置输入分频,不分频 
  TIM2_ICInitStructure.TIM_ICFilter = 0x0f;																//IC1F=0000 配置输入滤波器 不滤波
  TIM_ICInit(TIM2, &TIM2_ICInitStructure);

	//中断分组初始化
	NVIC2_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  						//TIM2 中断
	NVIC2_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  	//先占优先级2级
	NVIC2_InitStructure.NVIC_IRQChannelSubPriority = 1;  				//从优先级0级
	NVIC2_InitStructure.NVIC_IRQChannelCmd = ENABLE; 						//IRQ通道被使能
	NVIC_Init(&NVIC2_InitStructure);  														//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
	
	TIM_ITConfig(TIM2,TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4,ENABLE);					//允许更新中断 ,允许CC1IE捕获中断	

  TIM_Cmd(TIM2,ENABLE ); 	//使能定时器2
}


void TIM2_IRQHandler(void)
{ 
		if(TIM_GetITStatus(TIM2,TIM_IT_CC1)!=RESET)  
		{  
       //TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);     
       if(Tim2_Capture_1.state==0)           
       {  
          Tim2_Capture_1.state=1;  
          Tim2_Capture_1.count_pre=TIM_GetCapture1(TIM2);            
       }  
       else if(Tim2_Capture_1.state==1)         
       {  
          Tim2_Capture_1.state=0;  
          Tim2_Capture_1.count_last=TIM_GetCapture1(TIM2);       
          if(Tim2_Capture_1.count_pre<Tim2_Capture_1.count_last)  
          {  
                Tim2_Capture_1.test=Tim2_Capture_1.count_last-Tim2_Capture_1.count_pre;   
          }  
          else if(Tim2_Capture_1.count_pre>Tim2_Capture_1.count_last)  
          {  
                Tim2_Capture_1.test=(0xffff-Tim2_Capture_1.count_pre)+Tim2_Capture_1.count_last; 
          }  
          else Tim2_Capture_1.test=1;  
          Tim2_Capture_1.freq=1000000/Tim2_Capture_1.test;   		
						if(Tim2_Capture_1.freq<100)
						{
							Tim2_Capture_1.freq = 0;
						}					
				}  
		}
	 if(TIM_GetITStatus(TIM2,TIM_IT_CC2)!=RESET)  
   {  
       //TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);     
       if(Tim2_Capture_2.state==0)           
       {  
          Tim2_Capture_2.state=1;  
          Tim2_Capture_2.count_pre=TIM_GetCapture2(TIM2);            
       }  
       else if(Tim2_Capture_2.state==1)         
       {  
          Tim2_Capture_2.state=0;  
          Tim2_Capture_2.count_last=TIM_GetCapture2(TIM2);       
          if(Tim2_Capture_2.count_pre<Tim2_Capture_2.count_last)  
          {  
                Tim2_Capture_2.test=Tim2_Capture_2.count_last-Tim2_Capture_2.count_pre;   
          }  
          else if(Tim2_Capture_2.count_pre>Tim2_Capture_2.count_last)  
          {  
                Tim2_Capture_2.test=(0xffff-Tim2_Capture_2.count_pre)+Tim2_Capture_2.count_last; 
          }  
          else Tim2_Capture_2.test=1;  
          Tim2_Capture_2.freq=1000000/Tim2_Capture_2.test; 
						if(Tim2_Capture_2.freq<100)
						{
							Tim2_Capture_2.freq = 0;
						}							
				}  
		}
		if(TIM_GetITStatus(TIM2,TIM_IT_CC3)!=RESET)  
		{  
       //TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);     
       if(Tim2_Capture_3.state==0)           
       {  
          Tim2_Capture_3.state=1;  
          Tim2_Capture_3.count_pre=TIM_GetCapture3(TIM2);            
       }  
       else if(Tim2_Capture_3.state==1)         
       {  
          Tim2_Capture_3.state=0;  
          Tim2_Capture_3.count_last=TIM_GetCapture3(TIM2);       
          if(Tim2_Capture_3.count_pre<Tim2_Capture_3.count_last)  
          {  
                Tim2_Capture_3.test=Tim2_Capture_3.count_last-Tim2_Capture_3.count_pre;   
          }  
          else if(Tim2_Capture_3.count_pre>Tim2_Capture_3.count_last)  
          {  
                Tim2_Capture_3.test=(0xffff-Tim2_Capture_3.count_pre)+Tim2_Capture_3.count_last; 
          }  
          else Tim2_Capture_3.test=1;  
          Tim2_Capture_3.freq=1000000/Tim2_Capture_3.test;    
						if(Tim2_Capture_3.freq<100)
						{
							Tim2_Capture_3.freq = 0;
						}							
				}  
		}
		if(TIM_GetITStatus(TIM2,TIM_IT_CC4)!=RESET)  
		{  
       //TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);     
       if(Tim2_Capture_4.state==0)           
       {  
          Tim2_Capture_4.state=1;  
          Tim2_Capture_4.count_pre=TIM_GetCapture4(TIM2);            
       }  
       else if(Tim2_Capture_4.state==1)         
       {  
          Tim2_Capture_4.state=0;  
          Tim2_Capture_4.count_last=TIM_GetCapture4(TIM2);       
          if(Tim2_Capture_4.count_pre<Tim2_Capture_4.count_last)  
          {  
                Tim2_Capture_4.test=Tim2_Capture_4.count_last-Tim2_Capture_4.count_pre;   
          }  
          else if(Tim2_Capture_4.count_pre>Tim2_Capture_4.count_last)  
          {  
                Tim2_Capture_4.test=(0xffff-Tim2_Capture_4.count_pre)+Tim2_Capture_4.count_last; 
          }  
          else  Tim2_Capture_4.test=1;  
          Tim2_Capture_4.freq=1000000/Tim2_Capture_4.test;  
						if(Tim2_Capture_4.freq<100)
						{
							Tim2_Capture_4.freq = 0;
						}							
				}  
			}
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4); 
}

