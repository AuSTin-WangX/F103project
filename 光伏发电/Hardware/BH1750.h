#ifndef __BH1750_H
#define __BH1750_H

#include "stm32f10x.h"

/* BH1750 ������ַ (ADDR���Žӵ�) */
#define BH1750_ADDR_WRITE  0x46  // 0x23 << 1 | 0
#define BH1750_ADDR_READ   0x47  // 0x23 << 1 | 1

/* BH1750 ָ� */
#define BH1750_POWER_DOWN  0x00  // �ϵ�
#define BH1750_POWER_ON    0x01  // �ϵ�
#define BH1750_RESET       0x07  // �������ݼĴ���
#define BH1750_CONT_H_RES  0x10  // �����߷ֱ���ģʽ (1 lx)
#define BH1750_CONT_H_RES2 0x11  // �����߷ֱ���ģʽ2 (0.5 lx)
#define BH1750_CONT_L_RES  0x13  // �����ͷֱ���ģʽ (4 lx)
#define BH1750_ONCE_H_RES  0x20  // ���θ߷ֱ���ģʽ (�������Զ��ϵ�)

/* ����ӿ�?*/
void BH1750_Init(void);
uint16_t BH1750_Read_Lux(void);

#endif
