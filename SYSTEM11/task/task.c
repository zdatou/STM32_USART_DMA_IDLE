#include "sys.h"
#include "usart_t.h"
#include "task.h"
#include "LED.h"
#include "key.h"
#include "DisDemo.h"

#define SYSTEMTICK_PERIOD_MS  1


static void USART_CallBack(void);
static void LED_CallBack(void);
static void Task_Poll(uint32_t time,Task p[],uint8_t Task_Num);
static void Key_Callback(void);

Task List[] = {
	0, 500, 	 LED_CallBack,
	0, 10,      USART_CallBack,
	0, 40,      Key_Callback
};

void run_task(void)
{
	uint8_t TaskNum = sizeof(List)/sizeof(List[0]);
	
	while(1)
	{
		Task_Poll(HAL_GetTick(),List,TaskNum);	
	}
}


void Task_Poll(uint32_t time,Task p[],uint8_t Task_Num)
{
	uint8_t i = 0;
	for(i = 0 ; i < Task_Num ; i++)
	{
		if( (time - p[i].Count ) >=  p[i].Period )
		{
			p[i].Count = time;
		   (p[i].CallBackFuntion)();
		}
	}
}

void Key_Callback(void) //×ÊÍ
{
	KeyRead();
	//DisDemo(Key_Proc.Key_Tri);
}

void USART_CallBack(void)
{
	receive_str_manage(&usart2,NULL,0);
}

void LED_CallBack(void)
{
	LED0_Toggle;
}

