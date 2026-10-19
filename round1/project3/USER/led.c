#include "stm32f10x.h"
#include "stm32f10x_RCC.H"
#include "stm32f10x_gpio.H"
#include "Delay.h"

void LED_Init(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;                      //定义结构体(必须放在执行命令之前）
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);     //开启时钟（在rcc.h文件里找 + 记得包含在头文件里面）
	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);   	                 //初始化
	
  
	
}

