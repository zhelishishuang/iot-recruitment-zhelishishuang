#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include <stdio.h>
#include "misc.h"

uint8_t Serial_RxData;
uint8_t Serial_RxFlag; 

void Serial_Init(void)
{ 
	USART_InitTypeDef USART_InitSructure;
	GPIO_InitTypeDef GPIO_InitStructure; 
	NVIC_InitTypeDef NVIC_InitStructture;
	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 


	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);   	
	

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	

	USART_InitSructure.USART_BaudRate = 9600;
	USART_InitSructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitSructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; 
	USART_InitSructure.USART_Parity = USART_Parity_No;
	USART_InitSructure.USART_StopBits = USART_StopBits_1;
	USART_InitSructure.USART_WordLength = USART_WordLength_8b;
	
	USART_Init(USART1, &USART_InitSructure); 
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStructture.NVIC_IRQChannel =  USART1_IRQn; 
	NVIC_InitStructture.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructture.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructture.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructture);
	
    USART_Cmd(USART1, ENABLE); 
}

void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1, Byte); 
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i=0; i< Length; i ++)
	{
	    Serial_SendByte(Array[i]);
	}
}

void Serial_SendString(char *String)
{
  uint8_t i;
  for (i = 0; String[i] != '\0'; i ++)
  {
	     Serial_SendByte (String[i]);	
	}	
}

int fputc(int ch, FILE *f)
{
  Serial_SendByte(ch);
	return ch;
}

uint8_t Serial_GetRxFlag(void)
{
   if (Serial_RxFlag == 1)
	 {
	   Serial_RxFlag = 0; 
		 return 1;
	 }
   return 0;
}

uint8_t Serial_GetRxData(void)
{
   return Serial_RxData; 
}

// ?? ??????? USART1_IRQHandler
void USART1_IRQHandler (void)
{
  if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET) 
	{
    Serial_RxData = USART_ReceiveData(USART1); 
		Serial_RxFlag = 1;
		USART_ClearITPendingBit(USART1, USART_IT_RXNE); 
	}
}


