#include "stm32f10x.h"
#include "I2C.h"
#include "PCA9685.h"
#include "Delay.h"
#include "OLED.h"
#include "DHT11.h"
int main(void)
{
    uint8_t ack1 = 0xFF;
    uint8_t ack2 = 0xFF;
    uint8_t ack3 = 0xFF;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //ÉèÖÃNVICÖÐ¶Ï·Ö×é2:2Î»ÇÀÕ¼ÓÅÏÈ¼¶£¬2Î»ÏìÓ¦ÓÅÏÈ¼¶
	
	DHT11_Init() ;   //ÎÂÊª¶È´«¸ÐÆ÷³õÊ¼»¯


    OLED_Init();
    IIC_Init();
    Delay_ms(10);

    

    while (1)
    {
        DHT11_receive_dat();

        Delay_ms(1000);
        

        
    }
}
