#include "usart2.h"
#include <string.h>
#include "delay.h"
#include "timer.h"

#define abs(x) ((x>0)?x:(-x))
#define DMA_Rec_Len_2 3
//void packet_dec(void);
static uint8_t buffer_UART2[DMA_Rec_Len_2];
uint8_t buf3_xxx[DMA_Rec_Len_2];
static uint8_t buf2_xxx[DMA_Rec_Len_2];

RC_DATA rc_data = {0};

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
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;                //USART1 RX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;            
  GPIO_Init(GPIOA, &GPIO_InitStructure); 

	USART_StructInit(&usart2);
	usart2.USART_BaudRate = 100e3;
	usart2.USART_WordLength = USART_WordLength_8b;
	usart2.USART_StopBits = USART_StopBits_2;
	usart2.USART_Parity = USART_Parity_Even;
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
  nvic_USART2_dma.NVIC_IRQChannelPreemptionPriority = 2;
  nvic_USART2_dma.NVIC_IRQChannelSubPriority = 0;
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

		if(buffer_UART2[0]==0xFF	&&	buffer_UART2[2]==0xFE)				
		{
			memcpy(buf3_xxx,buffer_UART2,DMA_Rec_Len_2);
		//	packet_dec();
		}else
		{
			int bit = 0;
			if(buf2_xxx[DMA_Rec_Len_2-1]==0xFF	&&	buffer_UART2[DMA_Rec_Len_2 - 2]==0xFE)	
			{
				bit = DMA_Rec_Len_2-1;
			}else 
			{
				for(int i = 0;i < DMA_Rec_Len_2;i++)											
				{
					if(buf2_xxx[i]==0xFF) 
						{
							bit = i;		
							break;
						}
					}
			}
			memcpy(buf3_xxx,buf2_xxx+bit,DMA_Rec_Len_2-bit);
			memcpy(buf3_xxx+DMA_Rec_Len_2-bit,buffer_UART2,bit);
			memcpy(buf2_xxx,buffer_UART2,DMA_Rec_Len_2);
			///packet_dec();
		}
	}
}




	
