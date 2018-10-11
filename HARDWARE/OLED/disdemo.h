#ifndef _DisDemo_H
#define _DisDemo_H
#include "sys.h"
#include "oled.h"
#define MENU_SIZE 200          //菜单长度
////////////////////////////////////////////////////////////////////////

extern unsigned char  KeyFuncIndex;    //存放当前的菜单索引

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


void DisDemo(uint8_t Key_Value);//选择界面

#endif

