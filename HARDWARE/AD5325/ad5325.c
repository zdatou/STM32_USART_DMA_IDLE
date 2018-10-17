#include "ad5325.h"
#include "myiic.h"

void AD5325_Init(void)
{
	IIC_Init();
}

u8 AD5325_Write2Byte(u8 writeAddr, u16 data)
{ 
    IIC_Start();  
	IIC_Send_Byte(AD5325_ADD|0);   	 
	if(IIC_Wait_Ack()) return 1;	   
	
    IIC_Send_Byte(writeAddr);
	if(IIC_Wait_Ack()) return 1;
	
	IIC_Send_Byte((data>>8)&0xff);     
	if(IIC_Wait_Ack()) return 1; 
	IIC_Send_Byte(data&0xff);     
	if(IIC_Wait_Ack()) return 1;
	
    IIC_Stop();
	return 0;
}

u16 AD5325_ReadByte(u8 readAddr)
{
	u16 temp=0;		  	    																 
    IIC_Start();  
	
	IIC_Send_Byte(AD5325_ADD|0);  	   
	if(IIC_Wait_Ack()) return 1; 
	
    IIC_Send_Byte(readAddr);   
	if(IIC_Wait_Ack()) return 1;
	
	IIC_Start();  
	IIC_Send_Byte(AD5325_ADD|1);          		   
	if(IIC_Wait_Ack()) return 1;	 
    temp=IIC_Read_Byte(1);		
	temp = (temp<<8) + IIC_Read_Byte(0);
    IIC_Stop();	    
	return temp;
}

u8 AD5325_Set_By_Value(u8 ch, u16 value)
{
	value = value & 0xFFF;
	value |= 0x2000;
	
	if(AD5325_Write2Byte(ch, value)) return 1;
	return 0;
}

u8 AD5325_Set_By_Volatage(float vol, float ref, u8 ch)
{
	return 0;
}

u16 AD5325_Read_By_Value(u8 ch)
{
	u16 temp = 0;
	temp = AD5325_ReadByte(ch);
	return temp;
}

u8 AD5325_Clear(void)
{
	IIC_Start();  
	IIC_Send_Byte(AD5325_ADD|0);   
	if(IIC_Wait_Ack()) return 1;
	
    IIC_Send_Byte(0x1);  
	if(IIC_Wait_Ack()) return 1;
	
	IIC_Send_Byte(0x00);     
	if(IIC_Wait_Ack()) return 1;
	IIC_Send_Byte(0x00);    
	if(IIC_Wait_Ack()) return 1;
    IIC_Stop();
	
	return 0;
}

u8 AD5325_Test(void)
{
	if(AD5325_Write2Byte(0x1, 0x3FFF)) return 1;
	if(AD5325_Write2Byte(0x2, 0x3155)) return 1;
	if(AD5325_Write2Byte(0x4, 0x3055)) return 1;
	if(AD5325_Write2Byte(0x8, 0x3055)) return 1;
	AD5325_Write2Byte(0, 0x2000);
	return 0;
}
