#include "ad5680.h"

void AD5680_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	
	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_Initure.Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
	GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_Initure.Pull = GPIO_PULLUP;
	GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOD, &GPIO_Initure);
}

void AD5680_Delay(void)
{
	u8 i = 0;
	for(i = 0; i < 5; i++)
	{
		;
	}
}
	

void AD5680_Write3Byte(u32 data)
{
	u8 i = 0;
	
	AD5680_SCK = 1;
	AD5680_CS = 0;  //开启数据传输
	
	for(i = 0; i < 24; i++)
	{
		if(data&0x800000)
		{
			AD5680_Data = 1;
		}
		else
		{
			AD5680_Data = 0;
		}
		AD5680_Delay();
		AD5680_SCK = 0;
		AD5680_Delay();
		AD5680_SCK = 1;
		data <<= 1;
	}
	AD5680_CS = 1;
}

void AD5680_SetOutput_By_Value(u32 value)
{
	value = (value << 2) & 0xFFFFFF;
	AD5680_Write3Byte(value);
}

void AD5680_SetOutput_By_Volatage(float vol, float ref)
{
	u32 res = 0;
	if(vol > ref) res = 0xFFFFC;
	else
	{
		res = (vol / ref)*0x3FFFF; //2^18 - 1
	}
	AD5680_Write3Byte(res << 2);
}

