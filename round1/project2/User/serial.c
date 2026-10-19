#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.H"
#include <stdio.h>
#include "misc.h"

uint8_t Serial_RxData;
uint8_t Serial_RxFlag; //接收标志位

void Serial_Init(void)
{ 
	USART_InitTypeDef USART_InitSructure;
	GPIO_InitTypeDef GPIO_InitStructure; 
	NVIC_InitTypeDef NVIC_InitStructture;
	
	// 时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 

	// 引脚初始化
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);   	
	
	//引脚设置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	
	//串口设置
	USART_InitSructure.USART_BaudRate = 115200;
	USART_InitSructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitSructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; 
	USART_InitSructure.USART_Parity = USART_Parity_No;
	USART_InitSructure.USART_StopBits = USART_StopBits_1;
	USART_InitSructure.USART_WordLength = USART_WordLength_8b;
	
	USART_Init(USART2, &USART_InitSructure); //按照上文把串口初始化
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //开启USART的接收终中断
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//优先级分组
	
	
	NVIC_InitStructture.NVIC_IRQChannel =  USART2_IRQn; //指定中断通道
	NVIC_InitStructture.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructture.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructture.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructture);
	
  USART_Cmd(USART2, ENABLE); //使能USART2外设
}

//串口发送单字节
void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART2, Byte);
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);//阻塞等待到寄存器发射空信号
}

//串口发送数组
void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i=0; i< Length; i ++)
	{
	    Serial_SendByte(Array[i]);
	}
}

//串口发送字符串
void Serial_SendString(char *String)
{
  uint8_t i;
  for (i = 0; String[i] != '\0'; i ++)
  {
	     Serial_SendByte (String[i]);	
	}	
}

//定义fputc，使支持printf函数
int fputc(int ch, FILE *f)
{
  Serial_SendByte(ch);
	return ch;
}

//串口接收标志位
uint8_t Serial_GetRxFlag(void)
{
   if (Serial_RxFlag == 1)
	 {
	   Serial_RxFlag = 0; // 
		 return 1;
	 }
   return 0;
}

//获取串口接收的数据
uint8_t Serial_GetRxData(void)
{
   return Serial_RxData; 
}

//USART2中断服务
void USART2_IRQHandler (void)
{
  if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{
    Serial_RxData = USART_ReceiveData(USART2);
		Serial_RxFlag = 1;

		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
}



