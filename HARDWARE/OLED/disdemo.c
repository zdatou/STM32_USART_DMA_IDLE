#include "disdemo.h"
#include "sys.h"
#include "key.h"

unsigned char  KeyFuncIndex=0;    //��ŵ�ǰ�Ĳ˵�����
void (*KeyFuncPtr)(void);            //���尴������ָ��
void DisUpadte(void);
__align(4) unsigned char dtbuf[50];

//״̬������+�ṹ�壩
StatusTab KeyTab[MENU_SIZE];

void DisDemo(u8 Key_Value)
{	
	static uint8_t Key_pValue=0;
	switch(Key_Value)
	{
		case  Key_S2:			  
				KeyFuncIndex=KeyTab[KeyFuncIndex].KeyOKStatus;  //ȷ�ϰ���
				break; 
		case  Key_S0:			 
				KeyFuncIndex=KeyTab[KeyFuncIndex].KeyUpStatus;	//���ϰ���s2
				break; 
	  case  Key_S1:		      
		    KeyFuncIndex=KeyTab[KeyFuncIndex].KeyDownStatus; //���°���s3
				break; 
		case  Key_S3:		      
		    KeyFuncIndex=KeyTab[KeyFuncIndex].KeyLeftStatus; //���󰴼�s3
				break; 
		case  Key_S4:		
				KeyFuncIndex=KeyTab[KeyFuncIndex].KeyRightStatus; //���Ұ���
				break; 
		case  Key_S5:		
				KeyFuncIndex=KeyTab[KeyFuncIndex].KeyBackStatus; //���ؼ�
				break; 
		default : 
				break;
				//�˴���Ӱ����������
	}
	if(Key_pValue != KeyFuncIndex)
	{
		Key_pValue = KeyFuncIndex;
		Fill_RAM(0);//����
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
		
			KeyFuncIndex == NetDetermine3 //ȡ���ֶ�����ҳ�淵��ҳ
		
		)//������������IP����ʱ��ȡIP��ֵ
		{
				switch(KeyFuncIndex)
				{
					default:break;
				}
				if(KeyFuncIndex == NetDetermine3)
				{
					//ȡ���ֶ�����IP����ҳ�����½��Զ����õı���IPֵ����ʵ��ֵ
				}
		}
		if(KeyFuncIndex == ShowTimeOne||KeyFuncIndex == ShowTimeTwo||KeyFuncIndex == ShowTimeThr||
			 KeyFuncIndex == ShowTimeFour||KeyFuncIndex == ShowTimeFive||KeyFuncIndex == ShowTimeSix||
			 KeyFuncIndex == ShowTimeSeven||KeyFuncIndex == ShowTimeEight)
		{		
		}
							
	}
			 KeyFuncPtr=KeyTab[KeyFuncIndex].CurrentOperate;		//������ִ�а����Ĳ���
//		if(KeyFuncPtr==Demo_NET9_IPshow||KeyFuncPtr==Demo_NET10_IPshow||KeyFuncPtr==Demo_NET11_IPshow||KeyFuncPtr==Demo_NET12_IPshow||
//			 KeyFuncPtr==Demo_NET13_IPshow||KeyFuncPtr==Demo_NET14_IPshow||KeyFuncPtr==Demo_NET15_IPshow||KeyFuncPtr==Demo_NET16_IPshow
//			)
//			{
//				cnt++;
//				if(cnt>=4)
//				{
//					cnt=0;
//					(*KeyFuncPtr)();     //ִ�е�ǰ�İ���������ҳ����ʾ��	
//				}
//			}
//			else
//			{
//				(*KeyFuncPtr)();     //ִ�е�ǰ�İ���������ҳ����ʾ��	
//			}
}


void Demo_OpMenu1(void)
{
}



