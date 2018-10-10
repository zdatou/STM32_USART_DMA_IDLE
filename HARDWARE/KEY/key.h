#ifndef _KEY_H
#define _KEY_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F7������
//KEY��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/11/27
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////


typedef enum {
	Key_S0 = 0x01, 
	Key_S1 = 0x02,
	Key_S2 = 0x03,
	Key_S3 = 0x04,
	Key_S4 = 0x05,
	Key_S5 = 0X06
} KeyFlagStatus; 


typedef struct _ST_KEY_
{
	u16 Key_Flag;//������־
	u16 Key_Count;//��������
	u8  Key_Value;//����ֵ
	u8  Key_Tri;//��������
	u8  Key_Cnt;//�������� 
} St_Key;


#define KEY0        1
#define KEY1       1
#define KEY2        1
#define WK_UP      1

#define KEY0_PRES 	1
#define KEY1_PRES	2
#define KEY2_PRES	3
#define WKUP_PRES   4

extern St_Key Key_Proc;

void KEY_Init(void);
u8 KEY_Scan(u8 mode);
void KeyRead(void);
#endif
