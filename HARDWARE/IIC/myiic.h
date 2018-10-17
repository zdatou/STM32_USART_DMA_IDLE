#ifndef _MYIIC_H
#define _MYIIC_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F103������
//IIC��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2017/5/30
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
//IO��������
#define SDA_IN()  {GPIOD->CRL&=0XFFF0FFFF;GPIOD->CRL|=(u32)8<<16;}	//PB7����ģʽ
#define SDA_OUT() {GPIOD->CRL&=0XFFF0FFFF;GPIOD->CRL|=(u32)3<<16;} 	//PB7���ģʽ

//IO����
#define IIC_SCL   PDout(5) //SCL
#define IIC_SDA   PDout(4) //SDA
#define READ_SDA  PDin(4)  //����SDA



////IO��������
//#define SDA_IN()  {GPIOE->CRH&=0XFFFFFFF0;GPIOE->CRH|=(u32)8<<0;}	//PB7����ģʽ
//#define SDA_OUT() {GPIOE->CRH&=0XFFFFFFF0;GPIOE->CRH|=(u32)3<<0;} 	//PB7���ģʽ

////IO����
//#define IIC_SCL   PEout(9) //SCL
//#define IIC_SDA   PEout(8) //SDA
//#define READ_SDA  PEin(8)  //����SDA


////IO��������
//#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}	//PB7����ģʽ
//#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;} 	//PB7���ģʽ

////IO����
//#define IIC_SCL   PBout(6) //SCL
//#define IIC_SDA   PBout(7) //SDA
//#define READ_SDA  PBin(7)  //����SDA

//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	 
#endif

