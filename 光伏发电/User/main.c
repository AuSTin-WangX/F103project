#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "BH1750.h"

int main(void)
{
	 uint16_t lux = 0;
	
	OLED_Init();
	BH1750_Init();
	
	OLED_ShowChar(1, 1, 'A');
	//OLED_ShowString(1, 3, "HelloWorld!");
	OLED_ShowNum(2, 1, 12345, 5);
	OLED_ShowSignedNum(2, 7, -66, 2);
	OLED_ShowHexNum(3, 1, 0xAA55, 4);
	OLED_ShowBinNum(4, 1, 0xAA55, 16);
	
	while(1)
	{
		 lux = BH1750_Read_Lux();
			OLED_ShowNum(1, 3, lux, 8);
	}
	
}
