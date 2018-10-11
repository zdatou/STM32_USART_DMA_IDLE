#include "task.h"
#include "led.h"
#include "key.h"
#include "usart.h"
#include "oled.h"

void *USART_Task(void *msg);
void *LED_Task(void *msg);
void *OLED_Task(void *msg);
void *KEY_Task(void *msg);
void *RG_AD_Task(void *msg);
void *DHT11_Task(void *msg);

u8 now_task_num = 0; 
u32 sys_time = 0;
stTask TaskBuff[] = {
	{0, 0, 500, 1,  USART_Task, NULL}, 
	{0, 0, 500,  1,  LED_Task, NULL},
	{0, 0, 220,  1,  OLED_Task, NULL},
	{0, 0, 20,   1,  KEY_Task, NULL},
	{0, 0, 5000,   1,  RG_AD_Task, NULL},
	{0, 0, 100,  1,  DHT11_Task, NULL},
};


void InitTask(void)
{
	now_task_num = sizeof(TaskBuff)/sizeof(TaskBuff[0]);
}

//执行任务
void TaskRun(void)
{
	u8 i;
	for(i = 0; i < now_task_num; i++)
	{
		if(TaskBuff[i].state == 1)
		{
			TaskBuff[i].state = 0;
			if(TaskBuff[i].TaskCallBack != NULL)
			{
				 TaskBuff[i].TaskCallBack(TaskBuff[i].msg);
			}
		}
	}
}

HAL_StatusTypeDef stat;

void *USART_Task(void *msg)
{
	receive_str_manage(&usart2, 0, 0);
	return NULL;
}

u8 Led_index = 0;
void *LED_Task(void *msg)
{
	LED0 = !LED0;
	return NULL;
}

extern __align(4) u8 OLED_GRAM[64][32];
extern unsigned char   ASC5X8[];
void *DHT11_Task(void *msg)
{
	//Asc6_12(150,0 ,"6*12 ASCII") ;	   // 6*12点阵ASCII码
//	u8 i = 0;
//	for(i = 0; i < 8; i++)
//	{
//		OLED_GRAM[i][0] = ASC5X8[('h'-' ')*8+i];
//	}
//	
//	for(i = 0; i < 8; i++)
//	{
//		OLED_GRAM[i][2] = ASC5X8[('e'-' ')*8+i];
//	}
	
	//OLED_ShowString(0, 0, "hello", 8, 1);
	//OLED_ShowString(0, 12, "hello", 12, 1);
	//OLED_ShowString(0, 36, "hello", 16, 1);
	//OLED_ShowString(100, 0, "hello", 24, 1);
	//OLED_ShowString(100, 24, "hello", 40, 1);
	OLED_ShowText(0, 0, "主菜单", 12, 1, 0xff);
//	OLED_DrawPoint(0, 0, 1);
//	OLED_DrawPoint(1, 1, 1);
//	OLED_DrawPoint(2, 2, 1);
//	OLED_DrawPoint(3, 3, 1);
//	OLED_DrawPoint(4, 4, 1);
//	OLED_DrawPoint(5, 5, 1);
	OLED_Refresh_Gram();
	return NULL;
}

void *OLED_Task(void *msg)
{
	return NULL;
}

void *KEY_Task(void *msg)
{ 
	return NULL;
}

void *RG_AD_Task(void *msg)
{
	//OLED_Refresh_Gram();
	return NULL;
}

//1ms执行一次
void TaskTick(void)
{
	u8 i;
	sys_time++;
	for(i = 0; i < now_task_num; i ++)
	{
		 if(TaskBuff[i].enable == 1)
		 {
		 	 TaskBuff[i].cnt ++;
			 if(TaskBuff[i].cnt >= TaskBuff[i].aim_val)
			 {
			 	TaskBuff[i].cnt = 0;
				TaskBuff[i].state = 1;
			 }
		 }
	}
}

