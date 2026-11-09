#ifndef __DHT11_H
#define __DHT11_H
 
#include "stm32f10x.h"
 
void DHT11_Init(void); //传感器初始化
 
void DHT11_Start(void);  //发送起始信号
u8 DHT11_receive_bit(void) ; //传感器返回响应信号。接收一个字节
void DHT11_receive_dat(void);//得到数据组
 
extern u8 receive_R_dat[10];//定义一个数据组
extern u8 receive_T_dat[10];//定义一个数据组
 
#endif
 
 
 
 