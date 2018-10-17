#include "tdc_gpx2.h"
#include "stdbool.h"
#include "delay.h"
#include "spi.h"
// ---------------------------------------------------------------------------------------------
// *** Configuration Registers ***
// ---------------------------------------------------------------------------------------------
const u16 config_register1[17] = { 0x00Cf, 0x008f, 0x009D, 0x00a0, 0x0086, 0x0001, 0x00C0, 0x00D3,   //ÄÚ²Î¿¼
0x00A1, 0x0013, 0x0000, 0x000A, 0x00CC, 0x00CC, 0x00F1, 0x007D, 0x0004};

const u16 config_register[17] = { 0x00Df, 0x008f, 0x009D, 0x00a0, 0x0086, 0x0001, 0x00C0, 0x0053,   //Íâ²Î¿¼
0x00A1, 0x0013, 0x0000, 0x000A, 0x00CC, 0x00CC, 0x00F1, 0x007D, 0x0004};

// A typical config settings = { config00, config01, … , config16 }

// ---------------------------------------------------------------------------------------------
// *** SPI Opcodes ***
// ---------------------------------------------------------------------------------------------
const u8 spiopc_power = 0x30; // opcode for "Power on Reset"
const u8 spiopc_init = 0x18; // opcode for "Initialize Chip and Start Measurement"
const u8 spiopc_write_config = 0x80; // opcode for "Write Configuration"
const u8 spiopc_read_config = 0x40; // opcode for "Read Configuration"
const u8 spiopc_read_results = 0x60; // opcode for "Read Measurement Results"
// ---------------------------------------------------------------------------------------------
// *** SPI Addresses ***
// ---------------------------------------------------------------------------------------------
const u8 reference_index_ch1_byte3 = 8; //
const u8 reference_index_ch1_byte2 = 9;
const u8 reference_index_ch1_byte1 = 10;

const u8 reference_index_ch2_byte3 = 14; //
const u8 reference_index_ch2_byte2 = 15;
const u8 reference_index_ch2_byte1 = 16;

const u8 reference_index_ch3_byte3 = 20; //
const u8 reference_index_ch3_byte2 = 21;
const u8 reference_index_ch3_byte1 = 22;

const u8 reference_index_ch4_byte3 = 26; //
const u8 reference_index_ch4_byte2 = 27;
const u8 reference_index_ch4_byte1 = 28;

const u8 stopresult_ch1_byte3 = 11;
const u8 stopresult_ch1_byte2 = 12;
const u8 stopresult_ch1_byte1 = 13;

const u8 stopresult_ch2_byte3 = 17;
const u8 stopresult_ch2_byte2 = 18;
const u8 stopresult_ch2_byte1 = 19;

const u8 stopresult_ch3_byte3 = 23;
const u8 stopresult_ch3_byte2 = 24;
const u8 stopresult_ch3_byte1 = 25;
// . . . .
const u8 stopresult_ch4_byte3 = 29;
const u8 stopresult_ch4_byte2 = 30;
const u8 stopresult_ch4_byte1 = 31;
// ---------------------------------------------------------------------------------------------
// *** Other Variables ***
// --------------------------------------------------------------------------------------------
u16 Buffer=0;
char i = 0; // counter for for-loops
long long reference_index[8][4] = {0}; // reference index data array {Ch1, Ch2, Ch3, Ch4}
long long stopresult[8][4] = {0}; // stop result data array {Ch1, Ch2, Ch3, Ch4}



bool config1_error = false; // flag that indicates if the config registers
bool config2_error = false; // flag that indicates if the config registers
// are not written correctly
u16 read_reg[17]={0};
u8  interrupt=0;

void TDC_GPX2_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
		
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	SPI1_Init();
	
	GPIO_Initure.Pin = GPIO_PIN_0;
	GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_Initure.Pull = GPIO_PULLUP;
	GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_Initure);
	
	GPIO_Initure.Pin = GPIO_PIN_4;
	HAL_GPIO_Init(GPIOE, &GPIO_Initure);
	
	GPIO_Initure.Pin = GPIO_PIN_5|GPIO_PIN_4;
	HAL_GPIO_Init(GPIOC, &GPIO_Initure);
}


void ReadTdc2Regs(u8 clksource)
{
// -----------------------------------------------------------------------------------------
// *** Power on reset ***
// -----------------------------------------------------------------------------------------
//	
	SPI1_TDC2_DISABLE;
	delay_ns(1);	
	SPI1_TDC2_ENABLE; 
	delay_ns(1);
	Buffer = spiopc_power;
	SPI1_ReadWriteByte(spiopc_power); // Opcode for "Power On Reset" is sent over SPI
	SPI1_TDC2_DISABLE;
	delay_ns(1);	
	SPI1_TDC2_ENABLE; 
	delay_ns(1);
	config2_error = false;
	Buffer = spiopc_write_config + 00;
	SPI1_ReadWriteByte(Buffer); // Opcode for "Write Configuration"
	// and config address (00) are sent over SPI

	for(i=0;i<17;i++) // Send all 17 config registers via SPI
	{
		if(clksource==1)
		{
			Buffer = config_register1[i];
		}
		else 
		{
			Buffer = config_register[i];
		}
		SPI1_ReadWriteByte(Buffer);
	}


	// -----------------------------------------------------------------------------------------
	// *** Verification of config registers ***
	// -----------------------------------------------------------------------------------------	
	SPI1_TDC2_DISABLE;
	delay_ns(1);	
	SPI1_TDC2_ENABLE; 
	delay_ns(1);
	Buffer = spiopc_read_config + 00;
	SPI1_ReadWriteByte(Buffer); // read one byte from SPI to Buffer variable	
	
	for(i=0; i<17; i++)
	{
		read_reg[i] = SPI1_ReadWriteByte(0xFF); // read one byte from SPI to Buffer variable
		if(clksource==1)
		{
			if (config_register1[i] != read_reg[i]) config2_error = true;
		}
		else
		{
			if (config_register[i] != read_reg[i]) config2_error = true;
		}
	} 
	SPI1_TDC2_DISABLE;
}

void ReadTdc1Regs(u8 clksource)
{
// -----------------------------------------------------------------------------------------
// *** Power on reset ***
// -----------------------------------------------------------------------------------------
	
	SPI1_TDC1_DISABLE;
	delay_ns(1);	
	SPI1_TDC1_ENABLE; 
	delay_ns(1);
	
	Buffer = spiopc_power;
	SPI1_ReadWriteByte(spiopc_power); // Opcode for "Power On Reset" is sent over SPI
	
	SPI1_TDC1_DISABLE;
	delay_ns(1);	
	SPI1_TDC1_ENABLE; 
	delay_ns(1);
	config1_error = false;
	Buffer = spiopc_write_config + 00;
	SPI1_ReadWriteByte(Buffer); // Opcode for "Write Configuration"

	for(i=0;i<17;i++) // Send all 17 config registers via SPI
	{
		if(clksource==1)
		{
			Buffer = config_register1[i];
		}
		else Buffer = config_register[i];
		SPI1_ReadWriteByte(Buffer);
	}
	
	SPI1_TDC1_DISABLE;
	delay_ns(1);	
	SPI1_TDC1_ENABLE; 
	delay_ns(1);
	Buffer = spiopc_read_config + 00;
	SPI1_ReadWriteByte(Buffer); // read one byte from SPI to Buffer variable

	for(i=0; i<17; i++)
	{
		read_reg[i] = SPI1_ReadWriteByte(0xFF); // read one byte from SPI to Buffer variable
		if(clksource==1)
		{
			if (config_register1[i] != read_reg[i]) config1_error = true;
		}
		else
		{
			if (config_register[i] != read_reg[i]) config1_error = true;
		}
	} 
	SPI1_TDC1_DISABLE;
}

void startTDC(void)
{
	if((config1_error==false)&&(config1_error==false))
	{
		TDC_DISABLE;		

		SPI1_TDC1_DISABLE;
		delay_ns(10);	
		SPI1_TDC1_ENABLE; 
		delay_ns(10);
		Buffer = spiopc_init;
		SPI1_ReadWriteByte(Buffer); // Opcode for "Initialize" is sent over SPI	
		SPI1_TDC1_DISABLE;

		SPI1_TDC2_DISABLE;
		delay_ns(10);	
		SPI1_TDC2_ENABLE; 
		delay_ns(10);
		Buffer = spiopc_init;
		SPI1_ReadWriteByte(Buffer); // Opcode for "Initialize" is sent over SPI		
		SPI1_TDC2_DISABLE; 	

		TDC_RSTENABLE;
		delay_ns(10);		
		TDC_RSTDISABLE;
		
		TDC_ENABLE;					
	}
}

extern u32 counter_halfs;
extern u8 oscclkerror;
extern u8 refclkerror;
extern u8 clksource;
extern u8 times;
extern u8 extref_flag;
void readTDC(void)
{
	startTDC();
	counter_halfs = 0;
	
	while((TDC_INTERRUPT1 != 0)&&(TDC_INTERRUPT2 != 0))
	{
		if(counter_halfs>10)
		{
			if(clksource==1)oscclkerror=1; // wait till the Interrupt pin is low		
			else refclkerror = 1;
			break;
		}
	}
	delay_ms(1000);	
	TDC_DISABLE;
	if(counter_halfs < 11) 
	{
		if(clksource==1)oscclkerror=0; // wait till the Interrupt pin is low		
		else refclkerror = 0;						
	}

	// Opcode for "Read Result"
	// Opcode for "Read Result" and data address are sent
	if(TDC_INTERRUPT1 == 0)
	{
		for(times=0;times<4;times++)
		{	
			extref_flag = times;
			SPI1_TDC1_DISABLE;
			delay_ns(1);	
			SPI1_TDC1_ENABLE; 
			delay_ns(10);						
			Buffer = spiopc_read_results + reference_index_ch1_byte3;
			SPI1_ReadWriteByte(Buffer);		

			for( i = 0; i < 4; i++)
			{
				if(times>3)
				{
					reference_index[i][3] = 0;
					stopresult[i][3] = 0;
					Buffer = SPI1_ReadWriteByte(0xFF); // read one byte from SPI to Buffer
					reference_index[i][3] = reference_index[i][3]+ ( Buffer << 16 ); // and added to the reference_index // Data is shifted 16 Bits to the left
					
					Buffer = SPI1_ReadWriteByte(0xFF); // read one byte from SPI to Buffer
					reference_index[i][3] = reference_index[i][3]+ ( Buffer << 8 ); // and added to the reference_index // Data is shifted 8 Bits to the left
					
					Buffer = SPI1_ReadWriteByte(0xFF); // read one byte from SPI to Buffer
					reference_index[i][3] = reference_index[i][3]+ Buffer; // Data is directly added to reference_index
					
					// The complete reference index (3 Bytes)
					// has been received.
					Buffer = SPI1_ReadWriteByte(0xFF); // read one byte from SPI to Buffer
					stopresult[i][3] = stopresult[i][3]+ ( Buffer << 16 ); // is repeated for stop results
					
					Buffer = SPI1_ReadWriteByte(0xFF); // read one byte from SPI to Buffer
					stopresult[i][3] = stopresult[i][3] + ( Buffer << 8 );
					Buffer = SPI1_ReadWriteByte(0xFF); // read one byte from SPI to Buffer
					stopresult[i][3] = stopresult[i][3] + Buffer;
				}
				else
				{
					reference_index[i][times] = 0;
					stopresult[i][times] = 0;
					Buffer = SPI1_ReadWriteByte(0xFF); // read one byte from SPI to Buffer
					reference_index[i][times] = reference_index[i][times]+ ( Buffer << 16 ); // and added to the reference_index // Data is shifted 16 Bits to the left
					
					Buffer = SPI1_ReadWriteByte(0xFF); // read one byte from SPI to Buffer
					reference_index[i][times] = reference_index[i][times]+ ( Buffer << 8 ); // and added to the reference_index // Data is shifted 8 Bits to the left
					
					Buffer = SPI1_ReadWriteByte(0xFF); // read one byte from SPI to Buffer
					reference_index[i][times] = reference_index[i][times]+ Buffer; // Data is directly added to reference_index
					
					// The complete reference index (3 Bytes)
					// has been received.
					Buffer = SPI1_ReadWriteByte(0xFF); // read one byte from SPI to Buffer
					stopresult[i][times] = stopresult[i][times]+ ( Buffer << 16 ); // is repeated for stop results
					
					Buffer = SPI1_ReadWriteByte(0xFF); // read one byte from SPI to Buffer
					stopresult[i][times] = stopresult[i][times] + ( Buffer << 8 );
					Buffer = SPI1_ReadWriteByte(0xFF); // read one byte from SPI to Buffer
					stopresult[i][times] = stopresult[i][times] + Buffer;										
				}
			}
		}
	}
	SPI1_TDC1_DISABLE;	

	// Opcode for "Read Result"
	// Opcode for "Read Result" and data address are sent
	if(TDC_INTERRUPT2 == 0)
	{
		for(times=0;times<4;times++)
		{
			
			SPI1_TDC2_DISABLE;
			delay_ns(1);	
			SPI1_TDC2_ENABLE; 
			delay_ns(10);
			Buffer = spiopc_read_results + reference_index_ch1_byte3;
			SPI1_ReadWriteByte(Buffer);	

			for( i = 0; i < 4; i++)
			{
				if(times>3)	
				{
					reference_index[i+4][3] = 0;
					stopresult[i+4][3] = 0;
					Buffer = SPI1_ReadWriteByte(0xFF); // read one byte from SPI to Buffer
					reference_index[i+4][3] = reference_index[i+4][3]+ ( Buffer << 16 ); // and added to the reference_index // Data is shifted 16 Bits to the left
					
					Buffer = SPI1_ReadWriteByte(0xFF); // read one byte from SPI to Buffer
					reference_index[i+4][3] = reference_index[i+4][3]+ ( Buffer << 8 ); // and added to the reference_index // Data is shifted 8 Bits to the left
					
					Buffer = SPI1_ReadWriteByte(0xFF); // read one byte from SPI to Buffer
					reference_index[i+4][3] = reference_index[i+4][3]+ Buffer; // Data is directly added to reference_index
					
					// The complete reference index (3 Bytes)
					// has been received.
					Buffer = SPI1_ReadWriteByte(0xFF); // read one byte from SPI to Buffer
					stopresult[i+4][3] = stopresult[i+4][3]+ ( Buffer << 16 ); // is repeated for stop results
					
					Buffer = SPI1_ReadWriteByte(0xFF); // read one byte from SPI to Buffer
					stopresult[i+4][3] = stopresult[i+4][3] + ( Buffer << 8 );
					Buffer = SPI1_ReadWriteByte(0xFF); // read one byte from SPI to Buffer
					stopresult[i+4][3] = stopresult[i+4][3] + Buffer;
				}
				else
				{
					reference_index[i+4][times] = 0;
					stopresult[i+4][times] = 0;
					Buffer = SPI1_ReadWriteByte(0xFF); // read one byte from SPI to Buffer
					reference_index[i+4][times] = reference_index[i+4][times]+ ( Buffer << 16 ); // and added to the reference_index // Data is shifted 16 Bits to the left
					
					Buffer = SPI1_ReadWriteByte(0xFF); // read one byte from SPI to Buffer
					reference_index[i+4][times] = reference_index[i+4][times]+ ( Buffer << 8 ); // and added to the reference_index // Data is shifted 8 Bits to the left
					
					Buffer = SPI1_ReadWriteByte(0xFF); // read one byte from SPI to Buffer
					reference_index[i+4][times] = reference_index[i+4][times]+ Buffer; // Data is directly added to reference_index
					
					// The complete reference index (3 Bytes)
					// has been received.
					Buffer = SPI1_ReadWriteByte(0xFF); // read one byte from SPI to Buffer
					stopresult[i+4][times] = stopresult[i+4][times]+ ( Buffer << 16 ); // is repeated for stop results
					
					Buffer = SPI1_ReadWriteByte(0xFF); // read one byte from SPI to Buffer
					stopresult[i+4][times] = stopresult[i+4][times] + ( Buffer << 8 );
					Buffer = SPI1_ReadWriteByte(0xFF); // read one byte from SPI to Buffer
					stopresult[i+4][times] = stopresult[i+4][times] + Buffer;										
				}
			}
		}
	}
	SPI1_TDC2_DISABLE;	
}
