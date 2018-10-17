#ifndef _SPI_H
#define _SPI_H
#include "sys.h"
extern SPI_HandleTypeDef SPI2_Handler;  //SPI¾ä±ú
extern SPI_HandleTypeDef SPI1_Handler;  //SPI¾ä±ú

void SPI2_Init(void);
void SPI1_Init(void);
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler);
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler);
u8 SPI2_ReadWriteByte(u8 TxData);
u8 SPI1_ReadWriteByte(u8 TxData);
#endif
