#include "stm32f10x.h" 
#include "OLED.h"

int main(void)
{
    OLED_Init();

    OLED_ShowString(1, 1, "Hello, World!");
    OLED_ShowNum(2, 1, 12345, 5);
    OLED_ShowSignedNum(2, 7, 12345, 5);
    OLED_ShowHexNum(3, 1, 0xAA55, 4);
    OLED_ShowBinNum(3, 7, 0xAA55, 16);

    while(1)
    {
      
    }
}

