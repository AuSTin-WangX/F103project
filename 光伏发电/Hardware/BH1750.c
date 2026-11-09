#include "BH1750.h"
#include "Delay.h"
#include "BH1750_I2C.H"  // ע�⣺������ո��ṩ�ĵײ�I2Cͷ�ļ�

/**
  * @brief  BH1750 ��ʼ��
  */
void BH1750_Init(void)
{
    // 1. ��ʼ���ײ� I2C ���� (PB10, PB11)
    IIC_Init();
    
    // 2. �����ϵ�ָ��
    IIC_Start();
    IIC_Send_Byte(BH1750_ADDR_WRITE);
    IIC_Wait_Ack();
    IIC_Send_Byte(BH1750_POWER_ON);
    IIC_Wait_Ack();
    IIC_Stop();
    
    // 3. ����Ϊ�����߷ֱ���ģʽ
    IIC_Start();
    IIC_Send_Byte(BH1750_ADDR_WRITE);
    IIC_Wait_Ack();
    IIC_Send_Byte(BH1750_CONT_H_RES);
    IIC_Wait_Ack();
    IIC_Stop();
    
    // ��һ�β�����Ҫһ��ʱ�� (Լ 120ms)
    Delay_ms(180); 
}

/**
  * @brief  ��ȡ����ǿ��
  * @retval ����ֵ (��λ: Lux)
  */
 uint16_t BH1750_Read_Lux(void)
{
    uint8_t buf[2] = {0};
    uint16_t raw_data = 0;
    float lux = 0.0;
    
    // 1. ���Ͷ���ַ
    IIC_Start();
    IIC_Send_Byte(BH1750_ADDR_READ);
    // ����ӻ�û��Ӧ��ֱ���˳������ش����?
    if (IIC_Wait_Ack()) {
        IIC_Stop();
        return 65535; 
    }
    
    // 2. ��ȡ�� 8 λ���ݣ����� ACK (���ߴ�����������)
    // ע�⣺�����ṩ��ԭ�����У�ack����0��������IIC_NAck������1����IIC_Ack��
    // ����������Ҫ��ACK�����Դ� 1��
    buf[0] = IIC_Read_Byte(1);
    
    // 3. ��ȡ�� 8 λ���ݣ����� NACK (���ߴ��������ݶ�����)
    // ����������Ҫ��NACK�����Դ� 0��
    buf[1] = IIC_Read_Byte(0);
    
    // 4. ����ͨ��
    IIC_Stop();
    
    // 5. ƴ�����ݲ�����
    raw_data = (buf[0] << 8) | buf[1];
    lux = raw_data / 1.2;
    
    return lux;
}
