#include "key.h"
#include "delay.h"
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
St_Key Key_Proc;
static u8 Beep_Flag = 0;
//按键初始化函数
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOD_CLK_ENABLE();            //开启GPIOD时钟
	__HAL_RCC_GPIOE_CLK_ENABLE();            //开启GPIOE时钟

    GPIO_Initure.Pin=GPIO_PIN_15|GPIO_PIN_14|GPIO_PIN_13|GPIO_PIN_12|GPIO_PIN_11|GPIO_PIN_10;//PA0
    GPIO_Initure.Mode=GPIO_MODE_INPUT;       //输入
    GPIO_Initure.Pull=GPIO_PULLUP;         //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH; //高速
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);
	
	GPIO_Initure.Pin = GPIO_PIN_14;
	GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(GPIOD, &GPIO_Initure);
}

//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，WKUP按下 WK_UP
//注意此函数有响应优先级,KEY0>KEY1>KEY2>WK_UP!!
u8 KEY_Scan(u8 mode)
{
    static u8 key_up=1;     //按键松开标志
    if(mode==1)key_up=1;    //支持连按
    if(key_up&&(KEY_UP==0||KEY_DOWN==0||KEY_RIGHT==0||KEY_LEFT==0||KEY_OK==0||KEY_CANNCE==0))
    {
        key_up=0;
        if(KEY_UP==0)       return KEY_UP_PRES;
        else if(KEY_DOWN == 0)  return KEY_DOWN_PRES;
        else if(KEY_RIGHT == 0)  return KEY_RIGHT_PRES;
        else if(KEY_LEFT == 0) return KEY_LEFT_PRES;   
		else if(KEY_OK == 0) return KEY_OK_PRES;
		else if(KEY_CANNCE == 0) return KEY_CANNCE_PRES;
    }else if(KEY_UP==1&&KEY_DOWN==1&&KEY_RIGHT==1&&KEY_LEFT==1&&KEY_OK==1&&KEY_CANNCE==1)key_up=1;
    return 0;   //无按键按下
}

void KeyRead(void)//20ms执行一次
{
	unsigned char ReadData;
	ReadData= KEY_Scan(1);
	Key_Proc.Key_Tri = ReadData&(ReadData^Key_Proc.Key_Cnt);//边沿检测（2次检测）
	Key_Proc.Key_Cnt = ReadData;
	if(Key_Proc.Key_Tri)
	{
		BEEP(1);
		Beep_Flag = 1;
	}
}

void Beep(void)
{
	static u8 beepvalue = 0;
	if(Beep_Flag == 1)
	{
		beepvalue++;
		if(beepvalue > 2)
		{
			beepvalue = 0;
			Beep_Flag = 0;
			BEEP(0);
		}
	}
}
