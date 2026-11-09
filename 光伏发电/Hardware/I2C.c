//#include "I2C.h"
//#include "Delay.h"
//	

//void IIC_Init(void)
//{					     
//	GPIO_InitTypeDef  GPIO_InitStructure;
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(GPIOB, &GPIO_InitStructure);
//}

//void IIC_Start(void)
//{
//	SDA_OUT();     
//	IIC_SDA_1;	  	  
//	IIC_SCL_1;
//	Delay_us(4);
// 	IIC_SDA_0; 
//	Delay_us(4);
//	IIC_SCL_0;
//}	  

//void IIC_Stop(void)
//{
//	SDA_OUT();
//	IIC_SCL_0;
//	IIC_SDA_0;
// 	Delay_us(4);
//	IIC_SCL_1; 
//	IIC_SDA_1;
//	Delay_us(4);							   	
//}


//uint8_t IIC_Wait_Ack(void)
//{
//	u8 ucErrTime=0;
//	SDA_IN();      
//	IIC_SDA_1;
//	Delay_us(1);	   
//	IIC_SCL_1;
//	Delay_us(1);	 
//	while(READ_SDA)
//	{
//		ucErrTime++;
//		if(ucErrTime>250)
//		{
//			IIC_Stop();
//			return 1;
//		}
//	}
//	IIC_SCL_0;	   
//	return 0;  
//} 

//void IIC_Ack(void)
//{
//	SDA_OUT();
//	IIC_SCL_0;
//	IIC_SDA_0;
//	Delay_us(2);
//	IIC_SCL_0;
//	Delay_us(2);
//	IIC_SCL_0;
//}
//		    
//void IIC_NAck(void)
//{
//	SDA_OUT();
//	IIC_SCL_0;
//	IIC_SDA_1;
//	Delay_us(2);
//	IIC_SCL_1;
//	Delay_us(2);
//	IIC_SCL_0;
//}					 				     
//		  
//void IIC_Send_Byte(u8 txd)
//{   
//	
//    u8 t;   
//		SDA_OUT();    
//    IIC_SCL_0;
//    for(t=0;t<8;t++)
//    {              

//		if((txd&0x80)>>7)
//			IIC_SDA_1;
//		else
//			IIC_SDA_0;
//		txd<<=1; 	  
//		Delay_us(2);   
//		IIC_SCL_1;
//		Delay_us(2); 
//		IIC_SCL_0;	
//		Delay_us(2);
//    }	 
//}   
//  
//u8 IIC_Read_Byte(unsigned char ack)
//{

//	u16 i;
//	u16 receive=0;
//	SDA_IN();
//  for(i=0;i<11;i++ )
//	{
//      IIC_SCL_0; 
//      Delay_us(2);
//			IIC_SCL_1;
//      receive<<=1;
//      if(READ_SDA)receive++;   
//		  Delay_us(1); 
//  }					 
//  if (!ack)
//     IIC_NAck();
//  else
//     IIC_Ack();   
//  return receive;
//}

