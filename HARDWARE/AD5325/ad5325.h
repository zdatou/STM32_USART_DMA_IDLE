#ifndef _AD5325_H
#define _AD5325_H
#include "sys.h"

#define AD5325_ADD 0x18
#define AD5325_CHA 0x1
#define AD5325_CHB 0x2
#define AD5325_CHC 0x4
#define AD5325_CHD 0x8


void AD5325_Init(void);
u8 AD5325_WriteByte(u8 writeAddr, u16 data);
u16 AD5325_ReadByte(u8 readAddr);

u8 AD5325_Set_By_Value(u8 ch, u16 value);
u8 AD5325_Set_By_Volatage(float vol, float ref, u8 ch);
u16 AD5325_Read_By_Value(u8 ch);
u8 AD5325_Clear(void);
u8 AD5325_Test(void);
#endif
