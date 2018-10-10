#include "sys.h"   
#include "usart_t.h"
#include "string.h"
#include "stdarg.h"
#include "stdio.h"

STUsart usart3, usart1, usart2;


//初始化串口
void com_init(PSTUsart com)
{
	com->USART_Handler.Init.HwFlowCtl     		   = UART_HWCONTROL_NONE;
	com->USART_Handler.Init.OverSampling  		   = UART_OVERSAMPLING_16;
	com->USART_Handler.Init.StopBits         	   = UART_STOPBITS_1;
	com->USART_Handler.Init.WordLength     		   = UART_WORDLENGTH_8B;	
	com->USART_Handler.Init.Mode         		   = UART_MODE_TX_RX;
	com->USART_Handler.Init.OneBitSampling 		   = UART_ONE_BIT_SAMPLE_DISABLE;
	com->USART_Handler.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	HAL_UART_Init(&com->USART_Handler);
	com->buff = com->buff1;
#if USART_DMA_ENABLE
	com->DMA_buff = com->buff2;	
	__HAL_UART_ENABLE_IT(&com->USART_Handler, UART_IT_IDLE);    //使能空闲中断
	HAL_UART_Receive_DMA(&com->USART_Handler, com->DMA_buff, USART_RECEIVE_MAX); //开启DMA接收 非原版库函数  删掉了半传输完成中断
#else
	if(com->USART_Handler.Instance != USART2)
		HAL_UART_Receive_IT(&com->USART_Handler, com->temp_buff, 1);
#endif
}

//		串口数组数据处理函数
u8 receive_str_manage(PSTUsart com, void *store, u8 count)
{
	u8 tmp = 0;
#if USART_DMA_ENABLE
	if(com->receive_count != 0)
	{
		tmp = com->func(com->buff, com->receive_count, store, count);
		com->receive_count = 0;
	}
#else	
	static u32 usart1_tmp = 0;
	static u32 usart2_tmp = 0;

	static u8 usart1_state = 0;
	static u8 usart2_state = 0;

	if(com->USART_Handler.Instance == USART1)
	{
		if(com->receive_count != usart1_tmp)
		{	
			usart1_tmp = com->receive_count;
			usart1_state++;
			return tmp;
		}
		if(usart1_state > 0)
		{
			usart1_tmp = 0;
			usart1_state = 0;
			tmp = com->func(com->buff, com->receive_count, store, count);	
			com->receive_count = 0;			
		}
	}
	else if(com->USART_Handler.Instance == USART2)
	{
		if(com->receive_count != usart2_tmp)
		{	
			usart2_tmp = com->receive_count;
			usart2_state++;
			return tmp;
		}
		if(usart2_state > 0)
		{	
			usart2_tmp = 0;
			usart2_state = 0;
		
			tmp = com->func(com->buff, com->receive_count, store, count);	
			com->receive_count = 0;			
		}
	}
#endif
	return tmp;		
}

//发送字符函数
HAL_StatusTypeDef com_send_str(PSTUsart com,void* str,u16 len)
{	
#if USART_DMA_ENABLE
	uint8_t Timeout = 10;
	uint32_t Tickstart = HAL_GetTick();
	while(HAL_UART_GetState(&com->USART_Handler) == HAL_UART_STATE_BUSY_TX)
	{
      if(((HAL_GetTick()-Tickstart) > Timeout))
      {
        return HAL_TIMEOUT;
      }	
	}	
	return HAL_UART_Transmit_DMA(&usart4.USART_Handler, (uint8_t *)(str), len);
#else
	return HAL_UART_Transmit(&com->USART_Handler, (uint8_t *)str, len, 1000);
#endif	
}

//发送一个字符
HAL_StatusTypeDef com_send_byte(PSTUsart com,u8 byte)
{
#if USART_DMA_ENABLE	
	uint8_t Timeout = 10;
	uint32_t Tickstart = HAL_GetTick();
	while(HAL_UART_GetState(&com->USART_Handler) == HAL_UART_STATE_BUSY_TX)
	{
      if(((HAL_GetTick()-Tickstart) > Timeout))
      {
        return HAL_TIMEOUT;
      }	
	}
#endif
	while((com->USART_Handler.Instance->ISR & UART_FLAG_TXE) == RESET)
	{
		;
	}
	com->USART_Handler.Instance->TDR = byte;
	return HAL_OK;
}


HAL_StatusTypeDef com_send_pstr(PSTUsart com,void *p)
{	
#if USART_DMA_ENABLE
	uint8_t Timeout = 10;
	uint32_t Tickstart = HAL_GetTick();
	while(HAL_UART_GetState(&com->USART_Handler) == HAL_UART_STATE_BUSY_TX)
	{
      if(((HAL_GetTick()-Tickstart) > Timeout))
      {
        return HAL_TIMEOUT;
      }	
	}
#endif
	char * str = (char *)p;
	while('\0' != *str)
	{
		while((com->USART_Handler.Instance->ISR & UART_FLAG_TXE) == RESET)
		{
			;
		}
		com->USART_Handler.Instance->TDR = *str++;
	}
	
	while((com->USART_Handler.Instance->ISR & UART_FLAG_TXE) == RESET)
	{
		;
	}
	return HAL_OK;
}
