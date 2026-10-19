#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "serial.h"
#include "Delay.h"
#include "Dht11.h" 
#include <stdio.h>

int main(void)
{
    
    DHT11_Data_TypeDef myDHT11Data; 
    uint8_t res = 0;                
    char send_buffer[50];           

    
    Serial_Init();          
    DHT11_GPIO_Config();   

    
    while (1)
    {
        
        res = Read_DHT11(&myDHT11Data); 

        if (res == 1) 
        {
           
            sprintf(send_buffer, "Success: T:%d.%d C, H:%d.%d %%\r\n", 
                    myDHT11Data.temp_int, myDHT11Data.temp_deci, 
                    myDHT11Data.humi_int, myDHT11Data.humi_deci);
        }
        else 
        {
            
            sprintf(send_buffer, "Read Failed\r\n");
        }
        
       
        Serial_SendString(send_buffer);

      
        Delay_ms(2000); 
    }
}