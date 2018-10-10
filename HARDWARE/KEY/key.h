#ifndef _KEY_H
#define _KEY_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F7开发板
//KEY驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/11/27
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
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
	u16 Key_Flag;//按键标志
	u16 Key_Count;//按键计数
	u8  Key_Value;//按键值
	u8  Key_Tri;//按键触发
	u8  Key_Cnt;//按键连按 
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
