#include "disdemo.h"
#include "sys.h"
#include "key.h"

unsigned char  KeyFuncIndex=0;    //��ŵ�ǰ�Ĳ˵�����
void (*KeyFuncPtr)(void);            //���尴������ָ��
void DisUpadte(void);
__align(4) unsigned char dtbuf[50];

u8 ChannelPageIndex = 1;
u8 RefrenceChannel = 1;
timeshow_t temp_time[8] = {
	{1,2,3,4},
	{5,6,7,8},
	{1,2,3,4},
	{5,6,7,8},
	{1,2,3,4},
	{5,6,7,8},
	{1,2,3,4},
	{5,6,7,8}
};
//״̬������+�ṹ�壩
StatusTab KeyTab[MENU_SIZE] = {
	{SelectMenu1, 			DataShowMenu, 			 SelectMenu1, 		SelectMenu2, 		SelectMenu1, 		SelectMenu1, 		SelectMenu1, Demo_OpMenu1},
	{SelectMenu2, 			ChannelTrigSet, 		 SelectMenu1, 		SelectMenu3, 		SelectMenu2, 		SelectMenu2,		SelectMenu2, Demo_OpMenu2},
	{SelectMenu3, 			RefrenceSet, 			 SelectMenu2, 		SelectMenu4, 		SelectMenu3, 		SelectMenu3,		SelectMenu3, Demo_OpMenu3},
	{SelectMenu4, 			RefrenceChaneelSelect, 	 SelectMenu3, 		SelectMenu4, 		SelectMenu4, 		SelectMenu4, 		SelectMenu4, Demo_OpMenu4},
	
	{DataShowMenu,  		DataShowMenu, 	 		 DataShowMenu, 		DataShowMenu, 		DataShowMenu, 		DataShowMenu, 		SelectMenu1, Demo_DataShow},
	{ChannelTrigSet,  		DataShowMenu, 	 		 SelectMenu3, 		SelectMenu4, 		SelectMenu4, 		SelectMenu4, 		SelectMenu4, Demo_OpMenu4},
	{RefrenceSet,  			DataShowMenu, 	 		 SelectMenu3, 		SelectMenu4, 		SelectMenu4, 		SelectMenu4, 		SelectMenu4, Demo_OpMenu4},
	{RefrenceChaneelSelect, DataShowMenu, 	 		 SelectMenu3, 		SelectMenu4, 		SelectMenu4, 		SelectMenu4, 		SelectMenu4, Demo_OpMenu4},
};	

void DisDemo(u8 Key_Value)
{	
	static uint8_t Key_pValue=100;
	switch(Key_Value)
	{
		case  KEY_OK_PRES:			  
				KeyFuncIndex=KeyTab[KeyFuncIndex].KeyOKStatus;  //ȷ�ϰ���
				break; 
		case  KEY_UP_PRES:			 
				KeyFuncIndex=KeyTab[KeyFuncIndex].KeyUpStatus;	//���ϰ���s2
				break; 
	  case  KEY_DOWN_PRES:		      
		    KeyFuncIndex=KeyTab[KeyFuncIndex].KeyDownStatus; //���°���s3
				break; 
		case  KEY_LEFT_PRES:		      
		    KeyFuncIndex=KeyTab[KeyFuncIndex].KeyLeftStatus; //���󰴼�s3
				break; 
		case  KEY_RIGHT_PRES:		
				KeyFuncIndex=KeyTab[KeyFuncIndex].KeyRightStatus; //���Ұ���
				break; 
		case  KEY_CANNCE_PRES:		
				KeyFuncIndex=KeyTab[KeyFuncIndex].KeyBackStatus; //���ؼ�
				break; 
		default : 
				break;
				//�˴���Ӱ����������
	}
	if(Key_pValue != KeyFuncIndex)
	{
		Key_pValue = KeyFuncIndex;
		OLED_Clear();
		BEEP(0);	
	    KeyFuncPtr=KeyTab[KeyFuncIndex].CurrentOperate;		//������ִ�а����Ĳ���
		(*KeyFuncPtr)();		
	}
}

void Main_Show(void)
{

}

void Demo_OpMenu1(void)
{
	OLED_ShowText_Middle(0, 0, (u8 *)"���˵�",           	12, 1, 0xff);
	OLED_ShowText_Middle(0, 16, (u8 *)"��������",  			12, 0, 0xff);
	OLED_ShowText_Middle(0, 32, (u8 *)"������ƽ����",		12, 1, 0xff);
	OLED_ShowText_Middle(0, 48, (u8 *)"�ⲿ���ڲ��ο�����", 12, 1, 0xff);
}

void Demo_OpMenu2(void)
{
	OLED_ShowText_Middle(0, 0, (u8 *)"���˵�",           	12, 1, 0xff);
	OLED_ShowText_Middle(0, 16, (u8 *)"��������",  			12, 1, 0xff);
	OLED_ShowText_Middle(0, 32, (u8 *)"������ƽ����",		12, 0, 0xff);
	OLED_ShowText_Middle(0, 48, (u8 *)"�ⲿ���ڲ��ο�����", 12, 1, 0xff);
}

void Demo_OpMenu3(void)
{
	OLED_ShowText_Middle(0, 0, (u8 *)"���˵�",           	12, 1, 0xff);
	OLED_ShowText_Middle(0, 16, (u8 *)"��������",  			12, 1, 0xff);
	OLED_ShowText_Middle(0, 32, (u8 *)"������ƽ����",		12, 1, 0xff);
	OLED_ShowText_Middle(0, 48, (u8 *)"�ⲿ���ڲ��ο�����", 12, 0, 0xff);
}

void Demo_OpMenu4(void)
{
	OLED_ShowText_Middle(0, 0, (u8 *)"���˵�",           	12, 1, 0xff);
	OLED_ShowText_Middle(0, 16, (u8 *)"������ƽ����",  			12, 1, 0xff);
	OLED_ShowText_Middle(0, 32, (u8 *)"�ⲿ���ڲ��ο�����",		12, 1, 0xff);
	OLED_ShowText_Middle(0, 48, (u8 *)"�ο�ͨ������", 12, 1, 0xff);
}

void Demo_DataShow(void)
{
	u8 buf[48];	
	u8 shiftx = 20;
	u8 shifty = 6;
	sprintf((char *)buf, "��ǰͨ��CH%1d                �ο�ͨ��:CH%1d", ChannelPageIndex, RefrenceChannel);
	OLED_ShowText(16,12, buf, 12, 1, 0xff);
	sprintf((char *)buf, "0. %03d %03d %03d %03d",temp_time[ChannelPageIndex].ms, temp_time[ChannelPageIndex].us, temp_time[ChannelPageIndex].ns, temp_time[ChannelPageIndex].ps);
	OLED_ShowString(32+shiftx, 24+shifty, buf, 16, 0xff);
	OLED_ShowString_Line(52+shiftx, 38+shifty, " ms  us  ns  ps", 16, 0xff);
	OLED_ShowString(20, 35, "<", 24, 0xff);
	OLED_ShowString(220, 35, ">", 24, 0xff);

}

