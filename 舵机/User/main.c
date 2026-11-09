#include "stm32f10x.h"
#include "I2C.h"
#include "PCA9685.h"
#include "Delay.h"
#include "OLED.h"

int main(void)
{
    uint8_t ack1 = 0xFF;
    uint8_t ack2 = 0xFF;
    uint8_t ack3 = 0xFF;

    OLED_Init();
    IIC_Init();
    Delay_ms(10);

    IIC_Start();

    IIC_Send_Byte(0x80);
    ack1 = IIC_Wait_Ack();

    if (ack1 == 0)
    {
        IIC_Send_Byte(0x00);
        ack2 = IIC_Wait_Ack();

        if (ack2 == 0)
        {
            IIC_Send_Byte(0x20);
            ack3 = IIC_Wait_Ack();

            if (ack3 == 0)
            {
                IIC_Stop();
            }
        }
    }

    OLED_ShowString(1, 1, "ACK1:");
    OLED_ShowHexNum(1, 7, ack1, 2);

    OLED_ShowString(2, 1, "ACK2:");
    OLED_ShowHexNum(2, 7, ack2, 2);

    OLED_ShowString(3, 1, "ACK3:");
    OLED_ShowHexNum(3, 7, ack3, 2);

    if (ack1 != 0 || ack2 != 0 || ack3 != 0)
    {
        OLED_ShowString(4, 1, "IIC FAIL");
        while (1);
    }

    OLED_ShowString(4, 1, "IIC OK");

    PCA9685_Init(50);
	
    OLED_ShowString(4, 1, "INIT DONE");
    
    PCA9685_SetServoAngle(0, 90);
    Delay_ms(1000);

    while (1)
    {
        PCA9685_SetServoAngle(0, 80);
        Delay_ms(1000);

        PCA9685_SetServoAngle(0, 100);
        Delay_ms(1000);
    }
}
