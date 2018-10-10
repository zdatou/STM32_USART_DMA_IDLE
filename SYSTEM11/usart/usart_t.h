#ifndef _USART_H_
#define _USART_H_
#include "sys.h"

#define USART_RECEIVE_MAX 555
#define USART_DMA_ENABLE 0
	
typedef int (*USART_Func)(void *, u16, void *, u8);
	
typedef struct _st_usart
{
	UART_HandleTypeDef    	USART_Handler;
	u16 			   		receive_count;
	u8 						*buff;
	USART_Func 				func;
	u8						buff1[USART_RECEIVE_MAX];
#if USART_DMA_ENABLE
	u8						*DMA_buff;  //使用DMA时 需要使用两个缓存  一个用来保存串口接收的数据  一个用户使用
	u8						buff2[USART_RECEIVE_MAX];//双缓存  避免内存拷贝
	DMA_HandleTypeDef       DMA_TX_Handler;
	DMA_HandleTypeDef       DMA_RX_Handler;
#else
	u8			 			temp_buff[8];    //不使用DMA时 使用一个较小的临时数组存放接收的一个字符或多个字符
#endif
}STUsart, *PSTUsart;
	
	
void com_init(PSTUsart com);


u8 receive_str_manage(PSTUsart com, void *store, u8 count);

HAL_StatusTypeDef com_send_byte(PSTUsart com,u8 byte);
HAL_StatusTypeDef com_send_pstr(PSTUsart com,void *p);
HAL_StatusTypeDef com_send_str(PSTUsart com,void* str,u16 len);



extern STUsart usart3, usart1, usart2;


#endif/*_USART_H*/
