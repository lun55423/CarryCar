#include "usart2.h"

#include "led.h" 
#include "delay.h"
#include "stm32f10x.h"
#include "stm32f10x_dma.h"
#include <string.h>
#include <math.h>
#include "timer.h"
extern u32 time_cnt;
extern char segment_flag;
#define abs(x) (x>0)?(x):(-x)
#define DMA_Rec_Len_2 5

static uint8_t buffer_UART2[DMA_Rec_Len_2];
 uint8_t buf3_xxx[DMA_Rec_Len_2];
static uint8_t buf2_xxx[DMA_Rec_Len_2];

typedef struct Data_Point_Ave_Fil
{
	int bf_size;
	int counter;
	float data_max;
	float data_min;
	float data_out;
	float buffer[30];
	float sum;
}AVE_FIL;


static void packet_dec(void);
static float aver_filter(float data,AVE_FIL* ave);
static int16_t Limit_A_Filter(int16_t data);
OPENMV Openmv = {0};

void usart2_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  USART_InitTypeDef usart2;
	NVIC_InitTypeDef nvic_USART2_dma;
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输
	
  USART_DeInit(USART2);  //复位串口1
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;                //USART2 RX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;            
  GPIO_Init(GPIOA, &GPIO_InitStructure); 

	USART_StructInit(&usart2);
	usart2.USART_BaudRate = 115200;
	usart2.USART_WordLength = USART_WordLength_8b;
	usart2.USART_StopBits = USART_StopBits_1;
	usart2.USART_Parity = USART_Parity_No;
	usart2.USART_Mode = USART_Mode_Rx;
	usart2.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART2,&usart2);
	
	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);   //使能串口1 DMA接收
	
	//相应的DMA配置
	DMA_DeInit(DMA1_Channel6);   //将DMA的通道5寄存器重设为缺省值  串口1对应的是DMA通道5
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART2->DR;  //DMA外设ADC基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)buffer_UART2;  //DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //数据传输方向，从外设读取发送到内存
	DMA_InitStructure.DMA_BufferSize = DMA_Rec_Len_2;  //DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  //工作在正常缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh; //DMA通道 x拥有中优先级 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA1_Channel6, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器
	
	nvic_USART2_dma.NVIC_IRQChannel = DMA1_Channel6_IRQn;
  nvic_USART2_dma.NVIC_IRQChannelPreemptionPriority = 0;
  nvic_USART2_dma.NVIC_IRQChannelSubPriority = 1;
  nvic_USART2_dma.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvic_USART2_dma);

  DMA_Cmd(DMA1_Channel6, ENABLE);  //正式驱动DMA传输
	DMA_ITConfig(DMA1_Channel6,DMA_IT_TC,ENABLE);  //DMA传输完成中断 ！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
	USART_Cmd(USART2,ENABLE);
}

void DMA1_Channel6_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC6))
  {
		DMA_ClearFlag(DMA1_IT_TC6);
    DMA_ClearITPendingBit(DMA1_IT_TC6);
		if(buffer_UART2[0] == 0x5A	&&	buffer_UART2[1] == 0xA5)				
		{
			memcpy(buf3_xxx,buffer_UART2,DMA_Rec_Len_2);
			packet_dec();
		}else
		{
			int bit = 0;
			if(buf2_xxx[DMA_Rec_Len_2-1] == 0x5A	&&	buffer_UART2[0] == 0xA5)	
			{
				bit = DMA_Rec_Len_2-1;
			}else 
			{
				for(int i = 0;i < DMA_Rec_Len_2;i++)											
				{
					if(buf2_xxx[i] == 0x5A && buf2_xxx[i+1] == 0xA5) 
						{
							bit = i;		
							break;
						}
					}
			}
			memcpy(buf3_xxx,buf2_xxx+bit,DMA_Rec_Len_2-bit);
			memcpy(buf3_xxx+DMA_Rec_Len_2-bit,buffer_UART2,bit);
			memcpy(buf2_xxx,buffer_UART2,DMA_Rec_Len_2);
			packet_dec();
		}
	}
}
const u16 axis_x = 80;
const u16 axis_y = 60;

AVE_FIL Openmv_rho = {30,0,0,0,0,{0},0};
AVE_FIL Openmv_theta = {10,0,0,0,0,{0},0};

static void packet_dec(void)
{
	Openmv.theta = buf3_xxx[2];
	Openmv.x = buf3_xxx[3];
	Openmv.y = buf3_xxx[4];
	Openmv.delta_x = Openmv.x - axis_x;
	Openmv.delta_y = Openmv.y - axis_y;
	u8 theta = 0;
	if(Openmv.theta > 140 || Openmv.theta < 40) 
	{
		Openmv.line_num++;
		if(Openmv.theta > 140 && Openmv.delta_theta<180) theta = Openmv.theta  - 90;
		else theta = Openmv.theta  + 90;
	}
	else if(Openmv.theta > 70 && Openmv.theta < 110) 
	{
		if(Openmv.line_num > 50)
		{
			
			Openmv.line_parl++;
			segment_flag = 1;
		}
		Openmv.line_num = 0;
		Openmv.theta_parallel = Openmv.theta ;
	}
	Openmv.theta_vertical = theta;

//	static u8 is_first = 0;
//	static u16 parallel[2] = {0};
//	static u32 last_time = 0;
//	static u8 flag_1 = 0;
//	parallel[0] = parallel[1];
//	parallel[1] = Openmv.theta_parallel;
//	if((abs(parallel[1] - parallel[0]) > 50) && is_first == 0)
//	{
//		Openmv.line_num++;
//		is_first = 1;
//		last_time = time_cnt;
//		parallel[0] = parallel[1] = 0;
//		Openmv.theta_parallel = 0;
//	}
//	else if((abs(parallel[1] - parallel[0]) > 50) && is_first == 1)
//	{
//		if((time_cnt - last_time) > 1500) 
//		{
//			Openmv.line_num++;
//			last_time = time_cnt;
//			parallel[1] = parallel[0] = 0;
//			Openmv.theta_parallel = 0;
//		}else if((time_cnt - last_time) < 800)
//		{
//			last_time = time_cnt;
//			parallel[1] = parallel[0] = 0;
//			Openmv.theta_parallel = 0;
//		}
//	}
}

static int16_t Limit_A_Filter(int16_t data)
{
	static int16_t data_last = 0,data_now = 0;
	data_last = data_now;
	data_now = data;
	if(fabsf(data_now - data_last )< 40.0f)
		return data_last;
	else
		return data_now;

}
static float aver_filter(float data,AVE_FIL* ave)
{
	ave->data_max = -999;
	ave->data_min = 999;
	ave->sum = 0;
	if(ave->counter < ave->bf_size -1)
	{
		ave->buffer[ave->counter++] = data;
	}else 
	{
		ave->buffer[ave->counter] = data;
		ave->counter = 0;
	}
	for(int i = 0 ;i < ave->bf_size;i++)
	{
		if(ave->buffer[i] > ave->data_max) ave->data_max = ave->buffer[i];
		if(ave->buffer[i] < ave->data_min) ave->data_min = ave->buffer[i];
		ave->sum += ave->buffer[i];
	}
	ave->data_out = (ave->sum-ave->data_max-ave->data_min)/(ave->bf_size-2);
	return ave->data_out;
}
