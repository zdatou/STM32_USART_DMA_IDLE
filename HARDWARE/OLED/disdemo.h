#ifndef _DisDemo_H
#define _DisDemo_H
#include "sys.h"
#include "oled.h"
#include "all.h"
#define MENU_SIZE 200          //菜单长度
////////////////////////////////////////////////////////////////////////

extern unsigned char  KeyFuncIndex;    //存放当前的菜单索引

extern uint8 ntp_Ceshi;
//定义一个菜单节点的结构体链表
typedef struct 
{
   unsigned char KeyStatusIndex;   //当前的状态索引号
   unsigned char KeyOKStatus;   	//按下确认键时的状态索引号
   unsigned char KeyUpStatus;      //按下向上键时的状态索引号     
   unsigned char KeyDownStatus;    //按下向下键时的状态索引号
	 unsigned char KeyLeftStatus;    //按下向左键时的状态索引号
   unsigned char KeyRightStatus;     //按下向右键时的状态索引号
	 unsigned char KeyBackStatus;     //按下向返回时的状态索引号
	
   void (*CurrentOperate)(void);      //当前状态应该执行的功能操作
}StatusTab;

//ip地址
typedef struct
{
	unsigned char ipaddr[4];
	unsigned char netmask[4];
	unsigned char gatway[4];
}ip_addr;


typedef enum {
	TimeShow = 0, //Page 0
	
	//主菜单
	MainMenuSetIp , //Page 1
	//时间显示，设备信息选择页
	MainMenuShowTime,  
	MainMenuShowDevInf,

//选择时间显示页	
	OpShowTimePage1,
	OpShowTimePage2,
	OpShowTimePage3,
	OpShowTimePage4,
	OpShowTimePage5,
	OpShowTimePage6,
	OpShowTimePage7,
	OpShowTimePage8,
//时间显示页	
	ShowTimeOne,    
	ShowTimeTwo,    
	ShowTimeThr,   
	ShowTimeFour,   
	ShowTimeFive,  
	ShowTimeSix,   
	ShowTimeSeven,  
	ShowTimeEight,
//设备信息显示页	
	ShowDevInf,  
	RefreshMeas1,
	RefreshMeas2,
	RefreshMeas3,
	Return0,
	//选择ip设置界面
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

	//IP设置界面
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



//typedef enum {
//	Display_Page_One = 0, //Page 0
//	Display_Page_Two = 1, //Page 1
//	Display_Page_Thr = 2, //Page 2
//	Display_Page_Four= 3, //Page 3
//	Display_Page_Five= 4, //Page 4
//	Display_Page_Six = 6,//查看IP页面
//} OLED_Page; 




void DisDemo(uint8_t Key_Value);//选择界面

void Main_Show(void);
void Demo_OpMenu1(void);
void Demo_OpMenu2(void);
void Demo_OpMenu3(void);

void Demo_OpTime1(void);
void Demo_OpTime2(void);
void Demo_OpTime3(void);
void Demo_OpTime4(void);
void Demo_OpTime5(void);
void Demo_OpTime6(void);
void Demo_OpTime7(void);
void Demo_OpTime8(void);

void Demo_DisTime1(void);
void Demo_DisTime2(void);
void Demo_DisTime3(void);
void Demo_DisTime4(void);
void Demo_DisTime5(void);
void Demo_DisTime6(void);
void Demo_DisTime7(void);
void Demo_DisTime8(void);
void Demo_DisDevInf(void);
void RefreshMeaspage1(void);
void RefreshMeaspage2(void);
void RefreshMeaspage3(void);

void Demo_NET1_IPshow(void);
void Demo_NET2_IPshow(void);
void Demo_NET3_IPshow(void);
void Demo_NET4_IPshow(void);
void Demo_NET5_IPshow(void);
void Demo_NET6_IPshow(void);
void Demo_NET7_IPshow(void);
void Demo_NET8_IPshow(void);
void Demo_NET9_IPshow(void);
void Demo_NET10_IPshow(void);
void Demo_NET11_IPshow(void);
void Demo_NET12_IPshow(void);
void Demo_NET13_IPshow(void);
void Demo_NET14_IPshow(void);
void Demo_NET15_IPshow(void);
void Demo_NET16_IPshow(void);
void DeterminePage1(void);
void DeterminePage2(void);
void DeterminePage3(void);
void SetLocalIpPage(void);
void Demo_IPshow(void);
void Demo_Sent_NET(PSTUsart com,unsigned char Local_IP[3][4],unsigned char Target_IP[3][4],char * str);
void Demo_SAVE_show(void);
void ContainSetOne(void);
void ContainSetTwo(void);
void ContainSetThr(void);
void NET_IP_Set(void);
void DisUpadteCancel(void);
void Set_IPAddr_Page1(void);
void Set_IPAddr_Page2(void);
void Demo_NetOption1(void);
void Demo_NetOption2(void);
void Demo_NetOption3(void);
void Demo_NetOption4(void);
void Demo_NetOption5(void);
void Demo_NetOption6(void);
void Demo_NetOption7(void);
void Demo_NetOption8(void);
void OptionShow1(void);
void OptionShow2(void);
void Returnpage0(void);
void Returnpage(void);
void SaveShow(void);
void CancelShow(void);
void BackShow(void);
#endif

