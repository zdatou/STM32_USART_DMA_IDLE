#include "disdemo.h"
#include "sys.h"
#include "key.h"

unsigned char  KeyFuncIndex=0;    //存放当前的菜单索引
void (*KeyFuncPtr)(void);            //定义按键功能指针
void DisUpadte(void);
__align(4) unsigned char dtbuf[50];

//状态表（数组+结构体）
StatusTab KeyTab[MENU_SIZE];

void DisDemo(u8 Key_Value)
{	
	static uint8_t Key_pValue=0;
	switch(Key_Value)
	{
		case  Key_S2:			  
				KeyFuncIndex=KeyTab[KeyFuncIndex].KeyOKStatus;  //确认按键
				break; 
		case  Key_S0:			 
				KeyFuncIndex=KeyTab[KeyFuncIndex].KeyUpStatus;	//向上按键s2
				break; 
	  case  Key_S1:		      
		    KeyFuncIndex=KeyTab[KeyFuncIndex].KeyDownStatus; //向下按键s3
				break; 
		case  Key_S3:		      
		    KeyFuncIndex=KeyTab[KeyFuncIndex].KeyLeftStatus; //向左按键s3
				break; 
		case  Key_S4:		
				KeyFuncIndex=KeyTab[KeyFuncIndex].KeyRightStatus; //向右按键
				break; 
		case  Key_S5:		
				KeyFuncIndex=KeyTab[KeyFuncIndex].KeyBackStatus; //返回键
				break; 
		default : 
				break;
				//此处添加按键错误代码
	}
	if(Key_pValue != KeyFuncIndex)
	{
		Key_pValue = KeyFuncIndex;
		Fill_RAM(0);//清屏
//		BEEP = 0;
		if(KeyFuncIndex == SetNetNightPage||KeyFuncIndex == SetNetTenPage||KeyFuncIndex == SetNetElevenPage||
			 KeyFuncIndex == SetNetTwelvePage||KeyFuncIndex == SetNetThirteenPage||KeyFuncIndex == SetNetFourteenPage||
			 KeyFuncIndex == SetNetFifteenPage||KeyFuncIndex == SetNetSixteenPage||
		
			KeyFuncIndex == OptNetOnePage||KeyFuncIndex == OptNetTwoPage||KeyFuncIndex == OptNetThrPage||
		  KeyFuncIndex == OptNetFourPage||KeyFuncIndex == OptNetFivePage||KeyFuncIndex == OptNetSixPage||
		  KeyFuncIndex == OptNetSevenPage||KeyFuncIndex == OptNetEightPage||
		
		  KeyFuncIndex == SetNetOnePage||KeyFuncIndex == SetNetTwoPage||KeyFuncIndex == SetNetThrPage||
		  KeyFuncIndex == SetNetFourPage||KeyFuncIndex == SetNetFivePage||KeyFuncIndex == SetNetSixPage||
		  KeyFuncIndex == SetNetSevenPage||KeyFuncIndex == SetNetEightPage||
		
			KeyFuncIndex == NetDetermine3 //取消手动设置页面返回页
		
		)//界面跳到设置IP界面时读取IP数值
		{
				switch(KeyFuncIndex)
				{
					default:break;
				}
				if(KeyFuncIndex == NetDetermine3)
				{
					//取消手动设置IP返回页，重新将自动设置的本地IP值赋予实际值
				}
		}
		if(KeyFuncIndex == ShowTimeOne||KeyFuncIndex == ShowTimeTwo||KeyFuncIndex == ShowTimeThr||
			 KeyFuncIndex == ShowTimeFour||KeyFuncIndex == ShowTimeFive||KeyFuncIndex == ShowTimeSix||
			 KeyFuncIndex == ShowTimeSeven||KeyFuncIndex == ShowTimeEight)
		{		
		}
							
	}
			 KeyFuncPtr=KeyTab[KeyFuncIndex].CurrentOperate;		//下面是执行按键的操作
//		if(KeyFuncPtr==Demo_NET9_IPshow||KeyFuncPtr==Demo_NET10_IPshow||KeyFuncPtr==Demo_NET11_IPshow||KeyFuncPtr==Demo_NET12_IPshow||
//			 KeyFuncPtr==Demo_NET13_IPshow||KeyFuncPtr==Demo_NET14_IPshow||KeyFuncPtr==Demo_NET15_IPshow||KeyFuncPtr==Demo_NET16_IPshow
//			)
//			{
//				cnt++;
//				if(cnt>=4)
//				{
//					cnt=0;
//					(*KeyFuncPtr)();     //执行当前的按键操作（页面显示）	
//				}
//			}
//			else
//			{
//				(*KeyFuncPtr)();     //执行当前的按键操作（页面显示）	
//			}
}


void Demo_OpMenu1(void)
{
}



