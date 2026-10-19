#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "serial.h"

int main(void)
{
  Serial_Init();
 
  Serial_SendByte(0x41); 	
	
	while (1)
	{
	
	
  }

	
	
	
	
	
	
	
	
	
	
	
}
