#include "task.h"
#include "led.h"
#include "key.h"
#include "usart.h"

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
	{0, 0, 57,   1,  RG_AD_Task, NULL},
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
	//stat = com_send_str(&usart2, "hello\r\n", 7);
	receive_str_manage(&usart2, 0, 0);
	return NULL;
}

u8 Led_index = 0;
void *LED_Task(void *msg)
{
	LED0 = !LED0;
	return NULL;
}

void *DHT11_Task(void *msg)
{
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

