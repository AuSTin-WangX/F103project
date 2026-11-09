#include "stm32f10x.h"                  // Device header
#include "Delay.h"

#define DHT11_H GPIO_SetBits(GPIOB, GPIO_Pin_12)
#define DHT11_L GPIO_ResetBits(GPIOB, GPIO_Pin_12)

//#define DHT11_Set_Pin(x)  GPIO_WriteBits(GPIOB, GPIO_Pin_12, x)

/**
  * @brief          设置DHT11的GPIO为输出模式
  * @param          无
  * @retval         无
  */
void DHT11ModeOut(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;            //PB12
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;      //开漏输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //速度50MHz

    GPIO_Init(GPIOB, &GPIO_InitStructure);                //初始化GPIOB12
}

/**
  * @brief          设置DHT11的GPIO为输入模式
  * @param          无
  * @retval         无
  */
void DHT11ModeIn(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;              //PB12
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;      //浮空输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       //速度50MHz

    GPIO_Init(GPIOB, &GPIO_InitStructure);                  //初始化GPIOB12
}

/**
  * @brief          初始化DHT11
  * @param          无
  * @retval         无
  */
void DHT11_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);     //使能GPIOB时钟
    DHT11ModeOut();                                           //设置为输出模式
    GPIO_SetBits(GPIOB, GPIO_Pin_12);                         //释放总线
}

uint8_t DHT11_ReadPin(void)
{
    return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12);           //读取DHT11引脚电平
}

/**
  * @brief          等待DHT11的GPIO电平变化
  * @param          Level: 期望的电平状态，0表示低电平，1表示高电平
  * @param          Timeout: 超时时间，单位为微秒
  * @retval         0: 成功等待到期望电平，1: 超时未等待到期望电平
  */
uint8_t DHT11_WaitLevel(uint8_t Level, uint32_t Timeout)
{
    while(DHT11_ReadPin() != Level)
    {
        if(Timeout == 0)
        {
          return 1;                                     //超时
        } 
        Timeout--;
        Delay_us(1);
    }
        return 0;                                       //成功  
}

/**
  * @brief          启动DHT11的通信
  * @param          无
  * @retval         无
  */
uint8_t DHT11_Start(void)
{
    DHT11ModeOut();                                      //设置为输出模式
    DHT11_L;                                            //拉低总线
                                                
    Delay_ms(20);                                         //延时20ms      
                                        
    DHT11_H;                                           //释放总线
                                              
    Delay_us(30);                                     //延时30us
                                           
    DHT11ModeIn();                                       //设置为输入模式

    if(DHT11_WaitLevel(0, 120))                           //等待DHT11拉低总线
    {
        return 1;                                         //DHT11未响应，返回错误
    }

    if(DHT11_WaitLevel(1, 80))                           //等待DHT11拉高总线，超时80us
    {
        return 1;                                         //DHT11未响应，返回错误
    }

    if(DHT11_WaitLevel(0, 80))                           //等待DHT11拉低总线，超时80us
    {
        return 1;                                         //DHT11未响应，返回错误
    }

    return 0;                                             //成功
}

// /**
//   * @brief          读取DHT11的一个位数据
//   * @param          无
//   * @retval         0: 读取到的位为0，1: 读取到的位为1
//   */
// uint8_t DHT11_ReadBit(void)
// {
//   uint8_t data = 0;     

//   if(DHT11_WaitLevel(1, 55))                           
//   {
//        return 3;                                   
//   } 

//   if(DHT11_WaitLevel(0, 28))                           
//   {
//     data = 0;                                         
//   } 

//   if(DHT11_WaitLevel(0, 44))                           
//   {
//     data = 1;                                         
//   } 
  

//   return data;
    
   
// }

uint8_t DHT11_ReadBit(void)
{
    uint8_t data;

                                                                         // 1. 等待当前数据位的低电平结束
    if (DHT11_WaitLevel(1, 100))
    {
        return 1;
    }

                                                                        // 2. 高电平开始后，延时40us
    Delay_us(40);

                                                                        // 3. 判断当前DATA的电平
    data = DHT11_ReadPin();

                                                                        // 4. 如果是1，等待高电平结束
    if (data == 1)
    {
        if (DHT11_WaitLevel(0, 100))
        {
            return 1;                                                   // 超时未等待到低电平，返回错误
        }
    }

    // 5. 返回读取的bit
    return data;
}


/**
  * @brief          读取DHT11的一个字节数据
  * @param          Value: 指向存储读取字节的变量的指针
  * @retval         0: 成功读取，1: 读取失败
  */
uint8_t DHT11_ReadByte(uint8_t *Value)
{
    uint8_t i;
    uint8_t value = 0;
		uint8_t bit;
  
    *Value = 0;                                              // 初始化字节值为0
    for(i = 0; i < 8; i++)
    {
        bit = DHT11_ReadBit();                       // 读取一个bit
        if(bit > 1)
        {
            return 1;                                   // 读取失败，返回错误
        }
        value = (uint8_t)((value << 1) | bit);
        
    }   
		*Value = value;
		return 0;
}

/**
  * @brief          读取DHT11的温湿度数据
  * @param          Temp: 指向存储温度数据的变量的指针
  * @param          Humid: 指向存储湿度数据的变量的指针
  * @retval         0: 成功读取，1: 读取失败
  */
uint8_t DHT11_ReadData(uint8_t *Temp , uint8_t *Humid)
{
    uint8_t data[5];
    uint8_t i;
    uint8_t CheckSum = 0;

    if(DHT11_Start())                                  // 启动DHT11通信
    {
        return 1;                                       // DHT11未响应，返回错误 
    }
    for(i = 0; i < 5; i++)
    {
        if(DHT11_ReadByte(&data[i]))                       // 读取5个字节的数据
        {
            return 1;                                      // 读取失败，返回错误
        }
    }

    CheckSum = (uint8_t)(data[0] + data[1] + data[2] + data[3]);          // 计算校验和
    if(CheckSum != data[4])
    {
        return 2;                                       // 校验和不正确，可能数据传输有误
    }
    if(data[1] != 0 || data[3] != 0 )
    {
        return 3;                                       // 数据不符合预期，可能数据传输有误
    }
    *Temp = data[2];                                    // 存储温度数据
    *Humid = data[0];                                   // 存储湿度数据
    return 0;                                           // 成功读取数据
}
    

