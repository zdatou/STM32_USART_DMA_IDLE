#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "task.h"
#include "timer.h"
#include "oled.h"
#include "ad5680.h"
#include "ad5325.h"
#include "24cxx.h"



long long value[8];

long long reference_indexmean[8]={0};
long long stopresultmean[8]={0};

extern long long reference_index[8][4];
extern long long stopresult[8][4];

unsigned char distable[8][32];
unsigned char distemp[32];

unsigned char disset[2][30];

u8 channel[8]={0};
u8 channelcount=0;

long long ct=0;
u8 keyvalue = 0;
u8 pset = 0;

u32 counter_halfs=0;
u8 oscclkerror=0;
u8 refclkerror=0;
u8 clksource = 1;
u8 clksourcetemp = 1;
extern u8 LED[2];
u8 clkerrorflag = 0;
u8 menuselect=0;
u8 refchannel_temp=1;
u8 senddata[200]="$XHTSXH0310020000000000000139";
u8 extref_flag = 0;
u8 noinput = 0;
u8 times=0;
u8 refchannel=1;


void SystemClock_Config(void);
void usart_init(void);
int usart2_call_back(void * str, u16 len, void * store, u16 count);

int main(void)
{
	HAL_Init();  //中断分组2
	Stm32_Clock_Init(RCC_PLL_MUL9);   	//设置时钟,72M
	delay_init(72);
	LED_Init();
	KEY_Init();
	TIM_Init(6, 1000, 72);
	TIM_Init(7, 1000, 72);
	TIM_Base_Start(6);
	usart_init();
	InitTask();
	OLED_Init();
	AD5325_Init();
	AD5325_Test();
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


