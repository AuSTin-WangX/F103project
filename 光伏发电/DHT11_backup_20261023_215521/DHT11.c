#include "DHT11.h"
#include "Delay.h"
#include "OLED.h"
// #include "usart.h"
 
u8 receive_R_dat[10];//����һ��������
u8 receive_T_dat[10];//����һ��������
 
void DHT11_Init(void)  //��������ʼ��
{
 
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹGPIO_Eʱ
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //PE.12
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;	//ͨ
  GPIO_Init(GPIOB, &GPIO_InitStructure);//ʼGPIOE12
}
 
void DHT11_Start(void)  //������ʼ�ź�
	
{
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
	Delay_us(10);  //���ߵ�ƽ�ȴ�
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	Delay_ms(25);  //�͵�ƽ����ʱ�䲻С��18MS������30MS
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
	Delay_us(30); //���ߵ�ƽ������ʼ�ź�
	
}
 
u8 DHT11_receive_bit(void)  //������������Ӧ�źš�����һ���ֽ�
{ 
	u8 i,dat=0;
	GPIO_ResetBits(GPIOB,GPIO_Pin_12); //����
	Delay_us(81); 
	GPIO_SetBits(GPIOB,GPIO_Pin_12); //
	Delay_us(85);
	//��ʼ׼����������
	for(i=0;i<8;i++)
	{
		PEout(12)=0; //���ͳ���83US
		Delay_us(50);
		PEout(12)=1; //���߳���
		Delay_us(60);
		dat<<=1;
		if(PEout(12)==1) //����ߵ�ƽʱ�䳬��60us��Ϊ��1��״̬
			dat+=1;
		
		while(PEout(12));
	}
	return dat; //��������
}
 
void DHT11_receive_dat(void)
{
	u8 R_H,R_L,T_H,T_L,RH,RL,TH,TL,Checkout;
	
	DHT11_Start();//��ʼ��ʼ��Ӧ������40λ����
	
	if(PEout(12)==0)
	{
		while(PEout(12)==0); //�ȴ�����
		Delay_us(80); //���ߺ���ʱ80us
		R_H=DHT11_receive_bit();
		R_L=DHT11_receive_bit();
		T_L=DHT11_receive_bit();
		T_L=DHT11_receive_bit();
		Checkout=DHT11_receive_bit();
		
		Delay_us (50);
			
	}
	/*У������*/
	if((R_H+R_L+T_H+T_L)==Checkout)
	{
		R_H=RH;
		R_L=RL;
		T_H=TH;
		T_L=TL;	
	}
	
	/*���ݴ���*/
 
	receive_R_dat[1]=RH/10+0x30;
	receive_R_dat[2]=RH%10+0x30;
	receive_R_dat[3]='.';
	receive_R_dat[4]=RL/10+0x30;
	receive_R_dat[5]=RL%10+0x30;
	receive_R_dat[6]='%';
	
 
	receive_T_dat[1]=TH/10+0x30;
	receive_T_dat[2]=TH%10+0x30;
	receive_T_dat[3]='.';
	receive_T_dat[4]=TL/10+0X30;
	receive_T_dat[5]=TL%10+0X30;
	receive_T_dat[6]='^';
	receive_T_dat[7]='C';	
	
	OLED_ShowString(1,1,receive_R_dat);
	
	// printf ("Humidity:\r\n");
	// MyUSART_SendArr(receive_R_dat,7);
	// printf("\r\n");
	// printf ("Temperture:\r\n");
	// MyUSART_SendArr(receive_T_dat,8);
	// printf("\r\n");
}
 
 