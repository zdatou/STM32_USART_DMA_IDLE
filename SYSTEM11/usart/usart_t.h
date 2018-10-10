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
	u8						*DMA_buff;  //ʹ��DMAʱ ��Ҫʹ����������  һ���������洮�ڽ��յ�����  һ���û�ʹ��
	u8						buff2[USART_RECEIVE_MAX];//˫����  �����ڴ濽��
	DMA_HandleTypeDef       DMA_TX_Handler;
	DMA_HandleTypeDef       DMA_RX_Handler;
#else
	u8			 			temp_buff[8];    //��ʹ��DMAʱ ʹ��һ����С����ʱ�����Ž��յ�һ���ַ������ַ�
#endif
}STUsart, *PSTUsart;
	
	
void com_init(PSTUsart com);


u8 receive_str_manage(PSTUsart com, void *store, u8 count);

HAL_StatusTypeDef com_send_byte(PSTUsart com,u8 byte);
HAL_StatusTypeDef com_send_pstr(PSTUsart com,void *p);
HAL_StatusTypeDef com_send_str(PSTUsart com,void* str,u16 len);



extern STUsart usart3, usart1, usart2;


#endif/*_USART_H*/
