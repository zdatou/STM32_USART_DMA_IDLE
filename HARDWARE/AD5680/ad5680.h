#ifndef _AD5680_H
#define _AD5680_H
#include "sys.h"

#define AD5680_CS PDout(1)
#define AD5680_SCK PDout(2)
#define AD5680_Data PDout(3)

void AD5680_Init(void);
void AD5680_Write3Byte(u32 data);
void AD5680_SetOutput_By_Value(u32 value);
void AD5680_SetOutput_By_Volatage(float vol, float ref);
void AD5680_Delay(void);
#endif
