#ifndef __BH1750_I2C_H
#define __BH1750_I2C_H
#include "stm32f10x.h"


#define SDA_IN()		GPIO_InitTypeDef  GPIO_InitStructure;\
										GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;\
										GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;\
										GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;\
										GPIO_Init(GPIOB, &GPIO_InitStructure);

#define SDA_OUT() 	GPIO_InitTypeDef  GPIO_InitStructure;\
										GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;\
										 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;\
										GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;\
										GPIO_Init(GPIOB,&GPIO_InitStructure);


#define IIC_SCL_1     GPIO_WriteBit(GPIOB,GPIO_Pin_11,(BitAction)1)//SCL
#define IIC_SCL_0     GPIO_WriteBit(GPIOB,GPIO_Pin_11,(BitAction)0)//SCL

#define IIC_SDA_0     GPIO_WriteBit(GPIOB,GPIO_Pin_10,(BitAction)0)//SDA	 
#define IIC_SDA_1			GPIO_WriteBit(GPIOB,GPIO_Pin_10,(BitAction)1)//SDA

#define READ_SDA    	 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)//SDA 


void IIC_Init(void);                	 
void IIC_Start(void);				
void IIC_Stop(void);	  			
void IIC_Send_Byte(uint8_t txd);			
uint8_t IIC_Read_Byte(unsigned char ack);
uint8_t IIC_Wait_Ack(void); 				
void IIC_Ack(void);					
void IIC_NAck(void);				

uint16_t Read_Pad (void);
#endif
