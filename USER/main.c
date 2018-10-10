#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "task.h"
#include "timer.h"

void SystemClock_Config(void);
void usart_init(void);
int usart2_call_back(void * str, u16 len, void * store, u16 count);

int main(void)
{
	HAL_Init();  //中断分组2
	Stm32_Clock_Init(RCC_PLL_MUL9);   	//设置时钟,72M
	delay_init(72);
	LED_Init();
	TIM3_Init(1000, 72);
	usart_init();
	InitTask();
	
	while (1)
	{
		TaskRun();
	}
}

void usart_init(void)
{
	usart2.USART_Handler.Instance = USART2;
	usart2.USART_Handler.Init.BaudRate = 115200;
	usart2.USART_Handler.Init.Parity = UART_PARITY_NONE;
	usart2.func = usart2_call_back;
	com_init(&usart2);
}

int usart2_call_back(void * str, u16 len, void * store, u16 count)
{
	int ret = 0;
	com_send_str(&usart2, str, len);
	return ret;
}


