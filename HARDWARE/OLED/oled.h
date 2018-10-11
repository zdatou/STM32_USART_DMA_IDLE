//////////////////////////////////////////////////////////////////////////////////	 
 //ʹ��SysTick����ͨ����ģʽ���ӳٽ��й���
   //********************************************************************************
#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	    	

#define OLED_MODE 0
#define SIZE 16
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column		0x3F			// 256/4-1
#define Max_Row		0x3F			// 64-1
#define	Brightness	0xFF 
#define	Shift		0x1C
	    						  
//-----------------OLED�˿ڶ��� 4��SPI----------------  					   
/* ����IO��ģ��SPIͨѶ��ʽ����oledģ��
CS   ~ PB8
RES ~ PB4
DC   ~ PB7
CLK  ~ PB6
DIN ~ PB5
*/
#define OLED_CS_Clr()  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_RESET)//CS
#define OLED_CS_Set()  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9, GPIO_PIN_SET)

#define OLED_RST_Clr() HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10, GPIO_PIN_RESET)//RES
#define OLED_RST_Set() HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10, GPIO_PIN_SET)

#define OLED_DC_Clr() HAL_GPIO_WritePin(GPIOD,GPIO_PIN_11, GPIO_PIN_RESET)//DC
#define OLED_DC_Set() HAL_GPIO_WritePin(GPIOD,GPIO_PIN_11, GPIO_PIN_SET)

#define OLED_SCLK_Clr() HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12, GPIO_PIN_RESET)//CLK
#define OLED_SCLK_Set() HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12, GPIO_PIN_SET)

#define OLED_SDIN_Clr() HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13, GPIO_PIN_RESET)//DIN
#define OLED_SDIN_Set() HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13, GPIO_PIN_SET)

//���Ŷ���
/**/
#define LcdControl_GPIO_Port  GPIOC
#define LcdSCL_Pin GPIO_PIN_4
#define LcdSDA_Pin GPIO_PIN_5
#define LcdRST_Pin GPIO_PIN_0
#define LcdDC_Pin  GPIO_PIN_1
#define LCD_CLK_ENABLE()   do{__HAL_RCC_GPIOC_CLK_ENABLE();\
							  __HAL_RCC_GPIOB_CLK_ENABLE();}while(0)
/**/


#define LCD_SCL(x)  x?HAL_GPIO_WritePin(LcdControl_GPIO_Port,LcdSCL_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(LcdControl_GPIO_Port,LcdSCL_Pin,GPIO_PIN_RESET)      //SCLK  ʱ�� D0��SCLK��
#define LCD_SDA(x)  x?HAL_GPIO_WritePin(LcdControl_GPIO_Port,LcdSDA_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(LcdControl_GPIO_Port,LcdSDA_Pin,GPIO_PIN_RESET)      //SDA   D1��MOSI�� ����
#define LCD_RST(x)  x?HAL_GPIO_WritePin(LcdControl_GPIO_Port,LcdRST_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(LcdControl_GPIO_Port,LcdRST_Pin,GPIO_PIN_RESET)  //_RES  hardware reset   ��λ 
#define LCD_DC(x)   x?HAL_GPIO_WritePin(LcdControl_GPIO_Port,LcdDC_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(LcdControl_GPIO_Port,LcdDC_Pin,GPIO_PIN_RESET)   //A0  H/L ��������ѡͨ�ˣ�H�����ݣ�L:����





 		     
#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����
//OLED�����ú���
void OLED_WR_Byte(u8 dat,u8 cmd);
void OLED_Init(void);
void Set_Column_Address(unsigned char a, unsigned char b);
void Set_Row_Address(unsigned char a, unsigned char b);
void Set_Write_RAM(void);
void Set_Read_RAM(void);
void Fill_RAM(unsigned char Data);
void Draw_Line(unsigned char Data, unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e);
void Draw_Rectangle(unsigned char Data, unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e);
void Fill_Block(unsigned char Data, unsigned char a, unsigned char b, unsigned char c, unsigned char d);
void Grayscale(void);
void Set_Remap_Format(unsigned char d);
void Show_Pattern(unsigned char *Data_Pointer, unsigned char a, unsigned char b, unsigned char c, unsigned char d);
void Con_4_byte(unsigned char DATA);
void Set_Partial_Display(unsigned char a, unsigned char b, unsigned char c);
void Set_Display_Offset(unsigned char d);
void Set_Start_Line(unsigned char d);
void Set_Master_Current(unsigned char d);
void Vertical_Scroll(unsigned char a, unsigned char b, unsigned char c);
void Set_Gray_Scale_Table(void);
void Set_Linear_Gray_Scale_Table(void); 
 //����ASCII����
void Asc5_8(unsigned char x,unsigned char y,unsigned char ch[]);
void Asc6_12(unsigned char x,unsigned char y,unsigned char ch[]);
void Asc8_16(unsigned char x,unsigned char y,unsigned char ch[]);
void Asc12_24(unsigned char x,unsigned char y,unsigned char ch[]);
void Asc20_40(unsigned char x,unsigned char y,unsigned char ch[]);
 //���º��ֳ���
void HZ16_16( unsigned char x, unsigned char y, unsigned char num);
void HZ12_12( unsigned char x, unsigned char y, unsigned char num);
void HZ24_24( unsigned char x, unsigned char y, unsigned char num);
void Show_HZ16_16(unsigned char  x,unsigned char  y, unsigned char  d,unsigned char num1,unsigned char num2);
void Show_HZ12_12(unsigned char  x,unsigned char  y, unsigned char  d,unsigned char num1,unsigned char num2);
void Show_HZ24_24(unsigned char  x,unsigned char  y, unsigned char  d,unsigned char num1,unsigned char num2);

void MYHZ12_12( unsigned char x, unsigned char y, char num[2]);
void Show_MYHZ12_12(unsigned char  x,unsigned char  y, unsigned char  d,char num[]);
void MYHZ16_16( unsigned char x, unsigned char y, char num[2]);
void Show_MYHZ16_16(unsigned char  x,unsigned char  y, unsigned char  d,char num[]);


void OLED_ShowString(u8 x,u8 y, u8 *p,u8 size, u8 gray);
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr, u8 size, u8 mode, u8 gray);
void OLED_Refresh_Gram(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_ShowText(u8 x,u8 y, u8 *str,u8 size, u8 mode, u8 gray);
void OLED_Clear(void);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size, u8 gray);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
#endif  
	 



