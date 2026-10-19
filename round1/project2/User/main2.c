#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "serial.h"
#include "led.h"

int main(void)
{
    uint8_t data; 

    Serial_Init(); 
    LED_Init();    

    while (1)
    {
        if (Serial_GetRxFlag() == 1)
        {
            data = Serial_GetRxData(); 
            
            if (data == '1') {
                
                GPIO_ResetBits(GPIOC, GPIO_Pin_13);
            }
            else if (data == '0') {
                GPIO_SetBits(GPIOC, GPIO_Pin_13);  
            }
        }
    }
}
	
	
	
	
	
	
	
	
	
	

