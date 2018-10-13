#include "oled.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "delay.h"
typedef struct typFNT_GB12                 // ������ģ���ݽṹ
{
       char Index[2];               // ������������
       char Msk[24];                       // ����������
}HZ12;

extern unsigned char   ASC5X8[];
extern unsigned char ASC6X12[];
extern unsigned char  ASC8X16[];
extern unsigned char ASC12X24[];
extern unsigned char   ASC20X40[];
extern HZ12 HZ12X12[];
extern unsigned char HZ16X16_S[];
extern unsigned char HZ24X24_S[];


 void Set_Remap_Format(unsigned char d)
{
	OLED_WR_Byte(0xA0,OLED_CMD);  			// Set Re-Map / Dual COM Line Mode
	OLED_WR_Byte(d,OLED_DATA);  				//   Default => 0x40
						//     Horizontal Address Increment
						//     Column Address 0 Mapped to SEG0
						//     Disable Nibble Remap
						//     Scan from COM0 to COM[N-1]
						//     Disable COM Split Odd Even
    OLED_WR_Byte(0x11,OLED_DATA); 	 			//   Default => 0x01 (Disable Dual COM Mode)
} 
  void Set_Partial_Display(unsigned char a, unsigned char b, unsigned char c)
{
  	OLED_WR_Byte((0xA8|a),OLED_CMD);		// Default => 0x8F
					
	if(a == 0x00)
	{
	    OLED_WR_Byte(b,OLED_DATA); 
		OLED_WR_Byte(c,OLED_DATA); 
	 
	}
}

void Set_Display_Offset(unsigned char d)
{
	OLED_WR_Byte(0xA2,OLED_CMD);
 	OLED_WR_Byte(d,OLED_DATA);
 
}

void Set_Start_Line(unsigned char d)
{
	OLED_WR_Byte(0xA1,OLED_CMD);	// Set Vertical Scroll by RAM
 	OLED_WR_Byte(d,OLED_DATA);
 
}

void Set_Master_Current(unsigned char d)
{
   OLED_WR_Byte(0xC7,OLED_CMD);	//  Master Contrast Current Control
 	OLED_WR_Byte(d,OLED_DATA);

}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Gray Scale Table Setting (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Set_Gray_Scale_Table()
{

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

}
void Set_Linear_Gray_Scale_Table()
{
   	OLED_WR_Byte(0xB9,OLED_CMD); //	Set Default Linear Gray Scale Table
 
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Vertical Scrolling (Full Screen)
//
//    a: Scrolling Direction
//       "0x00" (Upward)		���Ϲ���
//       "0x01" (Downward)		���¹���
//    b: Set Numbers of Row Scroll per Step	  ÿ֡����
//    c: Set Time Interval between Each Scroll Step	ÿ֡����ʱ
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Vertical_Scroll(unsigned char a, unsigned char b, unsigned char c)
{
unsigned char i,j;	

	Set_Partial_Display(0x00,0x00,Max_Row);

	switch(a)
	{
		case 0:
			for(i=0;i<(Max_Row+1);i+=b)
			{
				Set_Display_Offset(i+1);
				for(j=0;j<c;j++)
				{
					delay_us(200);
				}
			}
			break;
		case 1:
			for(i=0;i<(Max_Row+1);i+=b)
			{
				Set_Display_Offset(Max_Row-i);
				for(j=0;j<c;j++)
				{
					delay_us(200);
				}
			}
			break;
	}
	Set_Partial_Display(0x01,0x00,0x00);
}

 //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  www.lcdsoc.com 201505
//	  Data:ȡֵΪ 0x11��0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff, ff����
//    a: Line Width		�߿�
//    b: Column Address of Start ��ʼ��
//    c: Column Address of End	 ������
//    d: Row Address of Start	 ��ʼ��
//    e: Row Address of End		 ������
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Draw_Line(unsigned char Data, unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e)
{
unsigned char i,j,k,l,b1,c1;
	b1=b/4;
	c1=c/4 ;
	k=a%4;
	if(k == 0)
	{
		l=(a/4)-1;
	}
	else
	{
		l=a/4;
	}

	Set_Column_Address(Shift+b1,Shift+c1);
	Set_Row_Address(d,(d+a-1));
	Set_Write_RAM();
	for(i=0;i<(c1-b1+1);i++)
	{
		for(j=0;j<a;j++)
		{
			OLED_WR_Byte(Data,OLED_DATA);
			OLED_WR_Byte(Data,OLED_DATA); 
		}
	}

	Set_Column_Address(Shift+(c1-l),Shift+c1);
	Set_Row_Address(d+a,e-a);
	Set_Write_RAM();
	for(i=0;i<(e-d);i++)//ɾ��һ��
	{
		for(j=0;j<(l+1);j++)
		{
			if(j == 0)
			{
				switch(k)
				{
					case 0:
						OLED_WR_Byte(Data,OLED_DATA);
						OLED_WR_Byte(Data,OLED_DATA);
						break;
					case 1:
						OLED_WR_Byte(0x00,OLED_DATA);
						OLED_WR_Byte((Data&0x0F),OLED_DATA);
						 
						break;
					case 2:
					    OLED_WR_Byte(0x00,OLED_DATA);
						OLED_WR_Byte(Data,OLED_DATA);
					
						break;
					case 3:
						OLED_WR_Byte((Data&0x0F),OLED_DATA);
						OLED_WR_Byte(Data,OLED_DATA);
						 
						break;
				}
			}
			else
			{
						OLED_WR_Byte(Data,OLED_DATA);
		             	OLED_WR_Byte(Data,OLED_DATA); 
			}
		}
	}
   

	Set_Column_Address(Shift+b1,Shift+c1);
	Set_Row_Address((e-a+1),e);
	Set_Write_RAM();
	for(i=0;i<(c1-b1+1);i++)
	{
		for(j=0;j<a;j++)
		{
			OLED_WR_Byte(Data,OLED_DATA);
			OLED_WR_Byte(Data,OLED_DATA); 
		 
		}
	}
 
	Set_Column_Address(Shift+b1,Shift+(b1+l));
	Set_Row_Address(d+a,e-a);
	Set_Write_RAM();
	for(i=0;i<(e-d);i++)//ɾ��һ��
	{
		for(j=0;j<(l+1);j++)
		{
			if(j == l)
			{
				switch(k)
				{
					case 0:
							OLED_WR_Byte(Data,OLED_DATA);
		                 	OLED_WR_Byte(Data,OLED_DATA); 
						break;
					case 1:
						
						OLED_WR_Byte((Data&0xF0),OLED_DATA);
						OLED_WR_Byte(0x00,OLED_DATA);
						 
						break;
					case 2:
						OLED_WR_Byte(Data,OLED_DATA);
						 OLED_WR_Byte(0x00,OLED_DATA);
					 
						break;
					case 3:
						
						OLED_WR_Byte(Data,OLED_DATA);
						OLED_WR_Byte((Data&0xF0),OLED_DATA);
				 
						break;
				}
			}
			else
			{
					OLED_WR_Byte(Data,OLED_DATA);
		         	OLED_WR_Byte(Data,OLED_DATA); 
			}
		}
	}
}

 //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  www.lcdsoc.com 201505
//	  Data:ȡֵΪ 0x11��0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff, ff����
//    a: Line Width		�߿�
//    b: Column Address of Start ��ʼ��
//    c: Column Address of End	 ������
//    d: Row Address of Start	 ��ʼ��
//    e: Row Address of End		 ������
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Draw_Rectangle(unsigned char Data, unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e)
{
unsigned char i,j,k,l,b1,c1;
	b1=b/4;
	c1=c/4 ;
	k=a%4;
	if(k == 0)
	{
		l=(a/4)-1;
	}
	else
	{
		l=a/4;
	}

	Set_Column_Address(Shift+b1,Shift+c1);
	Set_Row_Address(d,(d+a-1));
	Set_Write_RAM();
	for(i=0;i<(c1-b1+1);i++)
	{
		for(j=0;j<a;j++)
		{
			OLED_WR_Byte(Data,OLED_DATA);
			OLED_WR_Byte(Data,OLED_DATA); 
		}
	}

	Set_Column_Address(Shift+(c1-l),Shift+c1);
	Set_Row_Address(d+a,e-a);
	Set_Write_RAM();
	for(i=0;i<(e-d+1);i++)
	{
		for(j=0;j<(l+1);j++)
		{
			if(j == 0)
			{
				switch(k)
				{
					case 0:
						OLED_WR_Byte(Data,OLED_DATA);
						OLED_WR_Byte(Data,OLED_DATA);
						break;
					case 1:
						OLED_WR_Byte(0x00,OLED_DATA);
						OLED_WR_Byte((Data&0x0F),OLED_DATA);
						 
						break;
					case 2:
					    OLED_WR_Byte(0x00,OLED_DATA);
						OLED_WR_Byte(Data,OLED_DATA);
					
						break;
					case 3:
						OLED_WR_Byte((Data&0x0F),OLED_DATA);
						OLED_WR_Byte(Data,OLED_DATA);
						 
						break;
				}
			}
			else
			{
						OLED_WR_Byte(Data,OLED_DATA);
		             	OLED_WR_Byte(Data,OLED_DATA); 
			}
		}
	}
   

	Set_Column_Address(Shift+b1,Shift+c1);
	Set_Row_Address((e-a+1),e);
	Set_Write_RAM();
	for(i=0;i<(c1-b1+1);i++)
	{
		for(j=0;j<a;j++)
		{
			OLED_WR_Byte(Data,OLED_DATA);
			OLED_WR_Byte(Data,OLED_DATA); 
		 
		}
	}
 
	Set_Column_Address(Shift+b1,Shift+(b1+l));
	Set_Row_Address(d+a,e-a);
	Set_Write_RAM();
	for(i=0;i<(e-d+1);i++)
	{
		for(j=0;j<(l+1);j++)
		{
			if(j == l)
			{
				switch(k)
				{
					case 0:
							OLED_WR_Byte(Data,OLED_DATA);
		                 	OLED_WR_Byte(Data,OLED_DATA); 
						break;
					case 1:
						
						OLED_WR_Byte((Data&0xF0),OLED_DATA);
						OLED_WR_Byte(0x00,OLED_DATA);
						 
						break;
					case 2:
						OLED_WR_Byte(Data,OLED_DATA);
						 OLED_WR_Byte(0x00,OLED_DATA);
					 
						break;
					case 3:
						
						OLED_WR_Byte(Data,OLED_DATA);
						OLED_WR_Byte((Data&0xF0),OLED_DATA);
				 
						break;
				}
			}
			else
			{
					OLED_WR_Byte(Data,OLED_DATA);
		         	OLED_WR_Byte(Data,OLED_DATA); 
			}
		}
	}
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Regular Pattern (Partial or Full Screen)
//
//    a: Column Address of Start
//    b: Column Address of End (Total Columns Devided by 4)
//    c: Row Address of Start
//    d: Row Address of End
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fill_Block(unsigned char Data, unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
unsigned char i,j;
	
	Set_Column_Address(Shift+a,Shift+b);
	Set_Row_Address(c,d);
	Set_Write_RAM();

	for(i=0;i<(d-c+1);i++)
	{
		for(j=0;j<(b-a+1);j++)
		{
			OLED_WR_Byte(Data,OLED_DATA);
		    OLED_WR_Byte(Data,OLED_DATA); 
		}
	}
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Pattern (Partial or Full Screen)
//
//    a: Column Address of Start
//    b: Column Address of End (Total Columns Devided by 4)
//    c: Row Address of Start
//    d: Row Address of End
// �Ҷ�ģʽ����ʾһ��ͼƬ
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Show_Pattern(unsigned char *Data_Pointer, unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{			 
	unsigned char *Src_Pointer;
	unsigned char i,j;
 
  //ȡģʱ����������	�����ܷ�����2.7��ͬ��
    Src_Pointer=Data_Pointer;
	Set_Column_Address(Shift+a,Shift+b);
	Set_Row_Address(c,d);
	Set_Write_RAM();
  	for(i=0;i<(d-c+1);i++)
	{
		for(j=0;j<(b-a+1);j++)
		{
			OLED_WR_Byte(*Src_Pointer,OLED_DATA);
			Src_Pointer++;
			OLED_WR_Byte(*Src_Pointer,OLED_DATA);
			Src_Pointer++;
		}
	}
 }

void HZ12_12( unsigned char x, unsigned char y, unsigned char num)
{
	unsigned char x1,j ;
	x1=x/4; 
	Set_Column_Address(Shift+x1,Shift+x1+3); // ���������꣬shiftΪ��ƫ������1322������3Ϊ16/4-1
	Set_Row_Address(y,y+11); 
	Set_Write_RAM();	 //	д�Դ�
	 
	for(j=0;j<24;j++)
	{
		Con_4_byte(HZ12X12[num].Msk[j]);
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char MY_Compare(char str1[2],char str2[2])
{
	unsigned char i;
	for(i=0;i<2;i++)
	{
		if(str1[i]!=str2[i])
			break;
		else if(i==1)
			return 1;
	}
	return 0;
}
void MYHZ12_12( unsigned char x, unsigned char y, char num[2])
{
	unsigned char x1,j,i=0 ;
	x1=x/4; 
	Set_Column_Address(Shift+x1,Shift+x1+3); // ���������꣬shiftΪ��ƫ������1322������3Ϊ16/4-1
	Set_Row_Address(y,y+11); 
	Set_Write_RAM();	 //	д�Դ�
	 while(1)
	 {
		if(1==MY_Compare(HZ12X12[i].Index,num))
		{
			for(j=0;j<24;j++)
			{
				Con_4_byte(HZ12X12[i].Msk[j]);
			}
			break;
		}
		i++;
	}
}
void Show_MYHZ12_12(unsigned char  x,unsigned char  y, unsigned char  d,char num[])
{
  unsigned char  i,d1,cnt,j=0;
	char str[2]={0};
  d1=d+16;
	cnt=strlen(num);
  for(i=0;i<cnt/2;i++)
  {
		sprintf(str,"%c%c",num[j],num[j+1]);
		MYHZ12_12(x,y,str);
		x=x+12;
		j+=2;		
  }
}

void MYHZ16_16( unsigned char x, unsigned char y, char num[2])
{
	unsigned char x1,j,i=0 ;
	x1=x/4; 
	Set_Column_Address(Shift+x1,Shift+x1+5); // ���������꣬shiftΪ��ƫ������1322������3Ϊ16/4-1
	Set_Row_Address(y,y+23); 
	Set_Write_RAM();	 //	д�Դ�
	 while(1)
	 {
		if(1==MY_Compare(HZ12X12[i].Index,num))
		{
			for(j=0;j<24;j++)
			{
				Con_4_byte(HZ12X12[i].Msk[j]);//�˴�Ҫ�޸�
			}
			break;
		}
		i++;
	}
}
void Show_MYHZ16_16(unsigned char  x,unsigned char  y, unsigned char  d,char num[])
{
  unsigned char  i,d1,cnt,j=0;
	char str[2]={0};
  d1=d+16;
	cnt=strlen(num);
  for(i=0;i<cnt/2;i++)
  {
		sprintf(str,"%c%c",num[j],num[j+1]);//�˴�Ҫ�޸�
		MYHZ12_12(x,y,str);
		x=x+d1;
		j+=2;		
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//****************************************************
//   д��һ��12*12���� www.lcdsoc.com	x������ּ��dҪΪ4�ı���
//    num1,num2���������ֿ��е�λ��	 ��num1��ʾ��num2
//    x: Start Column  ��ʼ�� ��Χ 0~��255-16��
//    y: Start Row    ��ʼ�� 0~63
//    d:�ּ���� 0Ϊ�޼�� 
//*****************************************************
void Show_HZ12_12(unsigned char  x,unsigned char  y, unsigned char  d,unsigned char num1,unsigned char num2)
{
  unsigned char  i,d1;
  d1=d+16;
  for(i=num1;i<num2+1;i++)
  {
  HZ12_12(x,y,i);
  x=x+d1;             
  }
}


/***************************************************************
//  ��ʾ16*16������ 2015-05���²���ͨ��
//  ȡģ��ʽΪ������ȡģ���λ,��������:�����Ҵ��ϵ���    16�� 16�� 
//   num���������ֿ��е�λ��
//   x: Start Column  ��ʼ�� ��Χ 0~��256-16��
//   y: Start Row   ��ʼ�� 0~63 
***************************************************************/
void HZ16_16( unsigned char x, unsigned char y, unsigned char num)
{
	unsigned char x1,j ;
	x1=x/4; 
	Set_Column_Address(Shift+x1,Shift+x1+3); // ���������꣬shiftΪ��ƫ������1322������3Ϊ16/4-1
	Set_Row_Address(y,y+15); 
	Set_Write_RAM();	 //	д�Դ�
	 
	for(j=0;j<32;j++)
	{
		 Con_4_byte(HZ16X16_S[num*32+j]);
	}
  
		
}	

//****************************************************
//   д��һ��16*16���� www.lcdsoc.com
//    num1,num2���������ֿ��е�λ��	 ��num1��ʾ��num2
//    x: Start Column  ��ʼ�� ��Χ 0~��255-16��
//    y: Start Row    ��ʼ�� 0~63
//    d:�ּ���� 0Ϊ�޼�� 
//x������ּ��dҪΪ4�ı���
//*****************************************************
void Show_HZ16_16(unsigned char  x,unsigned char  y, unsigned char  d,unsigned char num1,unsigned char num2)
{
  unsigned char  i,d1;
  d1=d+16;
  for(i=num1;i<num2+1;i++)
  {
  HZ16_16(x,y,i);
  x=x+d1;             
  }
}

/***************************************************************
//  ��ʾ24*24������ 2015-05���²���ͨ��
//  ȡģ��ʽΪ������ȡģ���λ,��������:�����Ҵ��ϵ���    24�� 24�� 
//   num���������ֿ��е�λ��
//   x: Start Column  ��ʼ�� ��Χ 0~��255-16��
//   y: Start Row   ��ʼ�� 0~63 
***************************************************************/
void HZ24_24( unsigned char x, unsigned char y, unsigned char num)
{
	unsigned char x1,j ;
	x1=x/4; 
	Set_Column_Address(Shift+x1,Shift+x1+5); // ���������꣬shiftΪ��ƫ������1322������3Ϊ16/4-1
	Set_Row_Address(y,y+23); 
	Set_Write_RAM();	 //	д�Դ�
	 
	for(j=0;j<72;j++)
	{
		 Con_4_byte(HZ24X24_S[num*72+j]);	//����24*24 ������������Ҫ����24*24��HZ24X24_K.h�� ,����24*24��HZ24X24_H.h��������Ӧ��ͷ�ļ����޸Ĵ˴�������
	}
 		
}	

//****************************************************
//   д��һ��24*24���� www.lcdsoc.com
//    num1,num2���������ֿ��е�λ��	 ��num1��ʾ��num2
//    x: Start Column  ��ʼ�� ��Χ 0~��255-16��
//    y: Start Row    ��ʼ�� 0~63
//    d:�ּ���� 0Ϊ�޼�� 
//    x������ּ��dҪΪ4�ı���
//*****************************************************
void Show_HZ24_24(unsigned char  x,unsigned char  y, unsigned char  d,unsigned char num1,unsigned char num2)
{
  unsigned char  i,d1;
  d1=d+24;
  for(i=num1;i<num2+1;i++)
  {
  HZ24_24(x,y,i);
  x=x+d1;             
  }
}


 //==============================================================
//����������д��һ���׼ASCII�ַ���	 5x8
//��������ʾ��λ�ã�x,y����ch[]Ҫ��ʾ���ַ���
//���أ���
//==============================================================  
void Asc5_8(unsigned char x,unsigned char y,unsigned char ch[])
{
  unsigned char x1,c=0, i=0,j=0;      
  while (ch[i]!='\0')
  {    
    x1=x/4;
	c =ch[i]-32;
    if(x1>61)
	   {x=0;
	   x1=x/4;
	   y=y+8;}  //����																	
    Set_Column_Address(Shift+x1,Shift+x1+1); // ���������꣬shiftΪ��ƫ������1322���� 
	Set_Row_Address(y,y+7); 
	Set_Write_RAM();	 //	д�Դ�    
  	
		for(j=0;j<8;j++)
	 		  {
				 Con_4_byte(ASC5X8[c*8+j]);	//����ת��
			   }
	 i++;
	 x=x+8;	 //�ּ�࣬8Ϊ��С
  }
}
 //==============================================================
//����������д��һ���׼ASCII�ַ���	 6x12
//��������ʾ��λ�ã�x,y����ch[]Ҫ��ʾ���ַ���
//���أ���
//==============================================================  
void Asc6_12(unsigned char x,unsigned char y,unsigned char ch[])
{
  unsigned char x1,c=0, i=0,j=0;      
  while (ch[i]!='\0')
  {    
    x1=x/4;
	c =ch[i]-32;
    if(x1>61)
	   {x=0;
	   x1=x/4;
	   y=y+12;}  //����																	
    Set_Column_Address(Shift+x1,Shift+x1+1); // ���������꣬shiftΪ��ƫ������1322���� 
	Set_Row_Address(y,y+11); 
	Set_Write_RAM();	 //	д�Դ�    
  	
		for(j=0;j<12;j++)
	 		  {
				 Con_4_byte(ASC6X12[c*12+j]);	//����ת��
			   }
	 i++;
	 x=x+8;	 //�ּ�࣬8Ϊ��С
  }
}
//==============================================================
//����������д��һ���׼ASCII�ַ���	 8x16
//��������ʾ��λ�ã�x,y����ch[]Ҫ��ʾ���ַ���
//���أ���
//==============================================================  
void Asc8_16(unsigned char x,unsigned char y,unsigned char ch[])
{
  unsigned char x1,c=0, i=0,j=0;      
  while (ch[i]!='\0')
  {    
    x1=x/4;
	c =ch[i]-32;
    if(x1>61)
	   {x=0;
	   x1=x/4;
	   y=y+16;}  //����																	
    Set_Column_Address(Shift+x1,Shift+x1+1); // ���������꣬shiftΪ��ƫ������1322���� 
	Set_Row_Address(y,y+15); 
	Set_Write_RAM();	 //	д�Դ�    
		for(j=0;j<16;j++)
	 		  {
				 Con_4_byte(ASC8X16[c*16+j]);	//����ת��
			   }
	 i++;
	 x=x+8;	 //�ּ�࣬8Ϊ��С
  }
}
 
//==============================================================
//����������д��һ���׼ASCII�ַ���	 12x24
//��������ʾ��λ�ã�x,y����ch[]Ҫ��ʾ���ַ���
//���أ���
//==============================================================  
void Asc12_24(unsigned char x,unsigned char y,unsigned char ch[])
{
  unsigned char x1,c=0, i=0,j=0;      
  while (ch[i]!='\0')
  {    
    x1=x/4;
	c =ch[i]-32;
    if(x1>59)
	   {x=0;
	   x1=x/4;
	   y=y+24;}  //����																	
    Set_Column_Address(Shift+x1,Shift+x1+3); // ���������꣬shiftΪ��ƫ������1322���� 
	Set_Row_Address(y,y+23); 
	Set_Write_RAM();	 //	д�Դ�    
  	
		for(j=0;j<48;j++)
	 		  {
				 Con_4_byte(ASC12X24[c*48+j]);	//����ת��
			   }
	 i++;
	 x=x+16;//�ּ�࣬12Ϊ��С	
  }
}

//==============================================================
//����������д��һ���׼ASCII�ַ���	 20x40	 256*64 ֻ����ʾһ��
//��������ʾ��λ�ã�x,y����ch[]Ҫ��ʾ���ַ���
//���أ���
//==============================================================  
void Asc20_40(unsigned char x,unsigned char y,unsigned char ch[])
{
  unsigned char x1,c=0, i=0,j=0;      
  while (ch[i]!='\0')
  {    
    x1=x/4;
	c =ch[i]-32;
   /* if(x1>10)
	   {x=0;
	   x1=x/4;
	    }  //ֻ����ʾһ��		*/														
    Set_Column_Address(Shift+x1,Shift+x1+5); // ���������꣬shiftΪ��ƫ������1322���� 
	Set_Row_Address(y,y+39); 
	Set_Write_RAM();	 //	д�Դ�    
  	
		for(j=0;j<120;j++)
	 		  {
				 Con_4_byte(ASC20X40[c*120+j]);	//����ת��
			   }
	 i++;
	 x=x+24;//�ּ�࣬20Ϊ��С	
  }
}

void Grayscale()
{
	// Level 16 => Column 1~16
		Fill_Block(0xFF,0x00,0x03,0x00,Max_Row);
   	// Level 15 => Column 17~32
		Fill_Block(0xEE,0x04,0x07,0x00,Max_Row);
  	// Level 14 => Column 33~48
		Fill_Block(0xDD,0x08,0x0B,0x00,Max_Row);
 	// Level 13 => Column 49~64
		Fill_Block(0xCC,0x0C,0x0F,0x00,Max_Row);
  	// Level 12 => Column 65~80
		Fill_Block(0xBB,0x10,0x13,0x00,Max_Row);
 	// Level 11 => Column 81~96
		Fill_Block(0xAA,0x14,0x17,0x00,Max_Row);
 	// Level 10 => Column 97~112
		Fill_Block(0x99,0x18,0x1B,0x00,Max_Row);
 	// Level 9 => Column 113~128
		Fill_Block(0x88,0x1C,0x1F,0x00,Max_Row);
 	// Level 8 => Column 129~144
		Fill_Block(0x77,0x20,0x23,0x00,Max_Row);
  	// Level 7 => Column 145~160
		Fill_Block(0x66,0x24,0x27,0x00,Max_Row);
  	// Level 6 => Column 161~176
		Fill_Block(0x55,0x28,0x2B,0x00,Max_Row);
 	// Level 5 => Column 177~192
		Fill_Block(0x44,0x2C,0x2F,0x00,Max_Row);
  	// Level 4 => Column 193~208
		Fill_Block(0x33,0x30,0x33,0x00,Max_Row);
  	// Level 3 => Column 209~224
		Fill_Block(0x22,0x34,0x37,0x00,Max_Row);
   	// Level 2 => Column 225~240
		Fill_Block(0x11,0x38,0x3B,0x00,Max_Row);
		// Level 1 => Column 241~256
		Fill_Block(0x00,0x3C,Max_Column,0x00,Max_Row);
}

