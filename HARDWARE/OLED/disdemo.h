#ifndef _DisDemo_H
#define _DisDemo_H
#include "sys.h"
#include "oled.h"
#define MENU_SIZE 200          //�˵�����
////////////////////////////////////////////////////////////////////////

extern unsigned char  KeyFuncIndex;    //��ŵ�ǰ�Ĳ˵�����

//����һ���˵��ڵ�Ľṹ������
typedef struct 
{
   unsigned char KeyStatusIndex;   //��ǰ��״̬������
   unsigned char KeyOKStatus;   	//����ȷ�ϼ�ʱ��״̬������
   unsigned char KeyUpStatus;      //�������ϼ�ʱ��״̬������     
   unsigned char KeyDownStatus;    //�������¼�ʱ��״̬������
	 unsigned char KeyLeftStatus;    //���������ʱ��״̬������
   unsigned char KeyRightStatus;     //�������Ҽ�ʱ��״̬������
	 unsigned char KeyBackStatus;     //�����򷵻�ʱ��״̬������
	
   void (*CurrentOperate)(void);      //��ǰ״̬Ӧ��ִ�еĹ��ܲ���
}StatusTab;

//ip��ַ
typedef struct
{
	unsigned char ipaddr[4];
	unsigned char netmask[4];
	unsigned char gatway[4];
}ip_addr;


typedef enum {
	TimeShow = 0, //Page 0
	
	//���˵�
	MainMenuSetIp , //Page 1
	//ʱ����ʾ���豸��Ϣѡ��ҳ
	MainMenuShowTime,  
	MainMenuShowDevInf,

//ѡ��ʱ����ʾҳ	
	OpShowTimePage1,
	OpShowTimePage2,
	OpShowTimePage3,
	OpShowTimePage4,
	OpShowTimePage5,
	OpShowTimePage6,
	OpShowTimePage7,
	OpShowTimePage8,
//ʱ����ʾҳ	
	ShowTimeOne,    
	ShowTimeTwo,    
	ShowTimeThr,   
	ShowTimeFour,   
	ShowTimeFive,  
	ShowTimeSix,   
	ShowTimeSeven,  
	ShowTimeEight,
//�豸��Ϣ��ʾҳ	
	ShowDevInf,  
	RefreshMeas1,
	RefreshMeas2,
	RefreshMeas3,
	Return0,
	//ѡ��ip���ý���
	OptNetOnePage,
	OptNetTwoPage,
	OptNetThrPage,
	OptNetFourPage,
	OptNetFivePage,    
	OptNetSixPage,     
	OptNetSevenPage,   
	OptNetEightPage,   

	SaveIpPage,
	ContainSet1,
	ContainSet2,
	ContainSet3,

	//IP���ý���
	SetNetOnePage,
	SetNetTwoPage,
	SetNetThrPage,
	SetNetFourPage,
	SetNetFivePage,    
	SetNetSixPage,     
	SetNetSevenPage,   
	SetNetEightPage,   
	
	SetNetNightPage,   
	SetNetTenPage,     
	SetNetElevenPage,  
	SetNetTwelvePage,  
	SetNetThirteenPage,
	SetNetFourteenPage,
	SetNetFifteenPage, 
	SetNetSixteenPage, 
	
	NetDetermine1,
	NetDetermine2,
	NetDetermine3,
	SetLocalIp,
	//save cancel opt 
	OptSavePage,
	OptCancelPage,
	Return,
	
	
	//bool Page                              
	CancelPage,
	SavePage,
	BackPage,
	
} MenuPage; 


void DisDemo(uint8_t Key_Value);//ѡ�����

#endif

