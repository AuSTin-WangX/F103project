#include "stm32f10x.h"                  // Device header
#include "I2C.h"
#include "Delay.h"

#define PCA9685_ADDR 0x40                 // PCA9685 I2C地址
#define PCA9685_MODE1 0x00                // 模式寄存器1
#define PCA9685_MODE2 0x01                // 模式寄存器2
#define PCA9685_PRE_SCALE 0xFE            // 预分频寄存器
#define PCA9685_LED0_ON_L 0x06            // LED0 ON低字节寄存器

/**
  * @brief          写入PCA9685寄存器
  * @param  reg     寄存器地址
  * @param  data    要写入的数据
  * @retval         无
  */
void PCA9685_WriteReg(uint8_t Reg, uint8_t Data)
{
    IIC_Start();                              // 发送起始信号

    IIC_Send_Byte(PCA9685_ADDR << 1);         // 写入寄存器
    IIC_Wait_Ack();                           // 等待应答

    IIC_Send_Byte(Reg);                       // 写入寄存器地址
    IIC_Wait_Ack();                           // 等待应答

    IIC_Send_Byte(Data);                      // 写入数据
    IIC_Wait_Ack();                           // 等待应答

    IIC_Stop();                               // 停止信号
}

/**
  * @brief            初始化PCA9865
  * @param  frequency PWM频率
  * @retval           无
  */
void PCA9685_Init(uint16_t frequency)
{
    uint8_t Prescale;
    uint8_t mode1 = 0x20;                      

    Prescale = (uint8_t)(25000000UL + 2048UL * frequency / (4096UL * frequency) - 1);    // 计算预分频值

    PCA9685_WriteReg(PCA9685_MODE1, mode1);         // 设置为睡眠模式
    PCA9685_WriteReg(PCA9685_MODE2, 0x40);      // 设置为推挽输出模式
    
    PCA9685_WriteReg(PCA9685_MODE1, mode1 | 0x01);  // 设置为睡眠模式 同时保留AI为1
    PCA9685_WriteReg(PCA9685_MODE1, Prescale);      // 设置PWM频率
    PCA9685_WriteReg(PCA9685_MODE1, mode1);         // 唤醒

    Delay_ms(1);                                     // 等待稳定
    PCA9685_WriteReg(PCA9685_MODE1, mode1 | 0x80);  // 设置为睡眠模式 同时保留AI为1
}

/**
  * @brief           设置PCA9685的PWM输出
  * @param  Channel  通道号（0-15）
  * @param  On       PWM的ON时间（0-4095）
  * @param  Off      PWM的OFF时间（0-4095）
  * @retval          无
  */
void PCA9685_SetPWM(uint8_t Channel, uint16_t On, uint16_t Off)
{
    uint8_t reg;

    if (Channel > 15 || On > 4095 || Off > 4095) 
    {
        return;                                         // 通道范围检查
    }

    reg = PCA9685_LED0_ON_L + 4 * Channel;

    PCA9685_WriteReg(reg, On & 0xFF);                   // 设置ON低字节
    PCA9685_WriteReg(reg + 1, (On >> 8) & 0x0F);       // 设置ON高字节
    PCA9685_WriteReg(reg + 2, Off & 0xFF);              // 设置OFF低字节
    PCA9685_WriteReg(reg + 3, (Off >> 8) & 0x0F);       // 设置OFF高字节
}

void PCA9685_SetServoAngle(uint8_t Channel, float Angle)
{
    uint16_t Pulse_us;
    uint16_t Count;
    if (Channel > 15 || Angle < 0.0f || Angle > 180.0f) 
    {
        return;                                         // 通道范围检查
    }

    Pulse_us = (uint16_t)(1000.0f + Angle * 1000.0f / 180.0f);
    Count = (uint32_t)Pulse_us * 4096UL / 1000000UL;                  // 将脉冲宽度转换为计数

    PCA9685_SetPWM(Channel, 0, Count);                  // 设置PWM输出


}
