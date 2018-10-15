#include "disdemo.h"
#include "sys.h"
#include "key.h"
#include "stdlib.h"
#include "string.h"
unsigned char  KeyFuncIndex=0;    //存放当前的菜单索引
void (*KeyFuncPtr)(void);            //定义按键功能指针
void DisUpadte(void);
__align(4) unsigned char dtbuf[50];

u8 ChannelPageIndex = 1;
u8 RefrenceChannel = 1;
char *EIBuff[] = {"内部", "外部"};
u8 EIFlag = 1;
u8 opt_bit = 2;	
u8 value_bit = 0;

timeshow_t test_time[8] = {
	{1,2,3,4},
	{5,6,7,8},
	{1,2,3,4},
	{5,6,7,8},
	{1,2,3,4},
	{5,6,7,8},
	{1,2,3,4},
	{5,6,7,8}
};
float TriggleVolatage[8] = {1.0, 2.0, 3.0, 4.0, 5.0, 1.2, 3.4, 5.0};
//状态表（数组+结构体）
StatusTab KeyTab[MENU_SIZE] = {
	{SelectMenu1, 			DataShowMenu, 				SelectMenu1, 		   SelectMenu2, 		  SelectMenu1, 			 SelectMenu1, 			SelectMenu1, Demo_OpMenu1},
	{SelectMenu2, 			ChannelTrigSet, 		 	SelectMenu1, 		   SelectMenu3, 		  SelectMenu2, 			 SelectMenu2,			SelectMenu2, Demo_OpMenu2},
	{SelectMenu3, 			RefrenceSet, 				SelectMenu2, 		   SelectMenu4, 		  SelectMenu3, 			 SelectMenu3,			SelectMenu3, Demo_OpMenu3},
	{SelectMenu4, 			RefrenceChaneelSelect, 	 	SelectMenu3, 		   SelectMenu4, 		  SelectMenu4, 			 SelectMenu4, 			SelectMenu4, Demo_OpMenu4},
	
	{DataShowMenu,  		DataShowMenu, 	 			DataShowMenu, 		   DataShowMenu, 		  DataShowMenu, 		 DataShowMenu, 			SelectMenu1, Demo_DataShow},
	{ChannelTrigSet,  		ChannelTrigSet, 	 		ChannelTrigSet,        ChannelTrigSet,        ChannelTrigSet, 		 ChannelTrigSet, 		SelectMenu2, Demo_TriSet},
	{RefrenceSet,  			RefrenceSet, 	 		 	RefrenceSet, 		   RefrenceSet, 		  RefrenceSet, 			 RefrenceSet, 			SelectMenu3, Demo_E_I_Set},
	{RefrenceChaneelSelect, RefrenceChaneelSelect, 	 	RefrenceChaneelSelect, RefrenceChaneelSelect, RefrenceChaneelSelect, RefrenceChaneelSelect, SelectMenu4, Demo_RefChannel},
};	

void DisDemo(u8 Key_Value)
{	
	static uint8_t Key_pValue=100;
	switch(Key_Value)
	{
		case  KEY_OK_PRES:			  
				KeyFuncIndex=KeyTab[KeyFuncIndex].KeyOKStatus;  //确认按键
				break; 
		case  KEY_UP_PRES:			 
				KeyFuncIndex=KeyTab[KeyFuncIndex].KeyUpStatus;	//向上按键s2
				break; 
	  case  KEY_DOWN_PRES:		      
		    KeyFuncIndex=KeyTab[KeyFuncIndex].KeyDownStatus; //向下按键s3
				break; 
		case  KEY_LEFT_PRES:		      
		    KeyFuncIndex=KeyTab[KeyFuncIndex].KeyLeftStatus; //向左按键s3
				break; 
		case  KEY_RIGHT_PRES:		
				KeyFuncIndex=KeyTab[KeyFuncIndex].KeyRightStatus; //向右按键
				break; 
		case  KEY_CANNCE_PRES:		
				KeyFuncIndex=KeyTab[KeyFuncIndex].KeyBackStatus; //返回键
				break; 
		default : 
				break;
				//此处添加按键错误代码
	}
	if((Key_pValue != KeyFuncIndex))
	{
		Key_pValue = KeyFuncIndex;
		OLED_Clear();	
	}
	else 
	{
		Key_Handler(Key_Value, KeyFuncIndex);
	}
//	else if(KeyFuncIndex == DataShowMenu)
//	{
//		switch(Key_Value)
//		{
//			case KEY_LEFT_PRES:
//				ChannelPageIndex--;
//				(ChannelPageIndex>7)?ChannelPageIndex = 7:0;
//				break;
//			case KEY_RIGHT_PRES:
//				ChannelPageIndex++;
//				(ChannelPageIndex>7)?ChannelPageIndex = 0:0;
//				break;
//			default:
//				break;
//		}
//	}
//	else if(KeyFuncIndex == ChannelTrigSet)
//	{
//		switch(Key_Value)
//		{
//			case KEY_UP_PRES:
//				TriggleVolatage[ChannelPageIndex] = Value_Opt(TriggleVolatage[ChannelPageIndex], value_bit, 1);				
//				break;
//			case KEY_DOWN_PRES:
//				TriggleVolatage[ChannelPageIndex] = Value_Opt(TriggleVolatage[ChannelPageIndex], value_bit, 0);
//				break;
//			case KEY_LEFT_PRES:
//				opt_bit--; value_bit++;
//				break;
//			case KEY_RIGHT_PRES:
//				ChannelPageIndex++;
//				(ChannelPageIndex>7)?ChannelPageIndex = 0:0;
//				break;
//			default:
//				break;
//		}
//		if(opt_bit == 1) opt_bit = 0;
//		else if(opt_bit > 10) opt_bit = 2;
//		if(value_bit > 1) value_bit = 0;
//	}
//	else if(KeyFuncIndex == RefrenceSet)
//	{
//		switch(Key_Value)
//		{
//			case KEY_LEFT_PRES:
//				EIFlag = 0;
//				break;
//			case KEY_RIGHT_PRES:
//				EIFlag = 1;
//				break;
//		}
//	}
//	else if(KeyFuncIndex == RefrenceChaneelSelect)
//	{
//		switch(Key_Value)
//		{
//			case KEY_LEFT_PRES:
//				RefrenceChannel--;
//				(RefrenceChannel>7)?RefrenceChannel = 7:0;
//				break;
//			case KEY_RIGHT_PRES:
//				RefrenceChannel++;
//				(RefrenceChannel>7)?RefrenceChannel = 0:0;
//				break;
//			default:
//				break;
//		}
//	}
	KeyFuncPtr=KeyTab[KeyFuncIndex].CurrentOperate;		//下面是执行按键的操作
	(*KeyFuncPtr)();	
}

void Demo_OpMenu1(void)
{	
	OLED_ShowText(0, 0, (u8 *)"主菜单\n> 测量结果\n触发电平设置\n外部与内部参考设置", 12, 1, 1, 0xff);	//自动换行 居中对齐
}

void Demo_OpMenu2(void)
{
	OLED_ShowText(0, 0, (u8 *)"主菜单\n测量结果\n> 触发电平设置\n外部与内部参考设置", 12, 1, 1, 0xff);
}

void Demo_OpMenu3(void)
{
	OLED_ShowText(0, 0, (u8 *)"主菜单\n测量结果\n触发电平设置\n> 外部与内部参考设置", 12, 1, 1, 0xff);
}

void Demo_OpMenu4(void)
{
	OLED_ShowText(0, 0, (u8 *)"主菜单\n触发电平设置\n外部与内部参考设置\n> 参考通道设置", 12, 1, 1, 0xff);
}

void Demo_DataShow(void)
{
	u8 buf[48];	
	u8 shiftx = 20;
	u8 shifty = 6;

	sprintf((char *)buf, "。当前通道CH%1d            参考通道:CH%1d", ChannelPageIndex+1, RefrenceChannel+1);
	OLED_ShowText(0,0, buf, 12, 1, 0, 0xff);	
	sprintf((char *)buf, "0. %03d %03d %03d %03d",test_time[ChannelPageIndex].ms, test_time[ChannelPageIndex].us, test_time[ChannelPageIndex].ns, test_time[ChannelPageIndex].ps);
	OLED_ShowString(32+shiftx, 16+shifty, buf, 16, 0, 0xff);	
	Asc8_16(32+shiftx, 16+shifty, buf); //快速刷新显示的时间
	OLED_ShowString(54+shiftx, 32+shifty, (u8 *)" ms  us  ns  ps", 16, 1, 0xff);//带上划线
	OLED_ShowString(shiftx, 22+shifty, (u8 *)"<", 24, 0, 0xff);
	OLED_ShowString(200+shiftx, 22+shifty, (u8 *)">", 24, 0, 0xff);
}

void Demo_TriSet(void)
{
	u8 buf[48];
	u8 shiftx = 20;
	u8 shifty = 6;
	sprintf((char *)buf, "。当前通道CH%1d\n\n            触发电平(0-5V):", ChannelPageIndex+1);
	OLED_ShowText(0, shifty, buf, 12, 1, 0, 0xff);
	OLED_ShowFloat(148+shiftx, 24+shifty, TriggleVolatage[ChannelPageIndex], 1, 12, opt_bit);
	OLED_ShowChar(148+shiftx+18, 24+shifty, 'V', 12, 1, 0xff);
	OLED_ShowString(200+shiftx, 22+shifty, (u8 *)">", 24, 0, 0xff);
}

void Demo_E_I_Set(void)
{
	u8 buf[100];
	sprintf((char *)buf, "。当前参考:\n\n           <        %s        >", (EIBuff[EIFlag]));
	OLED_ShowText(0, 12, buf, 12, 1, 0, 0xff);
}

void Demo_RefChannel(void)
{
	u8 buf[100];
	sprintf((char *)buf, "。当前参考通道:\n\n           <        CH%1d        >", RefrenceChannel+1);
	OLED_ShowText(0, 12, buf, 12, 1, 0, 0xff);
}

void Key_Handler(u8 key_value, u8 keyfuncIndex)
{
	switch(keyfuncIndex)
	{
		case DataShowMenu:
			DataShowMenu_Key_Handler(key_value);
			break;
		case ChannelTrigSet:
			ChannelTrigSet_Key_Handler(key_value);
			break;
		case RefrenceSet:
			RefrenceSet_Key_Handler(key_value);
			break;
		case RefrenceChaneelSelect:
			RefrenceChaneelSelect_Key_Handler(key_value);
			break;
	}
}

void DataShowMenu_Key_Handler(u8 key_value)
{
	switch(key_value)
	{
		case KEY_LEFT_PRES:
			ChannelPageIndex--;
			(ChannelPageIndex>7)?ChannelPageIndex = 7:0;
			break;
		case KEY_RIGHT_PRES:
			ChannelPageIndex++;
			(ChannelPageIndex>7)?ChannelPageIndex = 0:0;
			break;
		default:
			break;
	}
}

void ChannelTrigSet_Key_Handler(u8 key_value)
{
	switch(key_value)
	{
		case KEY_UP_PRES:
			TriggleVolatage[ChannelPageIndex] = Value_Opt(TriggleVolatage[ChannelPageIndex], value_bit, 1);				
			break;
		case KEY_DOWN_PRES:
			TriggleVolatage[ChannelPageIndex] = Value_Opt(TriggleVolatage[ChannelPageIndex], value_bit, 0);
			break;
		case KEY_LEFT_PRES:
			opt_bit--; value_bit++;
			break;
		case KEY_RIGHT_PRES:
			ChannelPageIndex++;
			(ChannelPageIndex>7)?ChannelPageIndex = 0:0;
			break;
		default:
			break;
	}
	if(opt_bit == 1) opt_bit = 0;
	else if(opt_bit > 10) opt_bit = 2;
	if(value_bit > 1) value_bit = 0;
}

void RefrenceSet_Key_Handler(u8 key_value)
{
	switch(key_value)
	{
		case KEY_LEFT_PRES:
			EIFlag = 0;
			break;
		case KEY_RIGHT_PRES:
			EIFlag = 1;
			break;
	}
}

void RefrenceChaneelSelect_Key_Handler(u8 key_value)
{
	switch(key_value)
	{
		case KEY_LEFT_PRES:
			RefrenceChannel--;
			(RefrenceChannel>7)?RefrenceChannel = 7:0;
			break;
		case KEY_RIGHT_PRES:
			RefrenceChannel++;
			(RefrenceChannel>7)?RefrenceChannel = 0:0;
			break;
		default:
			break;
	}
}

float Value_Opt(float value,u8 opt,u8 addorsub)
{
	int temp=0; 
	value = value*10;
	temp=(int)value;	
	if(addorsub)temp=temp+mypow(10,opt);
	else temp=temp-mypow(10,opt);	
	if(temp<0) return 0;
	if(temp>50) return 5.0f; 
	return (temp/10.0f + 0.001f);
}
