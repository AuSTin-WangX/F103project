#include "stm32f10x.h" 
#include "OLED.h"
#include "Encoder.h"
#include "Delay.h"
    
int16_t Num = 0;

int main(void)
{
    OLED_Init();
    Encoder_Init();
	
		OLED_ShowString(1, 1, "Num: ");

    while(1)
    {
       Num += Encoder_GetCount();
       OLED_ShowSignedNum(1, 5, Num, 5);
    }
}

