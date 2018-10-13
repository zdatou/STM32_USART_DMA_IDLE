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

typedef struct _ST_KEY_
{
	u16 Key_Flag;//按键标志
	u16 Key_Count;//按键计数
	u8  Key_Value;//按键值
	u8  Key_Tri;//按键触发
	u8  Key_Cnt;//按键连按 
} St_Key;


#define KEY_UP        HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_15)
#define KEY_DOWN      HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_14)
#define KEY_RIGHT     HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_11)
#define KEY_LEFT      HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_12)
#define KEY_OK        HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_13)
#define KEY_CANNCE    HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_10)


#define KEY_UP_PRES 	1
#define KEY_DOWN_PRES	2
#define KEY_RIGHT_PRES	3
#define KEY_LEFT_PRES   4
#define KEY_OK_PRES		5
#define KEY_CANNCE_PRES 6

#define BEEP(n) (n)?HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET)

extern St_Key Key_Proc;

void KEY_Init(void);
u8 KEY_Scan(u8 mode);
void KeyRead(void);
void Beep(void);
#endif
