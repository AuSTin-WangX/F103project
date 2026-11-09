#ifndef  _DHT11_H
#define  _DHT11_H

#include "stm32f10x.h"

void DHT11ModeOut(void);
void DHT11ModeIn(void);
void DHT11_Init(void);
uint8_t DHT11_ReadPin(void);
uint8_t DHT11_WaitLevel(uint8_t Level, uint16_t Timeout);
uint8_t DHT11_Start(void);
uint8_t DHT11_ReadByte(uint8_t *Value);
uint8_t DHT11_ReadData(uint8_t *Temp , uint8_t *Humid);


#endif
