#ifndef _PCA9685_H_
#define _PCA9685_H_

#include"stm32f10x.h"

#define PCA9685_MODE1 0x00
#define PCA9685_MODE2 0x01
#define PCA9685_LED0_ON_L 0x02
#define PCA9685_PRE_SCALE 0x03

void PCA9685_WriteReg(uint8_t reg, uint8_t data);
void PCA9685_Init(uint16_t frequency);
void PCA9685_SetPWM(uint8_t Channel, uint16_t On, uint16_t Off);
void PCA9685_SetServoAngle(uint8_t Channel, float Angle);

#endif
