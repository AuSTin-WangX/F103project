#ifndef __USART1_H
#define __USART1_H

#include "stm32f10x.h"
#include <stdio.h>

/* 定义最大接收缓冲长度 */
#define USART1_RX_BUF_SIZE 256

/* 声明外部可调用的全局变量 */
extern uint8_t  USART1_RxBuffer[USART1_RX_BUF_SIZE]; // 接收缓冲区
extern volatile uint16_t USART1_RxLen;                // 当前接收到的数据长度
extern volatile uint8_t  USART1_RxFlag;               // 一帧数据接收完成标志位 (1: 完成)

/* 函数声明 */
void USART1_Init(uint32_t baudrate);
void USART1_SendByte(uint8_t byte);
void USART1_SendString(char *str);
void USART1_SendBuffer(uint8_t *buf, uint16_t len);

#endif
