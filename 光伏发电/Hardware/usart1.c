#include "usart1.h"

/* 全局变量定义 */
uint8_t  USART1_RxBuffer[USART1_RX_BUF_SIZE];
volatile uint16_t USART1_RxLen = 0;
volatile uint8_t  USART1_RxFlag = 0;

/**
  * @brief  初始化 USART1 (PA9 TX, PA10 RX)
  * @param  baudrate: 波特率 (如 115200)
  */
void USART1_Init(uint32_t baudrate)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 1. 开启时钟：GPIOA, USART1, AFIO(如需重映射)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);

    // 2. 配置 PA9 (TX) 为复用推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 3. 配置 PA10 (RX) 为浮空输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 4. 配置 USART1 参数
    USART_InitStructure.USART_BaudRate = baudrate;                  // 波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;     // 8位数据
    USART_InitStructure.USART_StopBits = USART_StopBits_1;          // 1位停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;             // 无校验
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件流控
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; // 收发模式
    USART_Init(USART1, &USART_InitStructure);

    // 5. 配置中断 (接收非空中断 + 空闲中断)
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // 接收到一个字节触发
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE); // 总线空闲触发

    // 6. 配置 NVIC
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 7. 使能串口
    USART_Cmd(USART1, ENABLE);
}

/**
  * @brief  发送单个字节
  */
void USART1_SendByte(uint8_t byte)
{
    USART_SendData(USART1, byte);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

/**
  * @brief  发送字符串
  */
void USART1_SendString(char *str)
{
    while (*str != '\0')
    {
        USART1_SendByte(*str);
        str++;
    }
}

/**
  * @brief  发送指定长度的数据 (用于发送二进制数据给RK3576)
  */
void USART1_SendBuffer(uint8_t *buf, uint16_t len)
{
    uint16_t i;
    for (i = 0; i < len; i++)
    {
        USART1_SendByte(buf[i]);
    }
}

/**
  * @brief  USART1 中断服务函数
  */
void USART1_IRQHandler(void)
{
    // 1. 处理接收中断 (每收到一个字节)
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        uint8_t data = USART_ReceiveData(USART1); // 读取数据自动清除RXNE标志
        
        // 存入缓冲区 (防止溢出)
        if (USART1_RxLen < USART1_RX_BUF_SIZE - 1)
        {
            USART1_RxBuffer[USART1_RxLen++] = data;
        }
    }

    // 2. 处理空闲中断 (一帧数据接收完毕)
    if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
    {
        // 必须"先读SR，再读DR"才能彻底清除IDLE标志
        volatile uint8_t temp = USART1->SR; 
        temp = USART1->DR; 
        (void)temp; // 防止编译警告

        // 标记一帧数据接收完成
        USART1_RxFlag = 1; 
    }
}