#include "oled.h"
#include "stdlib.h"
#include "delay.h"
//定义ASCII库
#include "Asc5x8.h" 	   //ascii 5*8字库
#include "Asc6x12.h" 	   //ascii 8*16字库
#include "Asc8x16.h" 	   //ascii 8*16字库
#include "Asc12x24.h" 	   //ascii 12*24字库
#include "Asc20x40.h" 	   //ascii 20*40字库
//自定义汉字库
#include "HZ16X16_S.h" 	   //16*16宋体自定义汉字库
#include "HZ24X24_S.h" 	   //24*24宋体自定义汉字库
#include "HZ12X12.h" 	   //12*12宋体自定义汉字库
#include "string.h"
#include "stdio.h"

__align(4) u8 OLED_GRAM[64][32];	

void OLED_MspInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;	
	
	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13, GPIO_PIN_SET);
}

void OLED_Refresh_Gram(void)
{
	unsigned char i,j;
	Set_Column_Address(Shift+0,Shift+63); // 设置列坐标，shift为列偏移量由1322决定 
	Set_Row_Address(i,i+63);
	Set_Write_RAM();
	for(i=0;i<64;i++)
	{
		for(j=0;j<32;j++)
		{
			Con_4_byte(OLED_GRAM[i][j]);
		}
	}
}

void OLED_Clear(void)  
{  
	u16 i,n;  
	for(i=0;i<32;i++)for(n=0;n<64;n++)OLED_GRAM[n][i]=0X00;  
	//OLED_Refresh_Gram();//更新显示
	//Fill_RAM(0);
}

void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>255||y>63)return;//超出范围了.
	pos=x/8; //计算位于那一列
	bx=x%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[y][pos]|=temp;
	else OLED_GRAM[y][pos]&=~temp;	    
}
//在指定位置显示一个字符
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr, u8 size, u8 mode, u8 gray)
{	
	u8 temp, t, t1, t2; 
	u8 *ptemp = NULL;
	u8 x0 = x, step = 0; 
	chr = chr - ' ';  //得到偏移后的值
	
	switch(size) //根据字体大小得到字体数组以及改字体每个字对应的字节数, t2为该字体每个字占屏幕多少列
	{
		case 8:ptemp = ASC5X8;step = 8;t2 = 8;	
			break;
		case 12:ptemp = ASC6X12;step = 12;t2 = 8; 		
			break;
		case 16:ptemp = ASC8X16;step = 16;t2 = 8;
			break;
		case 24:ptemp = ASC12X24;step = 48;t2 = 16;
			break;
		case 40:ptemp = ASC20X40;step = 120;t2 = 24;
			break;
		default:
			return;
	}
	for(t = 0; t < step; t++)
	{
		temp = ptemp[chr*step+t];
		for(t1 = 0; t1 < 8; t1++)
		{
			if(temp&0x80)
			{
				OLED_DrawPoint(x, y, mode);
			}
			else
			{
				OLED_DrawPoint(x,y,!mode);
			}
			temp <<= 1;
			x++;
			if((x-x0)==t2)
			{
				x = x0;
				y++;
				break;
			}
		}
	}
}

//获取字体在字库中的索引
u16 GetFontIndex(u8 *font, u8 fontSize)
{
	u16 index = 0;

	if(fontSize == 12)
	{
		u16 len = sizeof(HZ12X12)/sizeof(HZ12X12[0]);
		for(u16 i = 0; i < len; i++)
		{
			if((HZ12X12[i].Index[0]==font[0]) && (HZ12X12[i].Index[1] == font[1]))
			{
				index = i;
				break;
			}
		}
	}
	else if(fontSize == 16)
	{
		
	}
	else if(fontSize == 24)
	{
		
	}
	return index;
}

//显示一个指定大小的汉字
//x,y :汉字的坐标
//font:汉字GBK码
//size:字体大小
//mode:0,正常显示,1,叠加显示	   
void Show_Font(u16 x,u16 y,u8 *font,u8 size,u8 mode, u8 gray)
{
	u8 temp,t,t1;
	u16 x0=x;
	u16 index = 0;
	u8 csize=(size/8+((size%8)?1:0))*(size);			//得到字体一个字符对应点阵集所占的字节数	 
	if(size!=12&&size!=16&&size!=24&&size!=32)return;	//不支持的size
	index = GetFontIndex(font, size);
	
	for(t=0;t<csize;t++)
	{   												   
		temp = HZ12X12[index].Msk[t];;			//得到点阵数据                          
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)
			{
				OLED_DrawPoint(x, y, mode);
			}
			else
			{
				OLED_DrawPoint(x,y,!mode);
			}	
			temp <<= 1;
			x++;
			if((x-x0)==size)
			{
				x = x0;
				y++;
				break;
			}
		}  	 
	}  
}


void _OLED_ShowText(u8 x,u8 y, u8 *str,u8 size, u8 mode, u8 gray)
{
	u16 x0 = x;
	u8 bHz = 0;
	u8 temp = 0;
	
	if(size == 8)temp = 5;
	else temp = size/2;
	
	while(*str != 0)
	{
		if(!bHz)
		{
			if(*str > 0x80)bHz = 1; //中文
			else
			{
				if(x > (256-size/2))
				{
					y += size;
					x = x0;
				}
				if(y > (64-size))break;
				if(*str == '\n')
				{
					str++;
					y+=size;
					x = x0;
					continue;
				}
				OLED_ShowChar(x, y, *str, size, mode, gray);
				str++;
				x += temp;
			}
		}
		else
		{
			bHz = 0;
			if(x > (256 - size))
			{
				y += size;
				x = x0;
			}
			if(y > (64 -size))break;
			Show_Font(x, y, str, size, mode, gray);
			str += 2;
			x += size;
		}
	}
}
//在指定位置开始显示一个字符串	    
//支持自动换行
//(x,y):起始坐标
//width,height:区域
//str  :字符串
//size :字体大小
//mode:0,反白显示 1,正常显示   
//middle: 0不居中 1居中
//gray: 灰度值 暂时未用到
void OLED_ShowText(u8 x,u8 y, u8 *str,u8 size, u8 mode, u8 middle, u8 gray)
{
	u8 t2 = 0;
	u16 width = 0;
	u8 buf[24];
	u8 *ptemp;
	u8 cnt = 0;
	
	
	switch(size) //根据字体大小得到字体数组以及改字体每个字对应的字节数, t2为该字体每个字占屏幕多少列
	{
		case 8:
		case 12:
		case 16:t2 = 8;break;
		case 24:t2 = 16;break;
		case 40:t2 = 24;break;
		default:return;
	}

	if(!middle)
	{
		_OLED_ShowText(x, y, str, size, mode, gray);
		return;
	}
	
	while(1)
	{
		if(*str >= 0x80)
		{
			buf[cnt++] = *str;
			buf[cnt++] = *(str+1);
			str+=2;
		}
		else if((*str != '\n') && (*str != '\0'))
		{
			buf[cnt++] = *str;
			str++;
		}
		else
		{		
			ptemp = buf;
			buf[cnt] = '\0';
			width = 0;
			cnt = 0;
			while(*ptemp != 0)
			{
				if(*ptemp > 0x80)
				{
					width += size;
					ptemp+=2;
				}
				else
				{
					width += t2;
					ptemp++;
				}
			}
			if(width < 256)
			{
				x = (256-width)/2;
			}
			_OLED_ShowText(x, y, buf, size, mode, gray);
			y += (size + 5);
			if(*str == '\0')break;
			str++;
		}
	}
}


//x1,y1,x2,y2 填充区域的对角坐标
//确保x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,清空;1,填充	  
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  
{  
	u8 x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}													    
	OLED_Refresh_Gram();//更新显示
}


u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			

//显示1个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size, u8 gray)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size,1, gray);
				continue;
			}else enshow=1;  	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1, gray); 
	}
} 
//显示1个浮点数
//x,y :起点坐标	 
//len :小数的位数
//size:字体大小
//pos:需要反白的位 从小数位最低为数起
//num:数值;	
void OLED_ShowFloat(u8 x, u8 y, float num, u8 len, u8 size, u8 pos)
{
	u8 i = 0;
	char buf[15];
	
	sprintf(buf, "%%.%df", len);
	sprintf(buf, buf, num);
	
	for(i=0; buf[i]!='\0'; i++)//判断是不是非法字符!
	{       
		if(x>(256-(size/2))){x=0;y+=size;}
		if(y>(64-size)){y=x=0;OLED_Clear();}
		if((buf[i]<='~')&&(buf[i]>=' '))
		{
			if(pos==i)
			{
				OLED_ShowChar(x,y,buf[i],size,0, 0xff);
			}
			else		
			{
				OLED_ShowChar(x,y,buf[i],size,1, 0xff);	
			}	
		}			
		x+=size/2;
	} 
	
	
//	u8 temp=0;
//	u8 str[15] = {0};
//	u8 i = 0;
//	u32 integer = 0;
//	u32 decimal = 0;
//	
//	if(!len) return;
//	
//	integer = (u32)num;
//	decimal = (u32)((num-integer)*100000);
//	
//	for(i = 0; i < 15; i++)
//	{
//		if(decimal != 0 || (i == 0))  //增加一个i=0的条件 可以防止要显示的数据小数部分为0时显示异常
//		{
//			temp = decimal % 10 + '0';
//			str[15-i-1] = temp;
//			decimal /= 10;
//			if(decimal == 0)  //小数处理完毕后添加小数点
//			{
//				i++;
//				str[15-i-1] = '.';
//				if(integer == 0)
//				{
//					i++;
//					str[15-i-1] = '0'; //防止整数部分为0时显示异常
//					i++;
//					break;
//				}
//			}
//		}
//		else if(integer != 0)
//		{
//			temp = integer % 10 + '0';
//			str[15-i-1] = temp;
//			integer /= 10;
//		}
//		else
//		{
//			break;
//		}
//	}

//	temp = 15 - i;
//	for(i=0;i<len+1;i++)//判断是不是非法字符!
//	{       
//		if(x>(256-(size/2))){x=0;y+=size;}
//		if(y>(64-size)){y=x=0;OLED_Clear();}
//		if((str[i+temp]<='~')&&(str[i+temp]>=' '))
//		{
//			if(pos==i)
//			{
//				OLED_ShowChar(x,y,str[i+temp],size,0, 0xff);
//			}
//			else		
//			{
//				OLED_ShowChar(x,y,str[i+temp],size,1, 0xff);	
//			}	
//		}			
//		x+=size/2;
//	} 
}

//显示字符串
//x,y:起点坐标  
//size:字体大小 
//*p:字符串起始地址 
//gray:灰度值
//line: 1上划线 2下划线 0无
void OLED_ShowString(u8 x,u8 y, u8 *p,u8 size, u8 line, u8 gray)
{
	u8 temp = 0;
	if(size == 8)temp = 5;
	else temp = size/2;
		
	if(line == 1)
	{
		while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
		{       
			if(x>(256-(size/2))){x=0;y+=size;}
			if(y>(64-size)){y=x=0;OLED_Clear();}
			OLED_ShowChar(x,y,*p,size, 1, gray);	

			if((*p!=' ') || (*(p+1) != ' ' && *(p+1)!=0))
			{
				for(u8 i = 0; i < 8; i++)
				{
					OLED_DrawPoint(x+i, y, 1);
				}
			}
			x+=temp;
			p++;
		} 
	}
	else if(line == 2)
	{
//		while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
//		{       
//			if(x>(256-(size/2))){x=0;y+=size;}
//			if(y>(64-size)){y=x=0;OLED_Clear();}
//			OLED_ShowChar(x,y,*p,size, 1, gray);	

//			if((*p!=' ') || (*(p+1) != ' ' && *(p+1)!=0))
//			{
//				for(u8 i = 0; i < 8; i++)
//				{
//					OLED_DrawPoint(x+i, y, 1);
//				}
//			}
//			x+=temp;
//			p++;
//		} 
	}
	else
	{
		while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
		{       
			if(x>(256-(size/2))){x=0;y+=size;}
			if(y>(64-size)){y=x=0;OLED_Clear();}
			OLED_ShowChar(x,y,*p,size, 1, gray);			
			x+=temp;
			p++;
		} 
	}
}
//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void OLED_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		OLED_DrawPoint(uRow,uCol, 1);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
} 

//画矩形	  
//(x1,y1),(x2,y2):矩形的对角坐标
void OLED_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	OLED_DrawLine(x1,y1,x2,y1);
	OLED_DrawLine(x1,y1,x1,y2);
	OLED_DrawLine(x1,y2,x2,y2);
	OLED_DrawLine(x2,y1,x2,y2);
}

//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		OLED_DrawPoint(x0+a,y0-b, 1);             //5
 		OLED_DrawPoint(x0+b,y0-a, 1);             //0           
		OLED_DrawPoint(x0+b,y0+a, 1);             //4               
		OLED_DrawPoint(x0+a,y0+b, 1);             //6 
		OLED_DrawPoint(x0-a,y0+b, 1);             //1       
 		OLED_DrawPoint(x0-b,y0+a, 1);             
		OLED_DrawPoint(x0-a,y0-b, 1);             //2             
  		OLED_DrawPoint(x0-b,y0-a, 1);             //7     	         
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 

//**********************************************************************//
//**********************************************************************//
//**********************************************************************//
//**********************************************************************//
//**********************************************************************//
void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	u8 i;			  
	if(cmd)
	  OLED_DC_Set();
	else 
	  OLED_DC_Clr();		  
	OLED_CS_Clr();
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK_Clr();
		if(dat&0x80)
		   OLED_SDIN_Set();
		else 
		   OLED_SDIN_Clr();
		OLED_SCLK_Set();
		dat<<=1;   
	}				 		  
	OLED_CS_Set();
	OLED_DC_Set();   	  
}
  
   
//初始化SSD1322				    
void OLED_Init(void)
{ 	
	OLED_MspInit();
 
	LCD_SCL(1);
    LCD_RST(0);
    HAL_Delay(100);
    LCD_RST(1); 
		
	OLED_WR_Byte(0xAE,OLED_CMD); //	Display Off
	
	OLED_WR_Byte(0xFD,OLED_CMD); // Set Command Lock
	OLED_WR_Byte(0x12,OLED_DATA); //
	
	OLED_WR_Byte(0xB3,OLED_CMD); // Set Clock as 80 Frames/Sec
	OLED_WR_Byte(0x91,OLED_DATA); //  

  OLED_WR_Byte(0xCA,OLED_CMD); // Set Multiplex Ratio
	OLED_WR_Byte(0x3F,OLED_DATA); // 1/64 Duty (0x0F~0x5F)
   
	OLED_WR_Byte(0xA2,OLED_CMD); // Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_DATA); //	
	
	OLED_WR_Byte(0xA1,OLED_CMD); // Set Mapping RAM Display Start Line (0x00~0x7F)
	OLED_WR_Byte(0x00,OLED_DATA); //	

	
	OLED_WR_Byte(0xA0,OLED_CMD); //Set Column Address 0 Mapped to SEG0 
	OLED_WR_Byte(0x14,OLED_DATA);      	//   Default => 0x40
	
	OLED_WR_Byte(0x11,OLED_DATA);      	//    Default => 0x01 (Disable Dual COM Mode)

	
	OLED_WR_Byte(0xB5,OLED_CMD); //  Disable GPIO Pins Input
	OLED_WR_Byte(0x00,OLED_DATA); //	
	
    OLED_WR_Byte(0xAB,OLED_CMD); //   Enable Internal VDD Regulator
	OLED_WR_Byte(0x01,OLED_DATA); //

	OLED_WR_Byte(0xB4,OLED_CMD); //  Display Enhancement  
	OLED_WR_Byte(0xA0,OLED_DATA); // Enable External VSL
	OLED_WR_Byte(0xF8,OLED_DATA); // Enhance Low Gray Scale Display Quality

    OLED_WR_Byte(0xC1,OLED_CMD); //  Set Contrast Current 
	OLED_WR_Byte(0xEF,OLED_DATA); //  Default => 0x7F

    OLED_WR_Byte(0xC7,OLED_CMD); //  Master Contrast Current Control 
	OLED_WR_Byte(Brightness,OLED_DATA); //  Default => 0x0f (Maximum)

    OLED_WR_Byte(0xB8,OLED_CMD); //  	// Set Gray Scale Table 
	OLED_WR_Byte(0x0C,OLED_DATA); //
	OLED_WR_Byte(0x18,OLED_DATA); //
	OLED_WR_Byte(0x24,OLED_DATA); //
	OLED_WR_Byte(0x30,OLED_DATA); //
	OLED_WR_Byte(0x3C,OLED_DATA); //
	OLED_WR_Byte(0x48,OLED_DATA); //
	OLED_WR_Byte(0x54,OLED_DATA); //
	OLED_WR_Byte(0x60,OLED_DATA); //
	OLED_WR_Byte(0x6C,OLED_DATA); //
	OLED_WR_Byte(0x78,OLED_DATA); //
	OLED_WR_Byte(0x84,OLED_DATA); //
	OLED_WR_Byte(0x90,OLED_DATA); //
	OLED_WR_Byte(0x9C,OLED_DATA); //
	OLED_WR_Byte(0xA8,OLED_DATA); //
	OLED_WR_Byte(0xB4,OLED_DATA); //
 	OLED_WR_Byte(0x00,OLED_CMD); 	// Enable Gray Scale Table

	
	OLED_WR_Byte(0xB1,OLED_CMD); //   Set Phase 1 as 5 Clocks & Phase 2 as 14 Clocks
	OLED_WR_Byte(0xE2,OLED_DATA); //  Default => 0x74 (7 Display Clocks [Phase 2] / 9 Display Clocks [Phase 1])

    OLED_WR_Byte(0xD1,OLED_CMD); //    Enhance Driving Scheme Capability (0x00/0x20)
	OLED_WR_Byte(0xA2,OLED_DATA); //   Normal
	OLED_WR_Byte(0x20,OLED_DATA); //

   
    OLED_WR_Byte(0xBB,OLED_CMD); //     Set Pre-Charge Voltage Level as 0.60*VCC
	OLED_WR_Byte(0x1F,OLED_DATA); //

    OLED_WR_Byte(0xB6,OLED_CMD); //  // Set Second Pre-Charge Period as 8 Clocks 
	OLED_WR_Byte(0x08,OLED_DATA); //

    OLED_WR_Byte(0xBE,OLED_CMD); //  Set Common Pins Deselect Voltage Level as 0.86*VCC 
	OLED_WR_Byte(0x07,OLED_DATA); //  Default => 0x04 (0.80*VCC)

    OLED_WR_Byte(0xA6,OLED_CMD); //     Normal Display Mode (0x00/0x01/0x02/0x03)
	 								  //   Default => 0xA4
						//     0xA4  => Entire Display Off, All Pixels Turn Off
						//     0xA5  => Entire Display On, All Pixels Turn On at GS Level 15
						//     0xA6  => Normal Display
						//     0xA7  => Inverse Display
	 
	 OLED_WR_Byte(0xA9,OLED_CMD); //   Disable Partial Display
     	 	
	Fill_RAM(0x00);
	OLED_WR_Byte(0xAF,OLED_CMD); //	Display On
 

}  

void Set_Column_Address(unsigned char a, unsigned char b)
{
	OLED_WR_Byte(0x15,OLED_CMD);		// Set Column Address
	OLED_WR_Byte(a,OLED_DATA);			//   Default => 0x00
	OLED_WR_Byte(b,OLED_DATA);			//   Default => 0x77
}


void Set_Row_Address(unsigned char a, unsigned char b)
{
	OLED_WR_Byte(0x75,OLED_CMD);			// Set Row Address
	OLED_WR_Byte(a,OLED_DATA);			//   Default => 0x00
	OLED_WR_Byte(b,OLED_DATA);			//   Default => 0x7F
}

void Set_Write_RAM()
{
	OLED_WR_Byte(0x5C,OLED_CMD);			// Enable MCU to Write into RAM
}

void Set_Read_RAM()
{
	OLED_WR_Byte(0x5D,OLED_CMD);			// Enable MCU to Read from RAM
}




  
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Regular Pattern (Full Screen)
 //清屏函数,清屏后,整个屏幕是黑的
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fill_RAM(unsigned char Data)
{
unsigned char i,j;

	Set_Column_Address(0x00,0x77);
	Set_Row_Address(0x00,0x7F);
	Set_Write_RAM();

	for(i=0;i<128;i++)
	{
		for(j=0;j<120;j++)
		{
			OLED_WR_Byte(Data,OLED_DATA);
		    OLED_WR_Byte(Data,OLED_DATA); 
		}
	}
}


 






/**************************************
  数据转换程序：将2位分成1个字节存入显存，由于1个seg表示4个列所以要同时写2个字节即4个像素
  uchar DATA：取模来的字模数据
****************************************/
inline void Con_4_byte(unsigned char DATA)
{
	unsigned char d1_4byte[4],d2_4byte[4];
	unsigned char i;
	unsigned char d,k1,k2;
	d=DATA;
 
	for(i=0;i<2;i++)   // 一两位的方式写入  2*4=8位
	{
		k1=d&0xc0;     //当i=0时 为D7,D6位 当i=1时 为D5,D4位

     /****有4种可能，16级灰度,一个字节数据表示两个像素，一个像素对应一个字节的4位***/
		switch(k1)
		{
			case 0x00:d1_4byte[i]=0x00;// 0000,0000   
				break;
			case 0x40:d1_4byte[i]=0x0f;// 0100,0000	   
				break;	
			case 0x80:d1_4byte[i]=0xf0;//1000,0000	   
				break;
			case 0xc0:d1_4byte[i]=0xff;//1100,0000	  
				break;	 
			default:
				break;
		} 
		d=d<<2;
		k2=d&0xc0;     //当i=0时 为D7,D6位 当i=1时 为D5,D4位

     /****有4种可能，16级灰度,一个字节数据表示两个像素，一个像素对应一个字节的4位***/
		switch(k2)
		{
			case 0x00:d2_4byte[i]=0x00;//0000,0000
				break;
			case 0x40:d2_4byte[i]=0x0f;//0100,0000
				break;	
			case 0x80:d2_4byte[i]=0xf0;//1000,0000
				break;
			case 0xc0:d2_4byte[i]=0xff;//1100,0000
				break;	 
			default:
				break;
		}	  
		d=d<<2;                                //左移两位    
		OLED_WR_Byte(d1_4byte[i],OLED_DATA);	    //写前2列
		OLED_WR_Byte(d2_4byte[i],OLED_DATA);       	//写后2列	  共计4列
   }
}
 








