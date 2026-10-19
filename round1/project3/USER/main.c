#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "serial.h"
#include "Delay.h"
#include "led.h"


uint8_t work_mode = 0; 
uint8_t step = 0;      
uint16_t timer = 0;    

int main(void)
{
    LED_Init();
    Serial_Init(); 

    while (1)
    {
       
        if (Serial_GetRxFlag() == 1)
        {
            uint8_t data = Serial_GetRxData();
            
            if (data == '1') 
            {
                work_mode = 1;
                GPIO_Write(GPIOA, ~0x0001); 
            }
            else if (data == '0') 
            {
                work_mode = 0;
                GPIO_Write(GPIOA, 0xFFFF); 
            }
            else if (data == '2') 
            {
                work_mode = 2;
                step = 0;
                timer = 0; 
            }
        }


        if (work_mode == 1) 
        {
            Delay_ms(10); 
        }
        else if (work_mode == 2) 
        {
            Delay_ms(10); 
            timer++;

            
            if (timer >= 50)
            {
                timer = 0;
                step++;
                if (step >= 8) step = 0; 

              
                switch(step)
                {
                    case 0: GPIO_Write(GPIOA, ~0x0001); break;
                    case 1: GPIO_Write(GPIOA, ~0x0002); break;
                    case 2: GPIO_Write(GPIOA, ~0x0004); break;
                    case 3: GPIO_Write(GPIOA, ~0x0008); break;
                    case 4: GPIO_Write(GPIOA, ~0x0010); break;
                    case 5: GPIO_Write(GPIOA, ~0x0020); break;
                    case 6: GPIO_Write(GPIOA, ~0x0040); break;
                    case 7: GPIO_Write(GPIOA, ~0x0080); break;
                }
            }
        }
        else 
        {
            Delay_ms(10); 
        }
    }
}
