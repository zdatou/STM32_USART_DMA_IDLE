#include "sys.h"
#include "usart.h"	

//������Ƶ�ʽϸ�ʱ˫���������⣬

uint8_t Timeout = 10;

#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART2->SR&0X40)==0)//ѭ������,ֱ���������   
	{
		;
	}
    USART2->DR = (u8) ch;      
	return ch;
}
#endif 

STUsart usart2, usart3, usart1, usart4;

//��ʼ������
void com_init(PSTUsart com)
{
	com->USART_Handler.Init.HwFlowCtl     		   = UART_HWCONTROL_NONE;
	com->USART_Handler.Init.OverSampling  		   = UART_OVERSAMPLING_16;
	com->USART_Handler.Init.StopBits         	   = UART_STOPBITS_1;
	com->USART_Handler.Init.WordLength     		   = UART_WORDLENGTH_8B;	
	com->USART_Handler.Init.Mode         		   = UART_MODE_TX_RX;
	HAL_UART_Init(&com->USART_Handler);
	com->buff = com->buff1;
#if USART_DMA_ENABLE
	com->DMA_buff = com->buff2;	
	__HAL_UART_ENABLE_IT(&com->USART_Handler, UART_IT_IDLE);
	HAL_UART_Receive_DMA(&com->USART_Handler, com->DMA_buff, USART_RECEIVE_MAX);
#else
	HAL_UART_Receive_IT(&com->USART_Handler, com->temp_buff, 1);
#endif
}

//�����������ݴ�����
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
	static u32 usart3_tmp = 0;

	static u8 usart1_state = 0;
	static u8 usart2_state = 0;
	static u8 usart3_state = 0;
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
	else if(com->USART_Handler.Instance == USART3)
	{
		if(com->receive_count != usart3_tmp)
		{	
			usart3_tmp = com->receive_count;
			usart3_state++;
			return tmp;
		}
		if(usart3_state > 0)
		{		
			usart3_tmp = 0;
			usart3_state = 0;
			tmp = com->func(com->buff, com->receive_count, store, count);		
			com->receive_count = 0;			
		}
	}
#endif
	return tmp;		
}

//�����ַ�����
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
	return HAL_UART_Transmit_DMA(&com->USART_Handler, (uint8_t *)(str), len);
#else
	return HAL_UART_Transmit(&com->USART_Handler, (uint8_t *)str, len, 1000);
#endif	
}

//����һ���ַ�
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
	while((com->USART_Handler.Instance->SR & UART_FLAG_TXE) == RESET)
	{
		;
	}
	com->USART_Handler.Instance->DR = byte;
	return HAL_OK;
}

HAL_StatusTypeDef com_send_pstr(PSTUsart com,void *p)
{	
#if USART_DMA_ENABLE
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
		while((com->USART_Handler.Instance->SR & UART_FLAG_TXE) == RESET)
		{
			;
		}
		com->USART_Handler.Instance->DR = *str++;
	}
	
	while((com->USART_Handler.Instance->SR & UART_FLAG_TXE) == RESET)
	{
		;
	}
	return HAL_OK;
}

/***********************�жϻص�����************************************************/
/***********************�жϻص�����************************************************/
/***********************�жϻص�����************************************************/
/***********************�жϻص�����************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) //������ɻص�����  ��ʱδ����Ƶ�����͵����  ������Ƶ�ʽϸ�ʱ ˫������������ ��������������
{
#if USART_DMA_ENABLE
	if(USART1 == huart->Instance)
	{
		u8 *temp;
		usart1.receive_count = USART_RECEIVE_MAX; //�����ѽ��յ��ֽ���  ������DMA������˺���ʱ ˵��DMA������ɣ� ���յ��ֽ���Ϊ֮ǰ���õ��ֽ���

		temp            = usart1.DMA_buff;   //��������  �����ڴ濽��
		usart1.DMA_buff = usart1.buff;
		usart1.buff     = temp;

		HAL_UART_Receive_DMA(&usart1.USART_Handler, (uint8_t *)usart1.DMA_buff, USART_RECEIVE_MAX); //��������DMA����
	}
	else if(USART2 == huart->Instance)
	{
		u8 *temp;
		usart2.receive_count = USART_RECEIVE_MAX; //�����ѽ��յ��ֽ���
		
		temp            = usart2.DMA_buff;   //��������  �����ڴ濽��
		usart2.DMA_buff = usart2.buff;
		usart2.buff     = temp;

		HAL_UART_Receive_DMA(&usart2.USART_Handler, (uint8_t *)usart2.DMA_buff, USART_RECEIVE_MAX); //��������DMA����
	}
	else if(USART3 == huart->Instance)
	{
		u8 *temp;
		usart3.receive_count = USART_RECEIVE_MAX; //�����ѽ��յ��ֽ���
		
		temp            = usart3.DMA_buff;   //��������  �����ڴ濽��
		usart3.DMA_buff = usart3.buff;
		usart3.buff     = temp;

		HAL_UART_Receive_DMA(&usart3.USART_Handler, (uint8_t *)usart3.DMA_buff, USART_RECEIVE_MAX); //��������DMA����
	}
	else if(UART4 == huart->Instance)
	{
		u8 *temp;
		usart4.receive_count = USART_RECEIVE_MAX; //�����ѽ��յ��ֽ���
		
		temp 			= usart4.DMA_buff; //��������  �����ڴ濽��
		usart4.DMA_buff = usart4.buff;
		usart4.buff     = temp;
		
		HAL_UART_Receive_DMA(&usart4.USART_Handler, (uint8_t *)usart3.DMA_buff, USART_RECEIVE_MAX); //��������DMA����
	}
#else
	if(USART1 == huart->Instance)
	{
		(usart1.buff)[usart1.receive_count++] = usart1.temp_buff[0];
		usart1.receive_count %= USART_RECEIVE_MAX;
	}
	else if(USART2 == huart->Instance)
	{
		(usart2.buff)[usart2.receive_count++] = usart2.temp_buff[0];
		usart2.receive_count %= USART_RECEIVE_MAX;
	}
	else if(USART3 == huart->Instance)
	{
		(usart3.buff)[usart3.receive_count++] = usart3.temp_buff[0];
		usart3.receive_count %= USART_RECEIVE_MAX;
	}
	else if(UART4 == huart->Instance)
	{
		(usart4.buff)[usart4.receive_count++] = usart4.temp_buff[0];
		usart4.receive_count %= USART_RECEIVE_MAX;
	}
#endif
}

/******************�жϳ���*******************************/
/******************�жϳ���*******************************/
/******************�жϳ���*******************************/
/******************�жϳ���*******************************/
/******************�жϳ���*******************************/
/****************�����жϷ�����***********************/
void USART1_IRQHandler(void)                	//����1�жϷ������
{
	HAL_UART_IRQHandler(&usart1.USART_Handler);	
#if USART_DMA_ENABLE 
	u8 *temp;
	if((USART1->SR & USART_SR_IDLE) != RESET)                                                                  	
	{                                                                                                            	
		__HAL_UART_CLEAR_IDLEFLAG(&usart1.USART_Handler);       //��������ж�                                                              	
		usart1.receive_count = USART_RECEIVE_MAX - usart1.DMA_RX_Handler.Instance->CNDTR;   //�����ѽ��յ��ֽ���                     	
		HAL_UART_DMAStop(&usart1.USART_Handler);             //��ֹDMA����                                                  	
		                                                                                                         	
		temp = usart1.DMA_buff;                          //��������  �����ڴ濽��                                                         	
		usart1.DMA_buff = usart1.buff;                                                                           	
		usart1.buff = temp;                                                                                      	
		                                                                                                         	
		HAL_UART_Receive_DMA(&usart1.USART_Handler, (uint8_t *)usart1.DMA_buff, USART_RECEIVE_MAX);   //��������DMA����           	
	}                                                                                                            	
#else                                                                                                            	
	HAL_UART_Receive_IT(&usart1.USART_Handler, usart1.temp_buff, 1);
#endif
} 

void USART2_IRQHandler(void)                	//����2�жϷ������
{
	HAL_UART_IRQHandler(&usart2.USART_Handler);
#if USART_DMA_ENABLE
	u8 *temp;
	if((USART2->SR & USART_SR_IDLE) != RESET)
	{
		__HAL_UART_CLEAR_IDLEFLAG(&usart2.USART_Handler);       //��������ж�        
		usart2.receive_count    = USART_RECEIVE_MAX - usart2.DMA_RX_Handler.Instance->CNDTR; //�����ѽ��յ��ֽ���  
		HAL_UART_DMAStop(&usart2.USART_Handler);      //��ֹDMA����  
		
		temp            = usart2.DMA_buff;		//��������  �����ڴ濽��   
		usart2.DMA_buff = usart2.buff;
		usart2.buff     = temp;
		
		HAL_UART_Receive_DMA(&usart2.USART_Handler, (uint8_t *)usart2.DMA_buff, USART_RECEIVE_MAX);  //��������DMA����   
	}
#else
	HAL_UART_Receive_IT(&usart2.USART_Handler, (uint8_t *)&usart2.temp_buff[0], 1);
#endif
} 

void USART3_IRQHandler(void)                	//����3�жϷ������
{
	HAL_UART_IRQHandler(&usart3.USART_Handler);
#if USART_DMA_ENABLE	
	u8 *temp;
	if((USART3->SR & USART_SR_IDLE) != RESET) //�����ж�
	{
		__HAL_UART_CLEAR_IDLEFLAG(&usart3.USART_Handler);       //��������ж�   
		usart3.receive_count = USART_RECEIVE_MAX - usart3.DMA_RX_Handler.Instance->CNDTR; //�����ѽ��յ��ֽ���
		HAL_UART_DMAStop(&usart3.USART_Handler);  //��ֹDMA����
		
		temp            = usart3.DMA_buff;   //��������  �����ڴ濽��
		usart3.DMA_buff = usart3.buff;	
		usart3.buff     = temp;
	
		HAL_UART_Receive_DMA(&usart3.USART_Handler, (uint8_t *)usart3.DMA_buff, USART_RECEIVE_MAX); //��������DMA����
	}
#else
	HAL_UART_Receive_IT(&usart3.USART_Handler, (uint8_t *)&usart3.temp_buff[0], 1); //���¿��������жϣ� ����һ���ַ�����ûص�����
#endif
} 

void UART4_IRQHandler(void) 					//����4�жϷ������
{
	HAL_UART_IRQHandler(&usart4.USART_Handler);
#if USART_DMA_ENABLE	
	u8 *temp;
	if((UART4->SR & USART_SR_IDLE) != RESET) //�����ж�
	{
		__HAL_UART_CLEAR_IDLEFLAG(&usart4.USART_Handler);       //��������ж�  
		usart4.receive_count = USART_RECEIVE_MAX - usart4.DMA_RX_Handler.Instance->CNDTR; //�����ѽ��յ��ֽ���
		HAL_UART_DMAStop(&usart4.USART_Handler);  //��ֹDMA����
		
		temp            = usart4.DMA_buff;   //��������  �����ڴ濽��
		usart4.DMA_buff = usart4.buff;
		usart4.buff     = temp;
	
		HAL_UART_Receive_DMA(&usart4.USART_Handler, (uint8_t *)usart4.DMA_buff, USART_RECEIVE_MAX); //��������DMA����
	}
#else
	HAL_UART_Receive_IT(&usart4.USART_Handler, (uint8_t *)&usart4.temp_buff[0], 1); //���¿��������жϣ� ����һ���ַ�����ûص�����
#endif
}

/****************DMA�жϷ�����***********************/
#if USART_DMA_ENABLE
void DMA1_Channel2_IRQHandler(void) //USART3_Tx
{
	HAL_DMA_IRQHandler(usart3.USART_Handler.hdmatx);
}
void DMA1_Channel3_IRQHandler(void) //USART3_RX
{
	HAL_DMA_IRQHandler(usart3.USART_Handler.hdmarx);
}
void DMA1_Channel4_IRQHandler(void) //USART1_TX
{
	HAL_DMA_IRQHandler(usart1.USART_Handler.hdmatx);	
}
void DMA1_Channel5_IRQHandler(void) //USART1_RX
{
	HAL_DMA_IRQHandler(usart1.USART_Handler.hdmarx);	
}
void DMA1_Channel6_IRQHandler(void) //USART2_RX
{
	HAL_DMA_IRQHandler(usart2.USART_Handler.hdmarx);
}
void DMA1_Channel7_IRQHandler(void) //USART2_TX
{
	HAL_DMA_IRQHandler(usart2.USART_Handler.hdmatx);
}
void DMA2_Channel3_IRQHandler(void) //UART4_RX
{
	HAL_DMA_IRQHandler(usart4.USART_Handler.hdmarx);
}
void DMA2_Channel5_IRQHandler(void) //UART4_TX
{
	HAL_DMA_IRQHandler(usart4.USART_Handler.hdmatx);
}
#endif

/*********************��ʼ���ص�����**********************************/
/*********************��ʼ���ص�����**********************************/
/*********************��ʼ���ص�����**********************************/
/*********************��ʼ���ص�����**********************************/
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	if(huart->Instance==USART1)
	{
		__HAL_RCC_USART1_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();

		GPIO_InitStruct.Pin = GPIO_PIN_9;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		
		GPIO_InitStruct.Pin = GPIO_PIN_10;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		
		HAL_NVIC_SetPriority(USART1_IRQn, 2,0);
		HAL_NVIC_EnableIRQ(USART1_IRQn);
#if USART_DMA_ENABLE
		__HAL_RCC_DMA1_CLK_ENABLE(); 
		
		usart1.DMA_TX_Handler.Instance         			= DMA1_Channel4;                
		usart1.DMA_TX_Handler.Init.Direction   			= DMA_MEMORY_TO_PERIPH;                     
		usart1.DMA_TX_Handler.Init.PeriphInc   			= DMA_PINC_DISABLE;                         
		usart1.DMA_TX_Handler.Init.MemInc      			= DMA_MINC_ENABLE;                          
		usart1.DMA_TX_Handler.Init.PeriphDataAlignment	= DMA_PDATAALIGN_BYTE;                      
		usart1.DMA_TX_Handler.Init.MemDataAlignment     = DMA_MDATAALIGN_BYTE;                      
		usart1.DMA_TX_Handler.Init.Mode                 = DMA_NORMAL;                               
		usart1.DMA_TX_Handler.Init.Priority             = DMA_PRIORITY_HIGH; 
		
		HAL_DMA_Init(&usart1.DMA_TX_Handler);                                                     
		__HAL_LINKDMA(huart, hdmatx, usart1.DMA_TX_Handler);                                      
		                                                                                             
		usart1.DMA_RX_Handler.Instance   				= DMA1_Channel5;                           
		usart1.DMA_RX_Handler.Init.Direction  			= DMA_PERIPH_TO_MEMORY;                    
		usart1.DMA_RX_Handler.Init.PeriphInc            = DMA_PINC_DISABLE;                        
		usart1.DMA_RX_Handler.Init.MemInc				= DMA_MINC_ENABLE;                         
		usart1.DMA_RX_Handler.Init.PeriphDataAlignment  = DMA_PDATAALIGN_BYTE;                     
		usart1.DMA_RX_Handler.Init.MemDataAlignment     = DMA_MDATAALIGN_BYTE;                     
		usart1.DMA_RX_Handler.Init.Mode 				= DMA_NORMAL;                              
		usart1.DMA_RX_Handler.Init.Priority 			= DMA_PRIORITY_HIGH;                                           
		                                                                                             
		HAL_DMA_Init(&usart1.DMA_RX_Handler);                                                   
		__HAL_LINKDMA(huart, hdmarx, usart1.DMA_RX_Handler);                                                                                       
		                                                                                            
		HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 2, 0);    //Tx                                         
		HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);   
		
		HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 2, 0);    //Rx                                         
		HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);  
#endif
	}
	else if(huart->Instance==USART2)
	{
		__HAL_RCC_USART2_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		
		GPIO_InitStruct.Pin = GPIO_PIN_2;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;	
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		
		GPIO_InitStruct.Pin = GPIO_PIN_3;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		
		HAL_NVIC_SetPriority(USART2_IRQn, 2, 1);
		HAL_NVIC_EnableIRQ(USART2_IRQn);
#if USART_DMA_ENABLE
		__HAL_RCC_DMA1_CLK_ENABLE();
		
		usart2.DMA_TX_Handler.Instance         			= DMA1_Channel7;
		usart2.DMA_TX_Handler.Init.Direction   			= DMA_MEMORY_TO_PERIPH;
		usart2.DMA_TX_Handler.Init.PeriphInc   			= DMA_PINC_DISABLE;
		usart2.DMA_TX_Handler.Init.MemInc      			= DMA_MINC_ENABLE;
		usart2.DMA_TX_Handler.Init.PeriphDataAlignment	= DMA_PDATAALIGN_BYTE;
		usart2.DMA_TX_Handler.Init.MemDataAlignment     = DMA_MDATAALIGN_BYTE;
		usart2.DMA_TX_Handler.Init.Mode                 = DMA_NORMAL;
		usart2.DMA_TX_Handler.Init.Priority             = DMA_PRIORITY_MEDIUM;
		
		HAL_DMA_Init(&usart2.DMA_TX_Handler);
		__HAL_LINKDMA(huart, hdmatx, usart2.DMA_TX_Handler);
		
		usart2.DMA_RX_Handler.Instance   				= DMA1_Channel6;
		usart2.DMA_RX_Handler.Init.Direction  			= DMA_PERIPH_TO_MEMORY;
		usart2.DMA_RX_Handler.Init.PeriphInc            = DMA_PINC_DISABLE;
		usart2.DMA_RX_Handler.Init.MemInc				= DMA_MINC_ENABLE;
		usart2.DMA_RX_Handler.Init.PeriphDataAlignment  = DMA_PDATAALIGN_BYTE;
		usart2.DMA_RX_Handler.Init.MemDataAlignment     = DMA_MDATAALIGN_BYTE;
		usart2.DMA_RX_Handler.Init.Mode 				= DMA_NORMAL;
		usart2.DMA_RX_Handler.Init.Priority 			= DMA_PRIORITY_MEDIUM;
		
		HAL_DMA_Init(&usart2.DMA_RX_Handler);
		__HAL_LINKDMA(huart, hdmarx, usart2.DMA_RX_Handler);
		
		HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 2, 1); //Tx
		HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);
		
		HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 2, 1); //Rx
		HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);		
#endif
	}
	else if(huart->Instance== USART3)
	{
		__HAL_RCC_USART3_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
		
		GPIO_InitStruct.Pin = GPIO_PIN_10;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_11;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		
		HAL_NVIC_SetPriority(USART3_IRQn, 2, 2);
		HAL_NVIC_EnableIRQ(USART3_IRQn);
		
#if USART_DMA_ENABLE
		__HAL_RCC_DMA1_CLK_ENABLE();
		
		usart3.DMA_TX_Handler.Instance         			= DMA1_Channel2;
		usart3.DMA_TX_Handler.Init.Direction   			= DMA_MEMORY_TO_PERIPH;
		usart3.DMA_TX_Handler.Init.PeriphInc   			= DMA_PINC_DISABLE;
		usart3.DMA_TX_Handler.Init.MemInc      			= DMA_MINC_ENABLE;
		usart3.DMA_TX_Handler.Init.PeriphDataAlignment	= DMA_PDATAALIGN_BYTE;
		usart3.DMA_TX_Handler.Init.MemDataAlignment     = DMA_MDATAALIGN_BYTE;
		usart3.DMA_TX_Handler.Init.Mode                 = DMA_NORMAL;
		usart3.DMA_TX_Handler.Init.Priority             = DMA_PRIORITY_LOW;
		
		HAL_DMA_Init(&usart3.DMA_TX_Handler);
		__HAL_LINKDMA(huart, hdmatx, usart3.DMA_TX_Handler);
		
		usart3.DMA_RX_Handler.Instance   				= DMA1_Channel3;
		usart3.DMA_RX_Handler.Init.Direction  			= DMA_PERIPH_TO_MEMORY;
		usart3.DMA_RX_Handler.Init.PeriphInc            = DMA_PINC_DISABLE;
		usart3.DMA_RX_Handler.Init.MemInc				= DMA_MINC_ENABLE;
		usart3.DMA_RX_Handler.Init.PeriphDataAlignment  = DMA_PDATAALIGN_BYTE;
		usart3.DMA_RX_Handler.Init.MemDataAlignment     = DMA_MDATAALIGN_BYTE;
		usart3.DMA_RX_Handler.Init.Mode 				= DMA_NORMAL;
		usart3.DMA_RX_Handler.Init.Priority 			= DMA_PRIORITY_MEDIUM;
		
		HAL_DMA_Init(&usart3.DMA_RX_Handler);
		__HAL_LINKDMA(huart, hdmarx, usart3.DMA_RX_Handler);	
		
		HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 2, 2);
		HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);
		
		HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 2, 2);
		HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
#endif		
	}
	else if(huart->Instance == UART4)
	{
		__HAL_RCC_UART4_CLK_ENABLE();
		__HAL_RCC_GPIOC_CLK_ENABLE();
		
		GPIO_InitStruct.Pin = GPIO_PIN_10;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_11;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
		
		HAL_NVIC_SetPriority(UART4_IRQn, 2, 3);
		HAL_NVIC_EnableIRQ(UART4_IRQn);
#if USART_DMA_ENABLE
		__HAL_RCC_DMA2_CLK_ENABLE();
		
		usart4.DMA_TX_Handler.Instance         			= DMA2_Channel5;
		usart4.DMA_TX_Handler.Init.Direction   			= DMA_MEMORY_TO_PERIPH;
		usart4.DMA_TX_Handler.Init.PeriphInc   			= DMA_PINC_DISABLE;
		usart4.DMA_TX_Handler.Init.MemInc      			= DMA_MINC_ENABLE;
		usart4.DMA_TX_Handler.Init.PeriphDataAlignment	= DMA_PDATAALIGN_BYTE;
		usart4.DMA_TX_Handler.Init.MemDataAlignment     = DMA_MDATAALIGN_BYTE;
		usart4.DMA_TX_Handler.Init.Mode                 = DMA_NORMAL;
		usart4.DMA_TX_Handler.Init.Priority             = DMA_PRIORITY_LOW;
		
		HAL_DMA_Init(&usart4.DMA_TX_Handler);
		__HAL_LINKDMA(huart, hdmatx, usart4.DMA_TX_Handler);
		
		usart4.DMA_RX_Handler.Instance   				= DMA2_Channel3;
		usart4.DMA_RX_Handler.Init.Direction  			= DMA_PERIPH_TO_MEMORY;
		usart4.DMA_RX_Handler.Init.PeriphInc            = DMA_PINC_DISABLE;
		usart4.DMA_RX_Handler.Init.MemInc				= DMA_MINC_ENABLE;
		usart4.DMA_RX_Handler.Init.PeriphDataAlignment  = DMA_PDATAALIGN_BYTE;
		usart4.DMA_RX_Handler.Init.MemDataAlignment     = DMA_MDATAALIGN_BYTE;
		usart4.DMA_RX_Handler.Init.Mode 				= DMA_NORMAL;
		usart4.DMA_RX_Handler.Init.Priority 			= DMA_PRIORITY_MEDIUM;
		
		HAL_DMA_Init(&usart4.DMA_RX_Handler);
		__HAL_LINKDMA(huart, hdmarx, usart4.DMA_RX_Handler);	
		
		HAL_NVIC_SetPriority(DMA2_Channel3_IRQn, 2, 3); //Rx
		HAL_NVIC_EnableIRQ(DMA2_Channel3_IRQn);	 
		
		HAL_NVIC_SetPriority(DMA2_Channel5_IRQn, 2, 3); //Tx
		HAL_NVIC_EnableIRQ(DMA2_Channel5_IRQn);	 
#endif
	}
}

