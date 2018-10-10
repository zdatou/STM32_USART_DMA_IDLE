#include "disdemo.h"
#include "mktime.h"
#include "usart_handle.h"

extern  STUsart usart1;
extern  STUsart usart3;
extern  STUsart usart2;
extern  STUsart usart4;
extern  STUsart usart5;
extern  STUsart usart6;

extern uint8 rev_finish;
uint8 ntp_Ceshi=0;

uint8 saveflag=0;
uint8 returnlocip=0;
uint8 PageFlag = 0;

unsigned char  KeyFuncIndex=0;    //存放当前的菜单索引
void (*KeyFuncPtr)(void);            //定义按键功能指针
void DisUpadte(void);
extern St_Key Key_Proc;
__align(4) unsigned char dtbuf[50];

//状态表（数组+结构体）
StatusTab KeyTab[MENU_SIZE]=
{
	{TimeShow             , MainMenuSetIp       , TimeShow             , TimeShow               , ShowTimeEight        , ShowTimeOne          ,  TimeShow            , (*Main_Show)},   //主界面
	{MainMenuSetIp        , OptNetOnePage       , MainMenuShowDevInf   , MainMenuShowTime       , MainMenuSetIp        , MainMenuSetIp        ,  TimeShow            , (*Demo_OpMenu1)},    				

	{MainMenuShowTime     , OpShowTimePage1     , MainMenuSetIp        , MainMenuShowDevInf     , MainMenuShowTime     , MainMenuShowTime     ,  TimeShow            , (*Demo_OpMenu2)}, 
	{MainMenuShowDevInf   , ShowDevInf          , MainMenuShowTime     , MainMenuSetIp          , MainMenuShowDevInf   , MainMenuShowDevInf   ,  TimeShow            , (*Demo_OpMenu3)}, 
	
	{OpShowTimePage1      , ShowTimeOne         , OpShowTimePage8      , OpShowTimePage2        , OpShowTimePage7      , OpShowTimePage3      ,  MainMenuShowTime    , (*Demo_OpTime1)},
	{OpShowTimePage2      , ShowTimeTwo         , OpShowTimePage1      , OpShowTimePage3        , OpShowTimePage8      , OpShowTimePage4      ,  MainMenuShowTime    , (*Demo_OpTime2)},
	{OpShowTimePage3      , ShowTimeThr         , OpShowTimePage2      , OpShowTimePage4        , OpShowTimePage1      , OpShowTimePage5      ,  MainMenuShowTime    , (*Demo_OpTime3)},
	{OpShowTimePage4      , ShowTimeFour        , OpShowTimePage3      , OpShowTimePage5        , OpShowTimePage2      , OpShowTimePage6      ,  MainMenuShowTime    , (*Demo_OpTime4)},
	{OpShowTimePage5      , ShowTimeFive        , OpShowTimePage4      , OpShowTimePage6        , OpShowTimePage3      , OpShowTimePage7      ,  MainMenuShowTime    , (*Demo_OpTime5)},
	{OpShowTimePage6      , ShowTimeSix         , OpShowTimePage5      , OpShowTimePage7        , OpShowTimePage4      , OpShowTimePage8      ,  MainMenuShowTime    , (*Demo_OpTime6)},
	{OpShowTimePage7      , ShowTimeSeven       , OpShowTimePage6      , OpShowTimePage8        , OpShowTimePage5      , OpShowTimePage1      ,  MainMenuShowTime    , (*Demo_OpTime7)},
	{OpShowTimePage8      , ShowTimeEight       , OpShowTimePage7      , OpShowTimePage1        , OpShowTimePage6      , OpShowTimePage2      ,  MainMenuShowTime    , (*Demo_OpTime8)},
	
	{ShowTimeOne          , RefreshMeas1        , ShowTimeOne          , ShowTimeOne            , ShowTimeEight        , ShowTimeTwo          ,  Return0             , (*Demo_DisTime1)},
	{ShowTimeTwo          , RefreshMeas1        , ShowTimeTwo          , ShowTimeTwo            , ShowTimeOne          , ShowTimeThr          ,  Return0             , (*Demo_DisTime2)},
	{ShowTimeThr          , RefreshMeas1        , ShowTimeThr          , ShowTimeThr            , ShowTimeTwo          , ShowTimeFour         ,  Return0             , (*Demo_DisTime3)},
	{ShowTimeFour         , RefreshMeas1        , ShowTimeFour         , ShowTimeFour           , ShowTimeThr          , ShowTimeFive         ,  Return0             , (*Demo_DisTime4)},
	{ShowTimeFive         , RefreshMeas1        , ShowTimeFive         , ShowTimeFive           , ShowTimeFour         , ShowTimeSix          ,  Return0             , (*Demo_DisTime5)},
	{ShowTimeSix          , RefreshMeas1        , ShowTimeSix          , ShowTimeSix            , ShowTimeFive         , ShowTimeSeven        ,  Return0             , (*Demo_DisTime6)},
	{ShowTimeSeven        , RefreshMeas1        , ShowTimeSeven        , ShowTimeSeven          , ShowTimeSix          , ShowTimeEight        ,  Return0             , (*Demo_DisTime7)},
	{ShowTimeEight        , RefreshMeas1        , ShowTimeEight        , ShowTimeEight          , ShowTimeSeven        , ShowTimeOne          ,  Return0             , (*Demo_DisTime8)},
	{ShowDevInf           , ShowDevInf          , ShowDevInf           , ShowDevInf             , ShowDevInf           , ShowDevInf           ,  MainMenuShowDevInf  , (*Demo_DisDevInf)},   
	{RefreshMeas1         , RefreshMeas3        , RefreshMeas1         , RefreshMeas1           , RefreshMeas2         , RefreshMeas2         ,  Return              , (*RefreshMeaspage1)},
  {RefreshMeas2         , Return              , RefreshMeas2         , RefreshMeas2           , RefreshMeas1         , RefreshMeas1         ,  Return              , (*RefreshMeaspage2)},
	{RefreshMeas3         , RefreshMeas3        , RefreshMeas3         , RefreshMeas3           , RefreshMeas3         , RefreshMeas3         ,  RefreshMeas3        , (*RefreshMeaspage3)},
	{Return0              , Return0             , Return0              , Return0                , Return0              , Return0              ,  Return0             , (*Returnpage0)},
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	
	{OptNetOnePage        , SetNetNightPage     , OptNetEightPage      , OptNetTwoPage          , OptNetSevenPage      , OptNetThrPage        , MainMenuSetIp        ,(*Demo_NetOption1)},
	{OptNetTwoPage        , SetNetTenPage       , OptNetOnePage        , OptNetThrPage          , OptNetEightPage      , OptNetFourPage       , MainMenuSetIp        ,(*Demo_NetOption2)},
	{OptNetThrPage        , SetNetElevenPage    , OptNetTwoPage        , OptNetFourPage         , OptNetOnePage        , OptNetFivePage       , MainMenuSetIp        ,(*Demo_NetOption3)},
	{OptNetFourPage       , SetNetTwelvePage    , OptNetThrPage        , OptNetFivePage         , OptNetTwoPage        , OptNetSixPage        , MainMenuSetIp        ,(*Demo_NetOption4)},
	{OptNetFivePage       , SetNetThirteenPage  , OptNetFourPage       , OptNetSixPage          , OptNetThrPage        , OptNetSevenPage      , MainMenuSetIp        ,(*Demo_NetOption5)},
	{OptNetSixPage        , SetNetFourteenPage  , OptNetFivePage       , OptNetSevenPage        , OptNetFourPage       , OptNetEightPage      , MainMenuSetIp        ,(*Demo_NetOption6)},
	{OptNetSevenPage      , SetNetFifteenPage   , OptNetSixPage        , OptNetEightPage        , OptNetFivePage       , OptNetOnePage        , MainMenuSetIp        ,(*Demo_NetOption7)},
	{OptNetEightPage      , SetNetSixteenPage   , OptNetSevenPage      , OptNetOnePage          , OptNetSixPage        , OptNetTwoPage        , MainMenuSetIp        ,(*Demo_NetOption8)},

	{SaveIpPage           , SaveIpPage          , SaveIpPage           , SaveIpPage             , SaveIpPage           , SaveIpPage           , SaveIpPage           ,(*Demo_SAVE_show)}, 
	{ContainSet1          , OptNetOnePage       , ContainSet1          , ContainSet1            , ContainSet2          , ContainSet2          , MainMenuSetIp        ,(*ContainSetOne)}, 
	{ContainSet2          , ContainSet3         , ContainSet2          , ContainSet2            , ContainSet1          , ContainSet1          , MainMenuSetIp        ,(*ContainSetTwo)},
	{ContainSet3          , ContainSet3         , ContainSet3          , ContainSet3            , ContainSet3          , ContainSet3          , ContainSet3          ,(*ContainSetThr)},
	
	{SetNetOnePage        , OptSavePage         , SetNetOnePage        , SetNetOnePage          , SetNetOnePage        , SetNetOnePage        , NetDetermine3        ,(*Demo_NET1_IPshow)}, 
	{SetNetTwoPage        , OptSavePage         , SetNetTwoPage			   , SetNetTwoPage					, SetNetTwoPage				 , SetNetTwoPage        , NetDetermine3        ,(*Demo_NET2_IPshow)}, 
	{SetNetThrPage        , OptSavePage         , SetNetThrPage		     , SetNetThrPage					, SetNetThrPage				 , SetNetThrPage        , NetDetermine3        ,(*Demo_NET3_IPshow)}, 
	{SetNetFourPage       , OptSavePage         , SetNetFourPage			 , SetNetFourPage					, SetNetFourPage			 , SetNetFourPage				, NetDetermine3        ,(*Demo_NET4_IPshow)}, 
	{SetNetFivePage       , OptSavePage         , SetNetFivePage       , SetNetFivePage         , SetNetFivePage       , SetNetFivePage       , NetDetermine3        ,(*Demo_NET5_IPshow)}, 
	{SetNetSixPage        , OptSavePage         , SetNetSixPage			   , SetNetSixPage					, SetNetSixPage				 , SetNetSixPage        , NetDetermine3        ,(*Demo_NET6_IPshow)}, 
	{SetNetSevenPage      , OptSavePage         , SetNetSevenPage		   , SetNetSevenPage			  , SetNetSevenPage			 , SetNetSevenPage      , NetDetermine3        ,(*Demo_NET7_IPshow)}, 
	{SetNetEightPage      , OptSavePage         , SetNetEightPage			 , SetNetEightPage				, SetNetEightPage			 , SetNetEightPage			, NetDetermine3        ,(*Demo_NET8_IPshow)},
	                                            
	{SetNetNightPage      , NetDetermine1      , SetNetNightPage       , SetNetNightPage        , SetNetNightPage      , SetNetNightPage      , OptNetOnePage        ,(*Demo_NET9_IPshow)}, 
	{SetNetTenPage        , NetDetermine1      , SetNetTenPage			   , SetNetTenPage					, SetNetTenPage				 , SetNetTenPage        , OptNetTwoPage        ,(*Demo_NET10_IPshow)}, 
	{SetNetElevenPage     , NetDetermine1      , SetNetElevenPage		   , SetNetElevenPage				, SetNetElevenPage		 , SetNetElevenPage     , OptNetThrPage        ,(*Demo_NET11_IPshow)}, 
	{SetNetTwelvePage     , NetDetermine1      , SetNetTwelvePage		   , SetNetTwelvePage				, SetNetTwelvePage		 , SetNetTwelvePage			, OptNetFourPage       ,(*Demo_NET12_IPshow)},
	{SetNetThirteenPage   , NetDetermine1      , SetNetThirteenPage    , SetNetThirteenPage     , SetNetThirteenPage   , SetNetThirteenPage   , OptNetFivePage       ,(*Demo_NET13_IPshow)}, 
	{SetNetFourteenPage   , NetDetermine1      , SetNetFourteenPage	   , SetNetFourteenPage			, SetNetFourteenPage	 , SetNetFourteenPage   , OptNetSixPage        ,(*Demo_NET14_IPshow)}, 
	{SetNetFifteenPage    , NetDetermine1      , SetNetFifteenPage		 , SetNetFifteenPage			, SetNetFifteenPage		 , SetNetFifteenPage    , OptNetSevenPage      ,(*Demo_NET15_IPshow)}, 
	{SetNetSixteenPage    , NetDetermine1      , SetNetSixteenPage		 , SetNetSixteenPage			, SetNetSixteenPage		 , SetNetSixteenPage		, OptNetEightPage      ,(*Demo_NET16_IPshow)},
	
	{NetDetermine1        , SaveIpPage          , NetDetermine3  		   , NetDetermine3   		  	, NetDetermine2  		   , NetDetermine2    		, Return               ,(*DeterminePage1)},
	{NetDetermine2        , Return              , NetDetermine3  		   , NetDetermine3   		  	, NetDetermine1  		   , NetDetermine1    		, Return               ,(*DeterminePage2)},
	{NetDetermine3        , SetLocalIp          , NetDetermine1  		   , NetDetermine1   		  	, NetDetermine3  		   , NetDetermine3    		, Return               ,(*DeterminePage3)},
	{SetLocalIp           , SetLocalIp          , SetLocalIp  		     , SetLocalIp   		      , SetLocalIp    		   , SetLocalIp    	    	, SetLocalIp           ,(*SetLocalIpPage)},
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	{OptSavePage				  , SavePage            , OptSavePage          , OptSavePage            , OptCancelPage        , OptCancelPage        , Return               ,(*OptionShow1)},
	{OptCancelPage        , CancelPage          , OptCancelPage        , OptCancelPage          , OptSavePage          , OptSavePage          , Return               ,(*OptionShow2)},
	{Return               , Return              , Return               , Return                 , Return               , Return               , Return               ,(*Returnpage)},
	{CancelPage           , CancelPage          , CancelPage           , CancelPage             , CancelPage           , CancelPage           , CancelPage           ,(*CancelShow)},
	{SavePage             , SavePage            , SavePage             , SavePage               , SavePage             , SavePage             , SavePage             ,(*SaveShow)},
};

const unsigned char Ip_Show[][31]={
	{":%03d.%03d.%03d.%03d"},\
	{":%03d.%03d.%03d.%03d"},\
	{":%03d.%03d.%03d.%03d"}
};//ip 显示

unsigned char One_Ip[3][4]={
{000,000,000,254}/*IP*/,
{255,255,255,000}/*子网掩码*/,
{000,000,000,8}/*网关*/
};

unsigned char Two_Ip[3][4]={
{000,000,000,253}/*IP*/,
{000,000,000,000}/*网关*/,
{000,000,000,8}/*子网掩码*/
};

unsigned char Thr_Ip[3][4]={
{000,000,000,252}/*IP*/,
{255,255,255,000}/*子网掩码*/,
{000,000,000,8}/*子网掩码*/
};

unsigned char Four_Ip[3][4]={
{000,000,000,251}/*IP*/,
{255,255,255,000}/*子网掩码*/,
{000,000,000,8}/*子网掩码*/
};
unsigned char Five_Ip[3][4]={
{000,000,000,250}/*IP*/,
{255,255,255,000}/*子网掩码*/,
{000,000,000,8}/*子网掩码*/
};
unsigned char Six_Ip[3][4]={
{000,000,000,249}/*IP*/,
{255,255,255,000}/*子网掩码*/,
{000,000,000,8}/*子网掩码*/
};
unsigned char Seven_Ip[3][4]={
{000,000,000,248}/*IP*/,
{255,255,255,000}/*子网掩码*/,
{000,000,000,8}/*子网掩码*/
};
unsigned char Eight_Ip[3][4]={
{000,000,000,247}/*IP*/,
{255,255,255,000}/*子网掩码*/,
{000,000,000,8}/*子网掩码*/
};
unsigned char Night_Ip[3][4]={
{255,255,255,255}/*IP*/,
{000,000,000,000}/*网关*/,
{000,000,000,000}/*子网掩码*/
};
unsigned char Ten_Ip[3][4]={
{255,255,255,255}/*IP*/,
{000,000,000,000}/*网关*/,
{000,000,000,000}/*子网掩码*/
};
unsigned char Eleven_Ip[3][4]={
{255,255,255,255}/*IP*/,
{000,000,000,000}/*网关*/,
{000,000,000,000}/*子网掩码*/
};
unsigned char Twelve_Ip[3][4]={
{255,255,255,255}/*IP*/,
{000,000,000,000}/*网关*/,
{000,000,000,000}/*子网掩码*/
};
unsigned char Thirteen_Ip[3][4]={
{255,255,255,255}/*IP*/,
{000,000,000,000}/*网关*/,
{000,000,000,000}/*子网掩码*/
};
unsigned char Fourteen_Ip[3][4]={
{255,255,255,255}/*IP*/,
{000,000,000,000}/*网关*/,
{000,000,000,000}/*子网掩码*/
};
unsigned char Fifteen_Ip[3][4]={
{255,255,255,255}/*IP*/,
{000,000,000,000}/*网关*/,
{000,000,000,000}/*子网掩码*/
};
unsigned char Sixteen_Ip[3][4]={
{255,255,255,255}/*IP*/,
{000,000,000,000}/*网关*/,
{000,000,000,000}/*子网掩码*/
};
/////////////////////////用于缓存本地IP///////////////////////////////////
unsigned char One_Ip_buf[3][4]={
{000,000,000,254}/*IP*/,
{255,255,255,000}/*子网掩码*/,
{000,000,000,8}/*网关*/
};

unsigned char Two_Ip_buf[3][4]={
{000,000,000,253}/*IP*/,
{000,000,000,000}/*网关*/,
{000,000,000,8}/*子网掩码*/
};

unsigned char Thr_Ip_buf[3][4]={
{000,000,000,252}/*IP*/,
{255,255,255,000}/*子网掩码*/,
{000,000,000,8}/*子网掩码*/
};

unsigned char Four_Ip_buf[3][4]={
{000,000,000,251}/*IP*/,
{255,255,255,000}/*子网掩码*/,
{000,000,000,8}/*子网掩码*/
};
unsigned char Five_Ip_buf[3][4]={
{000,000,000,250}/*IP*/,
{255,255,255,000}/*子网掩码*/,
{000,000,000,8}/*子网掩码*/
};
unsigned char Six_Ip_buf[3][4]={
{000,000,000,249}/*IP*/,
{255,255,255,000}/*子网掩码*/,
{000,000,000,8}/*子网掩码*/
};
unsigned char Seven_Ip_buf[3][4]={
{000,000,000,248}/*IP*/,
{255,255,255,000}/*子网掩码*/,
{000,000,000,8}/*子网掩码*/
};
unsigned char Eight_Ip_buf[3][4]={
{000,000,000,247}/*IP*/,
{255,255,255,000}/*子网掩码*/,
{000,000,000,8}/*子网掩码*/
};
unsigned char One_Ip_buf2[3][4]={
{000,000,000,254}/*IP*/,
{255,255,255,000}/*子网掩码*/,
{000,000,000,8}/*网关*/
};

unsigned char Two_Ip_buf2[3][4]={
{000,000,000,253}/*IP*/,
{000,000,000,000}/*网关*/,
{000,000,000,8}/*子网掩码*/
};

unsigned char Thr_Ip_buf2[3][4]={
{000,000,000,252}/*IP*/,
{255,255,255,000}/*子网掩码*/,
{000,000,000,8}/*子网掩码*/
};

unsigned char Four_Ip_buf2[3][4]={
{000,000,000,251}/*IP*/,
{255,255,255,000}/*子网掩码*/,
{000,000,000,8}/*子网掩码*/
};
unsigned char Five_Ip_buf2[3][4]={
{000,000,000,250}/*IP*/,
{255,255,255,000}/*子网掩码*/,
{000,000,000,8}/*子网掩码*/
};
unsigned char Six_Ip_buf2[3][4]={
{000,000,000,249}/*IP*/,
{255,255,255,000}/*子网掩码*/,
{000,000,000,8}/*子网掩码*/
};
unsigned char Seven_Ip_buf2[3][4]={
{000,000,000,248}/*IP*/,
{255,255,255,000}/*子网掩码*/,
{000,000,000,8}/*子网掩码*/
};
unsigned char Eight_Ip_buf2[3][4]={
{000,000,000,247}/*IP*/,
{255,255,255,000}/*子网掩码*/,
{000,000,000,8}/*子网掩码*/
};

//////////////////////////////////////////////////////////////////////////////////////////
uint8 replace=0;
void DisDemo(u8 Key_Value)
{	
	static uint8_t Key_pValue=0;
	static u8 cnt=0;
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
					case SetNetNightPage   :Sent_IpCmd_Request(&usart6,(char *)"1");ntp_Ceshi&=0xfe;break;
					case SetNetTenPage     :Sent_IpCmd_Request(&usart6,(char *)"2");ntp_Ceshi&=0xfd;break;
					case SetNetElevenPage  :Sent_IpCmd_Request(&usart6,(char *)"3");ntp_Ceshi&=0xfb;break;
					case SetNetTwelvePage  :Sent_IpCmd_Request(&usart6,(char *)"4");ntp_Ceshi&=0xf7;break;
					case SetNetThirteenPage:Sent_IpCmd_Request(&usart6,(char *)"5");ntp_Ceshi&=0xef;break;
					case SetNetFourteenPage:Sent_IpCmd_Request(&usart4,(char *)"0");ntp_Ceshi&=0xdf;break;
					case SetNetFifteenPage :Sent_IpCmd_Request(&usart5,(char *)"0");ntp_Ceshi&=0xbf;break;
					case SetNetSixteenPage :Sent_IpCmd_Request(&usart2,(char *)"0");ntp_Ceshi&=0x7f;break;
					                                              
					case OptNetOnePage     :Sent_IpCmd_Request(&usart6,(char *)"1");break;
					case OptNetTwoPage     :Sent_IpCmd_Request(&usart6,(char *)"2");break;
					case OptNetThrPage     :Sent_IpCmd_Request(&usart6,(char *)"3");break;
					case OptNetFourPage    :Sent_IpCmd_Request(&usart6,(char *)"4");break;
					case OptNetFivePage    :Sent_IpCmd_Request(&usart6,(char *)"5");break;
					case OptNetSixPage     :Sent_IpCmd_Request(&usart4,(char *)"0");break;
					case OptNetSevenPage   :Sent_IpCmd_Request(&usart5,(char *)"0");break;
					case OptNetEightPage   :Sent_IpCmd_Request(&usart2,(char *)"0");break;
					//用于手动设置IP,将缓存接收的IP值重新赋予实际IP值,且buf2记录本地IP旧值
					case SetNetOnePage  :  memcpy(One_Ip_buf2  ,One_Ip,    12)  ;memcpy(One_Ip,One_Ip_buf    , 12)  ; break;
					case SetNetTwoPage  :  memcpy(Two_Ip_buf2  ,Two_Ip,    12)  ;memcpy(Two_Ip,Two_Ip_buf    , 12)  ; break;
					case SetNetThrPage  :  memcpy(Thr_Ip_buf2  ,Thr_Ip,    12)  ;memcpy(Thr_Ip,Thr_Ip_buf    , 12)  ; break;
					case SetNetFourPage :  memcpy(Four_Ip_buf2 ,Four_Ip,   12)  ;memcpy(Four_Ip,Four_Ip_buf  , 12)  ; break;
					case SetNetFivePage :  memcpy(Five_Ip_buf2 ,Five_Ip,   12)  ;memcpy(Five_Ip,Five_Ip_buf  , 12)  ; break;
					case SetNetSixPage  :  memcpy(Six_Ip_buf2  ,Six_Ip,    12)  ;memcpy(Six_Ip,Six_Ip_buf    , 12)  ; break;
					case SetNetSevenPage:  memcpy(Seven_Ip_buf2,Seven_Ip,  12)  ;memcpy(Seven_Ip,Seven_Ip_buf, 12)  ; break;
					case SetNetEightPage:  memcpy(Eight_Ip_buf2,Eight_Ip,  12)  ;memcpy(Eight_Ip,Eight_Ip_buf, 12)  ; break;
					default:break;
				}
				if(KeyFuncIndex == NetDetermine3)
				{
					//取消手动设置IP返回页，重新将自动设置的本地IP值赋予实际值
					switch(returnlocip)
					{
						case SetNetOnePage  :;memcpy(One_Ip,One_Ip_buf2    , 12)  ; break;
						case SetNetTwoPage  :;memcpy(Two_Ip,Two_Ip_buf2    , 12)  ; break;
						case SetNetThrPage  :;memcpy(Thr_Ip,Thr_Ip_buf2    , 12)  ; break;
						case SetNetFourPage :;memcpy(Four_Ip,Four_Ip_buf2  , 12)  ; break;
						case SetNetFivePage :;memcpy(Five_Ip,Five_Ip_buf2  , 12)  ; break;
						case SetNetSixPage  :;memcpy(Six_Ip,Six_Ip_buf2    , 12)  ; break;
						case SetNetSevenPage:;memcpy(Seven_Ip,Seven_Ip_buf2, 12)  ; break;
						case SetNetEightPage:;memcpy(Eight_Ip,Eight_Ip_buf2, 12)  ; break;
						default:break;	
					}
				}
		}
		if(KeyFuncIndex == ShowTimeOne||KeyFuncIndex == ShowTimeTwo||KeyFuncIndex == ShowTimeThr||
			 KeyFuncIndex == ShowTimeFour||KeyFuncIndex == ShowTimeFive||KeyFuncIndex == ShowTimeSix||
			 KeyFuncIndex == ShowTimeSeven||KeyFuncIndex == ShowTimeEight)
		{
				ntp_Ceshi=0xff;//开启测试模式			
				Sntp_Sent_Request(&usart2,0x02,2,5);
				Sntp_Sent_Request(&usart4,0x02,2,5);
				Sntp_Sent_Request(&usart5,0x02,2,5);
				com_send_str(&usart6,"Start0",6);//启动从MCU的请求NTP时间
		}
							
	}

	IWDG_Feed();
			 KeyFuncPtr=KeyTab[KeyFuncIndex].CurrentOperate;		//下面是执行按键的操作
		if(KeyFuncPtr==Demo_NET9_IPshow||KeyFuncPtr==Demo_NET10_IPshow||KeyFuncPtr==Demo_NET11_IPshow||KeyFuncPtr==Demo_NET12_IPshow||
			 KeyFuncPtr==Demo_NET13_IPshow||KeyFuncPtr==Demo_NET14_IPshow||KeyFuncPtr==Demo_NET15_IPshow||KeyFuncPtr==Demo_NET16_IPshow
			)
			{
				cnt++;
				if(cnt>=4)
				{
					cnt=0;
					(*KeyFuncPtr)();     //执行当前的按键操作（页面显示）	
				}
			}
			else
			{
				(*KeyFuncPtr)();     //执行当前的按键操作（页面显示）	
			}
	
}

void Main_Show(void)
{
	uint8 date[20]={0};
	uint8 time[20]={0};
	DateTime tmputc;	
	tmputc=SecToTime(System_Time,21);
	PageFlag=KeyFuncIndex;
	sprintf((char*)date,"%04d-%02d-%02d",tmputc.year,tmputc.month,tmputc.day);
	sprintf((char*)time,"%02d:%02d:%02d",tmputc.hour,tmputc.minute,tmputc.second);
	
	Asc8_16(142,0,date);
	Asc20_40(34,24,time);
	Show_MYHZ12_12(38,0,0,"标准时间");
}

void Demo_OpMenu1(void)
{
	Asc8_16(64,20,(u8*)">>");//
	Show_MYHZ12_12(101,0,4,"主菜单");
	Show_MYHZ12_12(80,20,0,"设置通道地址");
	Show_MYHZ12_12(80,36,0,"测量结果显示");//时间
	Show_MYHZ12_12(80,52,0,"设备信息显示");//设备信息
}

void Demo_OpMenu2(void)
{
	Asc8_16(64,36,(u8*)">>");//
	Show_MYHZ12_12(101,0,4,"主菜单");
	Show_MYHZ12_12(80,20,0,"设置通道地址");
	Show_MYHZ12_12(80,36,0,"测量结果显示");//时间
	Show_MYHZ12_12(80,52,0,"设备信息显示");//设备信息
}

void Demo_OpMenu3(void)
{
	Asc8_16(64,52,(u8*)">>");//
	Show_MYHZ12_12(101,0,4,"主菜单");
	Show_MYHZ12_12(80,20,0,"设置通道地址");
	Show_MYHZ12_12(80,36,0,"测量结果显示");//时间
	Show_MYHZ12_12(80,52,0,"设备信息显示");//设备信息
}

void Demo_OpTime1(void)
{
	PageFlag=KeyFuncIndex;
	Show_MYHZ12_12(65,4,0,"选择");//选择
	Asc8_16(100,4,(u8*) "NTP");
	Show_MYHZ12_12(129,4,0,"测量通道");//测量
	Asc8_16(10,25,(u8*)" >>CH1    CH3    CH5    CH7 ");
	Asc8_16(10,45,(u8*)"   CH2    CH4    CH6    CH8 ");
}
void Demo_OpTime2(void)
{
	PageFlag=KeyFuncIndex;
	Show_MYHZ12_12(65,4,0,"选择");//选择
	Asc8_16(100,4,(u8*) "NTP");
	Show_MYHZ12_12(129,4,0,"测量通道");//测量
	Asc8_16(10,25,(u8*)"   CH1    CH3    CH5    CH7 ");
	Asc8_16(10,45,(u8*)" >>CH2    CH4    CH6    CH8 ");
}
void Demo_OpTime3(void)
{
	PageFlag=KeyFuncIndex;
	Show_MYHZ12_12(65,4,0,"选择");//选择
	Asc8_16(100,4,(u8*) "NTP");
	Show_MYHZ12_12(129,4,0,"测量通道");//测量
	Asc8_16(10,25,(u8*)"   CH1  >>CH3    CH5    CH7 ");
	Asc8_16(10,45,(u8*)"   CH2    CH4    CH6    CH8 ");
}
void Demo_OpTime4(void)
{
	PageFlag=KeyFuncIndex;
	Show_MYHZ12_12(65,4,0,"选择");//选择
	Asc8_16(100,4,(u8*) "NTP");
	Show_MYHZ12_12(129,4,0,"测量通道");//测量
	Asc8_16(10,25,(u8*)"   CH1    CH3    CH5    CH7 ");
	Asc8_16(10,45,(u8*)"   CH2  >>CH4    CH6    CH8 ");
}
void Demo_OpTime5(void)
{
	PageFlag=KeyFuncIndex;
	Show_MYHZ12_12(65,4,0,"选择");//选择
	Asc8_16(100,4,(u8*) "NTP");
	Show_MYHZ12_12(129,4,0,"测量通道");//测量
	Asc8_16(10,25,(u8*)"   CH1    CH3  >>CH5    CH7 ");
	Asc8_16(10,45,(u8*)"   CH2    CH4    CH6    CH8 ");
}
void Demo_OpTime6(void)
{
	PageFlag=KeyFuncIndex;
	Show_MYHZ12_12(65,4,0,"选择");//选择
	Asc8_16(100,4,(u8*) "NTP");
	Show_MYHZ12_12(129,4,0,"测量通道");//测量
	Asc8_16(10,25,(u8*)"   CH1    CH3    CH5    CH7 ");
	Asc8_16(10,45,(u8*)"   CH2    CH4  >>CH6    CH8 ");
}
void Demo_OpTime7(void)
{
	PageFlag=KeyFuncIndex;
	Show_MYHZ12_12(65,4,0,"选择");//选择
	Asc8_16(100,4,(u8*) "NTP");
	Show_MYHZ12_12(129,4,0,"测量通道");//测量
	Asc8_16(10,25,(u8*)"   CH1    CH3    CH5  >>CH7 ");
	Asc8_16(10,45,(u8*)"   CH2    CH4    CH6    CH8 ");
}
void Demo_OpTime8(void)
{
	PageFlag=KeyFuncIndex;
	Show_MYHZ12_12(65,4,0,"选择");//选择
	Asc8_16(100,4,(u8*) "NTP");
	Show_MYHZ12_12(129,4,0,"测量通道");//测量
	Asc8_16(10,25,(u8*)"   CH1    CH3    CH5    CH7 ");
	Asc8_16(10,45,(u8*)"   CH2    CH4    CH6  >>CH8 ");
}

void Demo_DisTime1(void)
{
	uint8 tim[50]={0};
	uint8 avcnt[3]={0};
	DateTime tmputc;
	uint8 meas[21]={0};
	uint8 show[21]={0};
	memset(meas,0,21);
	memset(show,0,21);
	memcpy(meas,Sntp.MeasTime[0],21);
	memcpy(show,Sntp.Average_Show[0],21);
	tmputc=SecToTime(System_Time,21);
	
	sprintf((char*)tim,"%04d-%02d-%02d%c%02d:%02d:%02d",\
		tmputc.year,tmputc.month,tmputc.day,' ',tmputc.hour,tmputc.minute,tmputc.second);
	replace=1;
	saveflag=KeyFuncIndex;
	Asc6_12(100,4,tim);
	Asc8_16(20,4,(u8*)"[");
	Show_MYHZ12_12(28,4,0,"通道");//通道
	Asc8_16(60,4,(u8*)"1]");
	Show_MYHZ12_12(20,24,0,"测");//测量
	Show_MYHZ12_12(40,24,0,"量");//测量
	Show_MYHZ12_12(60,24,0,"值");//测量
	Show_MYHZ12_12(20,40,0,"平");//测量
	Show_MYHZ12_12(40,40,0,"均");//测量
	Show_MYHZ12_12(60,40,0,"值");//测量
	Show_MYHZ12_12(76,24,0,"：");//测量
	Show_MYHZ12_12(76,40,0,"：");//平均值:
	
	Asc6_12(44,54,(u8*)"/100");
	Draw_Line(0xff,1,108,187,33,33);
	Draw_Line(0xff,1,196,219,33,33);
	Draw_Line(0xff,1,228,251,33,33);
	Draw_Line(0xff,1,108,187, 53,53);
	Draw_Line(0xff,1,196,219,53,53);
	Draw_Line(0xff,1,228,251,53,53);
	
	Asc6_12(145,54,(u8*)"s");
	Asc6_12(201,54,(u8*)"ms");
	Asc6_12(233,54,(u8*)"us");
	sprintf((char*)avcnt,"%03d",Ave_cnt[0]);
	if(ntp_k[0]>=2&&Sntp.MeasFlag[0] == 1)
	{
		Show_MYHZ12_12(2,4,0,"。");//一个圆点，不是句号
		Asc8_16(84,17,(unsigned char*)meas);
		Asc8_16(84,37,(unsigned char*)show);
	}
	else if(Sntp.MeasFlag[0] == 0)
	{
		Asc8_16(2,0,(u8*)"  ");
	}
	Asc6_12(20,54,avcnt);
}
void Demo_DisTime2(void)
{
	uint8 tim[50]={0};
	uint8 avcnt[3]={0};
	DateTime tmputc;
	uint8 meas[21]={0};
	uint8 show[21]={0};
	memset(meas,0,21);
	memset(show,0,21);
	memcpy(meas,Sntp.MeasTime[1],21);
	memcpy(show,Sntp.Average_Show[1],21);
		
	tmputc=SecToTime(System_Time,21);
	
	sprintf((char*)tim,"%04d-%02d-%02d%c%02d:%02d:%02d",\
		tmputc.year,tmputc.month,tmputc.day,' ',tmputc.hour,tmputc.minute,tmputc.second);
	replace=2;
	saveflag=KeyFuncIndex;
	Asc6_12(100,4,tim);
	Asc8_16(20,4,(u8*)"[");
	Show_MYHZ12_12(28,4,0,"通道");//通道
	Asc8_16(60,4,(u8*)"2]");
	Show_MYHZ12_12(20,24,0,"测");//测量
	Show_MYHZ12_12(40,24,0,"量");//测量
	Show_MYHZ12_12(60,24,0,"值");//测量
	Show_MYHZ12_12(20,40,0,"平");//测量
	Show_MYHZ12_12(40,40,0,"均");//测量
	Show_MYHZ12_12(60,40,0,"值");//测量
	Show_MYHZ12_12(76,24,0,"：");//测量
	Show_MYHZ12_12(76,40,0,"：");//平均值:
	
	Asc6_12(44,54,(u8*)"/100");
	Draw_Line(0xff,1,108,187,33,33);
	Draw_Line(0xff,1,196,219,33,33);
	Draw_Line(0xff,1,228,251,33,33);
	Draw_Line(0xff,1,108,187, 53,53);
	Draw_Line(0xff,1,196,219,53,53);
	Draw_Line(0xff,1,228,251,53,53);
	
	Asc6_12(145,54,(u8*)"s");
	Asc6_12(201,54,(u8*)"ms");
	Asc6_12(233,54,(u8*)"us");
	sprintf((char*)avcnt,"%03d",Ave_cnt[1]);
	if(ntp_k[1]>=2&&Sntp.MeasFlag[1] == 1)
	{
		Show_MYHZ12_12(2,4,0,"。");//一个圆点，不是句号
		Asc8_16(84,17,(unsigned char*)meas);
		Asc8_16(84,37,(unsigned char*)show);
	}
	else if(Sntp.MeasFlag[1] == 0)
	{
		Asc8_16(2,0,(u8*)"  ");
	}
	Asc6_12(20,54,avcnt);
}
void Demo_DisTime3(void)
{
	uint8 tim[50]={0};
	uint8 avcnt[3]={0};
	DateTime tmputc;	
	uint8 meas[21]={0};
	uint8 show[21]={0};
	memset(meas,0,21);
	memset(show,0,21);
	memcpy(meas,Sntp.MeasTime[2],21);
	memcpy(show,Sntp.Average_Show[2],21);
	tmputc=SecToTime(System_Time,21);
	
	sprintf((char*)tim,"%04d-%02d-%02d%c%02d:%02d:%02d",\
		tmputc.year,tmputc.month,tmputc.day,' ',tmputc.hour,tmputc.minute,tmputc.second);
	replace=3;
	saveflag=KeyFuncIndex;
	Asc6_12(100,4,tim);
	Asc8_16(20,4,(u8*)"[");
	Show_MYHZ12_12(28,4,0,"通道");//通道
	Asc8_16(60,4,(u8*)"3]");
	Show_MYHZ12_12(20,24,0,"测");//测量
	Show_MYHZ12_12(40,24,0,"量");//测量
	Show_MYHZ12_12(60,24,0,"值");//测量
	Show_MYHZ12_12(20,40,0,"平");//测量
	Show_MYHZ12_12(40,40,0,"均");//测量
	Show_MYHZ12_12(60,40,0,"值");//测量
	Show_MYHZ12_12(76,24,0,"：");//测量
	Show_MYHZ12_12(76,40,0,"：");//平均值:
	
	Asc6_12(44,54,(u8*)"/100");
	Draw_Line(0xff,1,108,187,33,33);
	Draw_Line(0xff,1,196,219,33,33);
	Draw_Line(0xff,1,228,251,33,33);
	Draw_Line(0xff,1,108,187, 53,53);
	Draw_Line(0xff,1,196,219,53,53);
	Draw_Line(0xff,1,228,251,53,53);
	
	Asc6_12(145,54,(u8*)"s");
	Asc6_12(201,54,(u8*)"ms");
	Asc6_12(233,54,(u8*)"us");
	sprintf((char*)avcnt,"%03d",Ave_cnt[2]);
	if(ntp_k[2]>=2&&Sntp.MeasFlag[2] == 1)
	{
		Show_MYHZ12_12(2,4,0,"。");//一个圆点，不是句号
		Asc8_16(84,17,(unsigned char*)meas);
		Asc8_16(84,37,(unsigned char*)show);
	}
	else if(Sntp.MeasFlag[2] == 0)
	{
		Asc8_16(2,0,(u8*)"  ");
	}
	Asc6_12(20,54,avcnt);
}
void Demo_DisTime4(void)
{
	uint8 tim[50]={0};
	uint8 avcnt[3]={0};
	DateTime tmputc;	
	uint8 meas[21]={0};
	uint8 show[21]={0};
	memset(meas,0,21);
	memset(show,0,21);
	memcpy(meas,Sntp.MeasTime[3],21);
	memcpy(show,Sntp.Average_Show[3],21);
	tmputc=SecToTime(System_Time,21);
	
	sprintf((char*)tim,"%04d-%02d-%02d%c%02d:%02d:%02d",\
		tmputc.year,tmputc.month,tmputc.day,' ',tmputc.hour,tmputc.minute,tmputc.second);
	replace=4;
	saveflag=KeyFuncIndex;
	Asc6_12(100,4,tim);
	Asc8_16(20,4,(u8*)"[");
	Show_MYHZ12_12(28,4,0,"通道");//通道
	Asc8_16(60,4,(u8*)"4]");
	Show_MYHZ12_12(20,24,0,"测");//测量
	Show_MYHZ12_12(40,24,0,"量");//测量
	Show_MYHZ12_12(60,24,0,"值");//测量
	Show_MYHZ12_12(20,40,0,"平");//测量
	Show_MYHZ12_12(40,40,0,"均");//测量
	Show_MYHZ12_12(60,40,0,"值");//测量
	Show_MYHZ12_12(76,24,0,"：");//测量
	Show_MYHZ12_12(76,40,0,"：");//平均值:
	
	Asc6_12(44,54,(u8*)"/100");
	Draw_Line(0xff,1,108,187,33,33);
	Draw_Line(0xff,1,196,219,33,33);
	Draw_Line(0xff,1,228,251,33,33);
	Draw_Line(0xff,1,108,187, 53,53);
	Draw_Line(0xff,1,196,219,53,53);
	Draw_Line(0xff,1,228,251,53,53);
	
	Asc6_12(145,54,(u8*)"s");
	Asc6_12(201,54,(u8*)"ms");
	Asc6_12(233,54,(u8*)"us");
	sprintf((char*)avcnt,"%03d",Ave_cnt[3]);
	if(ntp_k[3]>=2&&Sntp.MeasFlag[3] == 1)
	{
		Show_MYHZ12_12(2,4,0,"。");//一个圆点，不是句号
		Asc8_16(84,17,(unsigned char*)meas);
		Asc8_16(84,37,(unsigned char*)show);
	}
	else if(Sntp.MeasFlag[3] == 0)
	{
		Asc8_16(2,0,(u8*)"  ");
	}
	Asc6_12(20,54,avcnt);
}
void Demo_DisTime5(void)
{
	uint8 tim[50]={0};
	uint8 avcnt[3]={0};
	DateTime tmputc;	
	uint8 meas[21]={0};
	uint8 show[21]={0};
	memset(meas,0,21);
	memset(show,0,21);
	memcpy(meas,Sntp.MeasTime[4],21);
	memcpy(show,Sntp.Average_Show[4],21);
	tmputc=SecToTime(System_Time,21);
	
	sprintf((char*)tim,"%04d-%02d-%02d%c%02d:%02d:%02d",\
		tmputc.year,tmputc.month,tmputc.day,' ',tmputc.hour,tmputc.minute,tmputc.second);
	replace=5;
	saveflag=KeyFuncIndex;

	Asc6_12(100,4,tim);
	Asc8_16(20,4,(u8*)"[");
	Show_MYHZ12_12(28,4,0,"通道");//通道
	Asc8_16(60,4,(u8*)"5]");
	Show_MYHZ12_12(20,24,0,"测");//测量
	Show_MYHZ12_12(40,24,0,"量");//测量
	Show_MYHZ12_12(60,24,0,"值");//测量
	Show_MYHZ12_12(20,40,0,"平");//测量
	Show_MYHZ12_12(40,40,0,"均");//测量
	Show_MYHZ12_12(60,40,0,"值");//测量
	Show_MYHZ12_12(76,24,0,"：");//测量
	Show_MYHZ12_12(76,40,0,"：");//平均值:
	
	Asc6_12(44,54,(u8*)"/100");
	Draw_Line(0xff,1,108,187,33,33);
	Draw_Line(0xff,1,196,219,33,33);
	Draw_Line(0xff,1,228,251,33,33);
	Draw_Line(0xff,1,108,187, 53,53);
	Draw_Line(0xff,1,196,219,53,53);
	Draw_Line(0xff,1,228,251,53,53);
	
	Asc6_12(145,54,(u8*)"s");
	Asc6_12(201,54,(u8*)"ms");
	Asc6_12(233,54,(u8*)"us");
	sprintf((char*)avcnt,"%03d",Ave_cnt[4]);
	if(ntp_k[4]>=2&&Sntp.MeasFlag[4] == 1)
	{
		Show_MYHZ12_12(2,4,0,"。");//一个圆点，不是句号
		Asc8_16(84,17,(unsigned char*)meas);
		Asc8_16(84,37,(unsigned char*)show);
	}
	else if(Sntp.MeasFlag[4] == 0)
	{
		Asc8_16(2,0,(u8*)"  ");
	}
	Asc6_12(20,54,avcnt);
}
void Demo_DisTime6(void)
{
	uint8 tim[50]={0};
	uint8 avcnt[3]={0};
	DateTime tmputc;	
	uint8 meas[21]={0};
	uint8 show[21]={0};
	memset(meas,0,21);
	memset(show,0,21);
	memcpy(meas,Sntp.MeasTime[5],21);
	memcpy(show,Sntp.Average_Show[5],21);
	tmputc=SecToTime(System_Time,21);
	
	sprintf((char*)tim,"%04d-%02d-%02d%c%02d:%02d:%02d",\
		tmputc.year,tmputc.month,tmputc.day,' ',tmputc.hour,tmputc.minute,tmputc.second);
	replace=6;
	saveflag=KeyFuncIndex;

	Asc6_12(100,4,tim);
	Asc8_16(20,4,(u8*)"[");
	Show_MYHZ12_12(28,4,0,"通道");//通道
	Asc8_16(60,4,(u8*)"6]");
	Show_MYHZ12_12(20,24,0,"测");//测量
	Show_MYHZ12_12(40,24,0,"量");//测量
	Show_MYHZ12_12(60,24,0,"值");//测量
	Show_MYHZ12_12(20,40,0,"平");//测量
	Show_MYHZ12_12(40,40,0,"均");//测量
	Show_MYHZ12_12(60,40,0,"值");//测量
	Show_MYHZ12_12(76,24,0,"：");//测量
	Show_MYHZ12_12(76,40,0,"：");//平均值:
	
	Asc6_12(44,54,(u8*)"/100");
	Draw_Line(0xff,1,108,187,33,33);
	Draw_Line(0xff,1,196,219,33,33);
	Draw_Line(0xff,1,228,251,33,33);
	Draw_Line(0xff,1,108,187, 53,53);
	Draw_Line(0xff,1,196,219,53,53);
	Draw_Line(0xff,1,228,251,53,53);
	
	Asc6_12(145,54,(u8*)"s");
	Asc6_12(201,54,(u8*)"ms");
	Asc6_12(233,54,(u8*)"us");
	sprintf((char*)avcnt,"%03d",Ave_cnt[5]);
	if(ntp_k[5]>=2&&Sntp.MeasFlag[5] == 1)
	{
		Show_MYHZ12_12(2,4,0,"。");//一个圆点，不是句号
		Asc8_16(84,17,(unsigned char*)meas);
		Asc8_16(84,37,(unsigned char*)show);
	}
	else if(Sntp.MeasFlag[5] == 0)
	{
		Asc8_16(2,0,(u8*)"  ");
	}
	Asc6_12(20,54,avcnt);
}
void Demo_DisTime7(void)
{
	uint8 tim[50]={0};
	uint8 avcnt[3]={0};
	DateTime tmputc;	
	uint8 meas[21]={0};
	uint8 show[21]={0};
	memset(meas,0,21);
	memset(show,0,21);
	memcpy(meas,Sntp.MeasTime[6],21);
	memcpy(show,Sntp.Average_Show[6],21);
	tmputc=SecToTime(System_Time,21);
	
	sprintf((char*)tim,"%04d-%02d-%02d%c%02d:%02d:%02d",\
		tmputc.year,tmputc.month,tmputc.day,' ',tmputc.hour,tmputc.minute,tmputc.second);
	replace=7;
	saveflag=KeyFuncIndex;
	Asc6_12(100,4,tim);
	Asc8_16(20,4,(u8*)"[");
	Show_MYHZ12_12(28,4,0,"通道");//通道
	Asc8_16(60,4,(u8*)"7]");
	Show_MYHZ12_12(20,24,0,"测");//测量
	Show_MYHZ12_12(40,24,0,"量");//测量
	Show_MYHZ12_12(60,24,0,"值");//测量
	Show_MYHZ12_12(20,40,0,"平");//测量
	Show_MYHZ12_12(40,40,0,"均");//测量
	Show_MYHZ12_12(60,40,0,"值");//测量
	Show_MYHZ12_12(76,24,0,"：");//测量
	Show_MYHZ12_12(76,40,0,"：");//平均值:
	
	Asc6_12(44,54,(u8*)"/100");
	Draw_Line(0xff,1,108,187,33,33);
	Draw_Line(0xff,1,196,219,33,33);
	Draw_Line(0xff,1,228,251,33,33);
	Draw_Line(0xff,1,108,187, 53,53);
	Draw_Line(0xff,1,196,219,53,53);
	Draw_Line(0xff,1,228,251,53,53);
	
	Asc6_12(145,54,(u8*)"s");
	Asc6_12(201,54,(u8*)"ms");
	Asc6_12(233,54,(u8*)"us");
	sprintf((char*)avcnt,"%03d",Ave_cnt[6]);
	if(ntp_k[6]>=2&&Sntp.MeasFlag[6] == 1)
	{
		Show_MYHZ12_12(2,4,0,"。");//一个圆点，不是句号
		Asc8_16(84,17,(unsigned char*)meas);
		Asc8_16(84,37,(unsigned char*)show);
	}
	else if(Sntp.MeasFlag[6] == 0)
	{
		Asc8_16(2,0,(u8*)"  ");
	}	
	Asc6_12(20,54,avcnt);
}
void Demo_DisTime8(void)
{
	uint8 tim[50]={0};
	uint8 avcnt[3]={0};
	DateTime tmputc;	
	uint8 meas[21]={0};
	uint8 show[21]={0};
	memset(meas,0,21);
	memset(show,0,21);
	memcpy(meas,Sntp.MeasTime[7],21);
	memcpy(show,Sntp.Average_Show[7],21);
	tmputc=SecToTime(System_Time,21);
	
	sprintf((char*)tim,"%04d-%02d-%02d%c%02d:%02d:%02d",\
		tmputc.year,tmputc.month,tmputc.day,' ',tmputc.hour,tmputc.minute,tmputc.second);
	replace=8;
	saveflag=KeyFuncIndex;

	Asc6_12(100,4,tim);
	Asc8_16(20,4,(u8*)"[");
	Show_MYHZ12_12(28,4,0,"通道");//通道
	Asc8_16(60,4,(u8*)"8]");
	Show_MYHZ12_12(20,24,0,"测");//测量
	Show_MYHZ12_12(40,24,0,"量");//测量
	Show_MYHZ12_12(60,24,0,"值");//测量
	Show_MYHZ12_12(20,40,0,"平");//测量
	Show_MYHZ12_12(40,40,0,"均");//测量
	Show_MYHZ12_12(60,40,0,"值");//测量
	Show_MYHZ12_12(76,24,0,"：");//测量
	Show_MYHZ12_12(76,40,0,"：");//平均值:
	
	Asc6_12(44,54,(u8*)"/100");
	Draw_Line(0xff,1,108,187,33,33);
	Draw_Line(0xff,1,196,219,33,33);
	Draw_Line(0xff,1,228,251,33,33);
	Draw_Line(0xff,1,108,187, 53,53);
	Draw_Line(0xff,1,196,219,53,53);
	Draw_Line(0xff,1,228,251,53,53);
	
	Asc6_12(145,54,(u8*)"s");
	Asc6_12(201,54,(u8*)"ms");
	Asc6_12(233,54,(u8*)"us");
	sprintf((char*)avcnt,"%03d",Ave_cnt[7]);
	if(ntp_k[7]>=2&&Sntp.MeasFlag[7] == 1)
	{
		Show_MYHZ12_12(2,4,0,"。");//一个圆点，不是句号
		Asc8_16(84,17,(unsigned char*)meas);
		Asc8_16(84,37,(unsigned char*)show);
	}
	else if(Sntp.MeasFlag[7] == 0)
	{
		Asc8_16(2,0,(u8*)"  ");
	}	
	Asc6_12(20,54,avcnt);
}
void Demo_DisDevInf(void)
{
	Show_MYHZ12_12(2,4,0,"设备名称：");
	Show_MYHZ12_12(2,24,0,"设备编号：");
  Show_MYHZ12_12(2,44,0,"出厂日期：");
	Show_MYHZ12_12(82,4,0,"同步网监测设备");
	Asc6_12(82,24,(unsigned char*)"XH032002");
	Asc6_12(82,44,(unsigned char*)"2018-07-25");
}

void RefreshMeaspage1(void)
{
	Show_MYHZ12_12(80,14,0,"是否重新测量");
	Show_MYHZ12_12(96,32,0,"是");
	Show_MYHZ12_12(144,32,0,"否");
	Asc8_16(76,32,(u8*)">>");
}
void RefreshMeaspage2(void)
{
	Show_MYHZ12_12(80,14,0,"是否重新测量");
	Show_MYHZ12_12(96,32,0,"是");
	Show_MYHZ12_12(144,32,0,"否");
	Asc8_16(124,32,(u8*)">>");
}
void Refresh(uint8 i)
{	
	aveval[i]=0;
	n_flag[i]=0;
	memset(&Sntp.Memory[i],0,100);
	Sntp.MeasValue[i]=0;
	Sntp.S_Sum[i]=0;
	Sntp.Us_Sum[i]=0;
	Sntp.S_Average[i]=0;
	Sntp.Us_Average[i]=0;
	Sntp.MeasFlag[i]=0;
	Ave_cnt[i]=0;
	ntp_k[i]=0;
}
void RefreshMeaspage3(void)
{
	Show_MYHZ12_12(40,14,0,"正在重新测量中");
	Asc8_16(154,14,(u8*)".");
	delay_ms(100);
	Show_MYHZ12_12(40,14,0,"正在重新测量中");
	Asc8_16(154,14,(u8*)"..");
	IWDG_Feed();
//	ntp_Ceshi=0;
	switch(replace)
	{
		case 1:Refresh(0);break;
		case 2:Refresh(1);break;
		case 3:Refresh(2);break;
		case 4:Refresh(3);break;
		case 5:Refresh(4);break;
		case 6:Refresh(5);break;
		case 7:Refresh(6);break;
		case 8:Refresh(7);break;
		default:break;
	}
	delay_ms(100);
	delay_ms(100);
	IWDG_Feed();
	Show_MYHZ12_12(40,14,0,"正在重新测量中");
	Asc8_16(154,14,(u8*)"...");
	delay_ms(100);
	delay_ms(100);
	IWDG_Feed();
//	ntp_Ceshi=1;
	KeyFuncIndex = saveflag;
}

void Demo_NetOption1(void)
{
	Show_MYHZ12_12(71,4,0,"选择目标");//选择本地
	Asc8_16(135,4,(u8*)"IP");
	Show_MYHZ12_12(153,4,0,"通道");
	Asc8_16(10,25,(u8*)">>LAN1    LAN3    LAN5    LAN7 ");
	Asc8_16(10,45,(u8*)"  LAN2    LAN4    LAN6    LAN8 ");
	PageFlag = KeyFuncIndex;
}

void Demo_NetOption2(void)
{
	Show_MYHZ12_12(71,4,0,"选择目标");//选择本地
	Asc8_16(135,4,(u8*)"IP");
	Show_MYHZ12_12(153,4,0,"通道");
	Asc8_16(10,25,(u8*)"  LAN1    LAN3    LAN5    LAN7 ");
	Asc8_16(10,45,(u8*)">>LAN2    LAN4    LAN6    LAN8 ");
	PageFlag = KeyFuncIndex;
}

void Demo_NetOption3(void)
{
	Show_MYHZ12_12(71,4,0,"选择目标");//选择本地
	Asc8_16(135,4,(u8*)"IP");
	Show_MYHZ12_12(153,4,0,"通道");
	Asc8_16(10,25,(u8*)"  LAN1  >>LAN3    LAN5    LAN7 ");
	Asc8_16(10,45,(u8*)"  LAN2    LAN4    LAN6    LAN8 ");
	PageFlag = KeyFuncIndex;
}

void Demo_NetOption4(void)
{
	Show_MYHZ12_12(71,4,0,"选择目标");//选择本地
	Asc8_16(135,4,(u8*)"IP");
	Show_MYHZ12_12(153,4,0,"通道");
	Asc8_16(10,25,(u8*)"  LAN1    LAN3    LAN5    LAN7 ");
	Asc8_16(10,45,(u8*)"  LAN2  >>LAN4    LAN6    LAN8 ");
	PageFlag = KeyFuncIndex;
}
void Demo_NetOption5(void)
{
	Show_MYHZ12_12(71,4,0,"选择目标");//选择本地
	Asc8_16(135,4,(u8*)"IP");
	Show_MYHZ12_12(153,4,0,"通道");
	Asc8_16(10,25,(u8*)"  LAN1    LAN3  >>LAN5    LAN7 ");
	Asc8_16(10,45,(u8*)"  LAN2    LAN4    LAN6    LAN8 ");
	PageFlag = KeyFuncIndex;
}
void Demo_NetOption6(void)
{
	Show_MYHZ12_12(71,4,0,"选择目标");//选择本地
	Asc8_16(135,4,(u8*)"IP");
	Show_MYHZ12_12(153,4,0,"通道");
	Asc8_16(10,25,(u8*)"  LAN1    LAN3    LAN5    LAN7 ");
	Asc8_16(10,45,(u8*)"  LAN2    LAN4  >>LAN6    LAN8 ");
	PageFlag = KeyFuncIndex;
}
void Demo_NetOption7(void)
{
	Show_MYHZ12_12(71,4,0,"选择目标");//选择本地
	Asc8_16(135,4,(u8*)"IP");
	Show_MYHZ12_12(153,4,0,"通道");
	Asc8_16(10,25,(u8*)"  LAN1    LAN3    LAN5  >>LAN7 ");
	Asc8_16(10,45,(u8*)"  LAN2    LAN4    LAN6    LAN8 ");
	PageFlag = KeyFuncIndex;
}
void Demo_NetOption8(void)
{
	Show_MYHZ12_12(71,4,0,"选择目标");//选择本地
	Asc8_16(135,4,(u8*)"IP");
	Show_MYHZ12_12(153,4,0,"通道");
	Asc8_16(10,25,(u8*)"  LAN1    LAN3    LAN5    LAN7 ");
	Asc8_16(10,45,(u8*)"  LAN2    LAN4    LAN6  >>LAN8 ");
	PageFlag = KeyFuncIndex;
}


void Demo_NET1_IPshow(void)
{
	returnlocip=KeyFuncIndex;
	Asc5_8(230,5,(u8*)"Net");
	Asc5_8(238,15,(u8*)"1");
	
	Asc8_16(5,5,(u8*)"IP");
	Asc8_16(5,25,(u8*)"IP");
	Asc8_16(5,45,(u8*)"IP");
	Show_MYHZ12_12(25,5,0,"地");//地
	Show_MYHZ12_12(73,5,0,"址");//地
	Show_MYHZ12_12(25,25,0,"子网掩码");//子网掩码
	Show_MYHZ12_12(25,45,0,"网");//网
	Show_MYHZ12_12(73,45,0,"关");//关
	
	sprintf((char *)dtbuf,(char*)Ip_Show[0],One_Ip[0][0],One_Ip[0][1],One_Ip[0][2],One_Ip[0][3]);
	Asc8_16(89,5,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[1],One_Ip[1][0],One_Ip[1][1],One_Ip[1][2],One_Ip[1][3]);
	Asc8_16(89,25,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[2],One_Ip[2][0],One_Ip[2][1],One_Ip[2][2],One_Ip[2][3]);
	Asc8_16(89,45,dtbuf); 
	
	sprintf((char *)dtbuf,(char*)Ip_Show[0],One_Ip[0][0],One_Ip[0][1],One_Ip[0][2],One_Ip[0][3]);
	Asc8_16(89,5,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[1],One_Ip[1][0],One_Ip[1][1],One_Ip[1][2],One_Ip[1][3]);
	Asc8_16(89,25,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[2],One_Ip[2][0],One_Ip[2][1],One_Ip[2][2],One_Ip[2][3]);
	Asc8_16(89,45,dtbuf); 
}

void Demo_NET2_IPshow(void)
{
	returnlocip=KeyFuncIndex;
	Asc5_8(230,5,(u8*)"Net");
	Asc5_8(238,15,(u8*)"2");
	
	Asc8_16(5,5,(u8*)"IP");
	Asc8_16(5,25,(u8*)"IP");
	Asc8_16(5,45,(u8*)"IP");
	Show_MYHZ12_12(25,5,0,"地");//地
	Show_MYHZ12_12(73,5,0,"址");//地
	Show_MYHZ12_12(25,25,0,"子网掩码");//子网掩码
	Show_MYHZ12_12(25,45,0,"网");//网
	Show_MYHZ12_12(73,45,0,"关");//关
	sprintf((char *)dtbuf,(char*)Ip_Show[0],Two_Ip[0][0],Two_Ip[0][1],Two_Ip[0][2],Two_Ip[0][3]);
	Asc8_16(89,5,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[1],Two_Ip[1][0],Two_Ip[1][1],Two_Ip[1][2],Two_Ip[1][3]);
	Asc8_16(89,25,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[2],Two_Ip[2][0],Two_Ip[2][1],Two_Ip[2][2],Two_Ip[2][3]);
	Asc8_16(89,45,dtbuf); 
	
	sprintf((char *)dtbuf,(char*)Ip_Show[0],Two_Ip[0][0],Two_Ip[0][1],Two_Ip[0][2],Two_Ip[0][3]);
	Asc8_16(89,5,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[1],Two_Ip[1][0],Two_Ip[1][1],Two_Ip[1][2],Two_Ip[1][3]);
	Asc8_16(89,25,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[2],Two_Ip[2][0],Two_Ip[2][1],Two_Ip[2][2],Two_Ip[2][3]);
	Asc8_16(89,45,dtbuf); 
}

void Demo_NET3_IPshow(void)
{
	returnlocip=KeyFuncIndex;
	Asc5_8(230,5,(u8*)"Net");
	Asc5_8(238,15,(u8*)"3");
	Asc8_16(5,5,(u8*)"IP");
	Asc8_16(5,25,(u8*)"IP");
	Asc8_16(5,45,(u8*)"IP");
	Show_MYHZ12_12(25,5,0,"地");//地
	Show_MYHZ12_12(73,5,0,"址");//地
	Show_MYHZ12_12(25,25,0,"子网掩码");//子网掩码
	Show_MYHZ12_12(25,45,0,"网");//网
	Show_MYHZ12_12(73,45,0,"关");//关
	sprintf((char *)dtbuf,(char*)Ip_Show[0],Thr_Ip[0][0],Thr_Ip[0][1],Thr_Ip[0][2],Thr_Ip[0][3]);
	Asc8_16(89,5,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[1],Thr_Ip[1][0],Thr_Ip[1][1],Thr_Ip[1][2],Thr_Ip[1][3]);
	Asc8_16(89,25,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[2],Thr_Ip[2][0],Thr_Ip[2][1],Thr_Ip[2][2],Thr_Ip[2][3]);
	Asc8_16(89,45,dtbuf); 
	
	sprintf((char *)dtbuf,(char*)Ip_Show[0],Thr_Ip[0][0],Thr_Ip[0][1],Thr_Ip[0][2],Thr_Ip[0][3]);
	Asc8_16(89,5,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[1],Thr_Ip[1][0],Thr_Ip[1][1],Thr_Ip[1][2],Thr_Ip[1][3]);
	Asc8_16(89,25,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[2],Thr_Ip[2][0],Thr_Ip[2][1],Thr_Ip[2][2],Thr_Ip[2][3]);
	Asc8_16(89,45,dtbuf); 
}

void Demo_NET4_IPshow(void)
{
	returnlocip=KeyFuncIndex;
	Asc5_8(230,5,(u8*)"Net");
	Asc5_8(238,15,(u8*)"4");
	Asc8_16(5,5,(u8*)"IP");
	Asc8_16(5,25,(u8*)"IP");
	Asc8_16(5,45,(u8*)"IP");
	Show_MYHZ12_12(25,5,0,"地");//地
	Show_MYHZ12_12(73,5,0,"址");//地
	Show_MYHZ12_12(25,25,0,"子网掩码");//子网掩码
	Show_MYHZ12_12(25,45,0,"网");//网
	Show_MYHZ12_12(73,45,0,"关");//关
	sprintf((char *)dtbuf,(char*)Ip_Show[0],Four_Ip[0][0],Four_Ip[0][1],Four_Ip[0][2],Four_Ip[0][3]);
	Asc8_16(89,5,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[1],Four_Ip[1][0],Four_Ip[1][1],Four_Ip[1][2],Four_Ip[1][3]);
	Asc8_16(89,25,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[2],Four_Ip[2][0],Four_Ip[2][1],Four_Ip[2][2],Four_Ip[2][3]);
	Asc8_16(89,45,dtbuf); 
	
	sprintf((char *)dtbuf,(char*)Ip_Show[0],Four_Ip[0][0],Four_Ip[0][1],Four_Ip[0][2],Four_Ip[0][3]);
	Asc8_16(89,5,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[1],Four_Ip[1][0],Four_Ip[1][1],Four_Ip[1][2],Four_Ip[1][3]);
	Asc8_16(89,25,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[2],Four_Ip[2][0],Four_Ip[2][1],Four_Ip[2][2],Four_Ip[2][3]);
	Asc8_16(89,45,dtbuf); 
}
void Demo_NET5_IPshow(void)
{
	returnlocip=KeyFuncIndex;
	Asc5_8(230,5,(u8*)"Net");
	Asc5_8(238,15,(u8*)"5");
	Asc8_16(5,5,(u8*)"IP");
	Asc8_16(5,25,(u8*)"IP");
	Asc8_16(5,45,(u8*)"IP");
	Show_MYHZ12_12(25,5,0,"地");//地
	Show_MYHZ12_12(73,5,0,"址");//地
	Show_MYHZ12_12(25,25,0,"子网掩码");//子网掩码
	Show_MYHZ12_12(25,45,0,"网");//网
	Show_MYHZ12_12(73,45,0,"关");//关
	sprintf((char *)dtbuf,(char*)Ip_Show[0],Five_Ip[0][0],Five_Ip[0][1],Five_Ip[0][2],Five_Ip[0][3]);
	Asc8_16(89,5,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[1],Five_Ip[1][0],Five_Ip[1][1],Five_Ip[1][2],Five_Ip[1][3]);
	Asc8_16(89,25,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[2],Five_Ip[2][0],Five_Ip[2][1],Five_Ip[2][2],Five_Ip[2][3]);
	Asc8_16(89,45,dtbuf); 
	
	sprintf((char *)dtbuf,(char*)Ip_Show[0],Five_Ip[0][0],Five_Ip[0][1],Five_Ip[0][2],Five_Ip[0][3]);
	Asc8_16(89,5,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[1],Five_Ip[1][0],Five_Ip[1][1],Five_Ip[1][2],Five_Ip[1][3]);
	Asc8_16(89,25,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[2],Five_Ip[2][0],Five_Ip[2][1],Five_Ip[2][2],Five_Ip[2][3]);
	Asc8_16(89,45,dtbuf); 
}
void Demo_NET6_IPshow(void)
{
	returnlocip=KeyFuncIndex;
	Asc5_8(230,5,(u8*)"Net");
	Asc5_8(238,15,(u8*)"6");
	Asc8_16(5,5,(u8*)"IP");
	Asc8_16(5,25,(u8*)"IP");
	Asc8_16(5,45,(u8*)"IP");
	Show_MYHZ12_12(25,5,0,"地");//地
	Show_MYHZ12_12(73,5,0,"址");//地
	Show_MYHZ12_12(25,25,0,"子网掩码");//子网掩码
	Show_MYHZ12_12(25,45,0,"网");//网
	Show_MYHZ12_12(73,45,0,"关");//关
	sprintf((char *)dtbuf,(char*)Ip_Show[0],Six_Ip[0][0],Six_Ip[0][1],Six_Ip[0][2],Six_Ip[0][3]);
	Asc8_16(89,5,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[1],Six_Ip[1][0],Six_Ip[1][1],Six_Ip[1][2],Six_Ip[1][3]);
	Asc8_16(89,25,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[2],Six_Ip[2][0],Six_Ip[2][1],Six_Ip[2][2],Six_Ip[2][3]);
	Asc8_16(89,45,dtbuf); 
	
	sprintf((char *)dtbuf,(char*)Ip_Show[0],Six_Ip[0][0],Six_Ip[0][1],Six_Ip[0][2],Six_Ip[0][3]);
	Asc8_16(89,5,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[1],Six_Ip[1][0],Six_Ip[1][1],Six_Ip[1][2],Six_Ip[1][3]);
	Asc8_16(89,25,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[2],Six_Ip[2][0],Six_Ip[2][1],Six_Ip[2][2],Six_Ip[2][3]);
	Asc8_16(89,45,dtbuf); 
}
void Demo_NET7_IPshow(void)
{
	returnlocip=KeyFuncIndex;
	Asc5_8(230,5,(u8*)"Net");
	Asc5_8(238,15,(u8*)"7");
	Asc8_16(5,5,(u8*)"IP");
	Asc8_16(5,25,(u8*)"IP");
	Asc8_16(5,45,(u8*)"IP");
	Show_MYHZ12_12(25,5,0,"地");//地
	Show_MYHZ12_12(73,5,0,"址");//地
	Show_MYHZ12_12(25,25,0,"子网掩码");//子网掩码
	Show_MYHZ12_12(25,45,0,"网");//网
	Show_MYHZ12_12(73,45,0,"关");//关
	sprintf((char *)dtbuf,(char*)Ip_Show[0],Seven_Ip[0][0],Seven_Ip[0][1],Seven_Ip[0][2],Seven_Ip[0][3]);
	Asc8_16(89,5,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[1],Seven_Ip[1][0],Seven_Ip[1][1],Seven_Ip[1][2],Seven_Ip[1][3]);
	Asc8_16(89,25,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[2],Seven_Ip[2][0],Seven_Ip[2][1],Seven_Ip[2][2],Seven_Ip[2][3]);
	Asc8_16(89,45,dtbuf); 
	
	sprintf((char *)dtbuf,(char*)Ip_Show[0],Seven_Ip[0][0],Seven_Ip[0][1],Seven_Ip[0][2],Seven_Ip[0][3]);
	Asc8_16(89,5,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[1],Seven_Ip[1][0],Seven_Ip[1][1],Seven_Ip[1][2],Seven_Ip[1][3]);
	Asc8_16(89,25,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[2],Seven_Ip[2][0],Seven_Ip[2][1],Seven_Ip[2][2],Seven_Ip[2][3]);
	Asc8_16(89,45,dtbuf); 
}
void Demo_NET8_IPshow(void)
{
	returnlocip=KeyFuncIndex;
	Asc5_8(230,5,(u8*)"Net");
	Asc5_8(238,15,(u8*)"8");
	Asc8_16(5,5,(u8*)"IP");
	Asc8_16(5,25,(u8*)"IP");
	Asc8_16(5,45,(u8*)"IP");
	Show_MYHZ12_12(25,5,0,"地");//地
	Show_MYHZ12_12(73,5,0,"址");//地
	Show_MYHZ12_12(25,25,0,"子网掩码");//子网掩码
	Show_MYHZ12_12(25,45,0,"网");//网
	Show_MYHZ12_12(73,45,0,"关");//关
	sprintf((char *)dtbuf,(char*)Ip_Show[0],Eight_Ip[0][0],Eight_Ip[0][1],Eight_Ip[0][2],Eight_Ip[0][3]);
	Asc8_16(89,5,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[1],Eight_Ip[1][0],Eight_Ip[1][1],Eight_Ip[1][2],Eight_Ip[1][3]);
	Asc8_16(89,25,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[2],Eight_Ip[2][0],Eight_Ip[2][1],Eight_Ip[2][2],Eight_Ip[2][3]);
	Asc8_16(89,45,dtbuf); 
	
	sprintf((char *)dtbuf,(char*)Ip_Show[0],Eight_Ip[0][0],Eight_Ip[0][1],Eight_Ip[0][2],Eight_Ip[0][3]);
	Asc8_16(89,5,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[1],Eight_Ip[1][0],Eight_Ip[1][1],Eight_Ip[1][2],Eight_Ip[1][3]);
	Asc8_16(89,25,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[2],Eight_Ip[2][0],Eight_Ip[2][1],Eight_Ip[2][2],Eight_Ip[2][3]);
	Asc8_16(89,45,dtbuf); 
}
void Demo_NET9_IPshow(void)
{
	saveflag=KeyFuncIndex;
	Asc5_8(230,5,(u8*)"Net");
	Asc5_8(238,15,(u8*)"1");
	Asc8_16(5,5,(u8*)"IP");
	Asc8_16(5,25,(u8*)"IP");
	Asc8_16(5,45,(u8*)"IP");
	Show_MYHZ12_12(25,5,0,"地");//地
	Show_MYHZ12_12(73,5,0,"址");//地
	Show_MYHZ12_12(25,25,0,"子网掩码");//子网掩码
	Show_MYHZ12_12(25,45,0,"网");//网
	Show_MYHZ12_12(73,45,0,"关");//关
	sprintf((char *)dtbuf,(char*)Ip_Show[0],Night_Ip[0][0],Night_Ip[0][1],Night_Ip[0][2],Night_Ip[0][3]);
	Asc8_16(89,5,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[1],Night_Ip[1][0],Night_Ip[1][1],Night_Ip[1][2],Night_Ip[1][3]);
	Asc8_16(89,25,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[2],Night_Ip[2][0],Night_Ip[2][1],Night_Ip[2][2],Night_Ip[2][3]);
	Asc8_16(89,45,dtbuf); 
}
void Demo_NET10_IPshow(void)
{
	saveflag=KeyFuncIndex;
	Asc5_8(230,5,(u8*)"Net");
	Asc5_8(238,15,(u8*)"2");
	Asc8_16(5,5,(u8*)"IP");
	Asc8_16(5,25,(u8*)"IP");
	Asc8_16(5,45,(u8*)"IP");
	Show_MYHZ12_12(25,5,0,"地");//地
	Show_MYHZ12_12(73,5,0,"址");//地
	Show_MYHZ12_12(25,25,0,"子网掩码");//子网掩码
	Show_MYHZ12_12(25,45,0,"网");//网
	Show_MYHZ12_12(73,45,0,"关");//关
	sprintf((char *)dtbuf,(char*)Ip_Show[0],Ten_Ip[0][0],Ten_Ip[0][1],Ten_Ip[0][2],Ten_Ip[0][3]);
	Asc8_16(89,5,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[1],Ten_Ip[1][0],Ten_Ip[1][1],Ten_Ip[1][2],Ten_Ip[1][3]);
	Asc8_16(89,25,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[2],Ten_Ip[2][0],Ten_Ip[2][1],Ten_Ip[2][2],Ten_Ip[2][3]);
	Asc8_16(89,45,dtbuf); 
}
void Demo_NET11_IPshow(void)
{
	saveflag=KeyFuncIndex;
	Asc5_8(230,5,(u8*)"Net");
	Asc5_8(238,15,(u8*)"3");
	Asc8_16(5,5,(u8*)"IP");
	Asc8_16(5,25,(u8*)"IP");
	Asc8_16(5,45,(u8*)"IP");
	Show_MYHZ12_12(25,5,0,"地");//地
	Show_MYHZ12_12(73,5,0,"址");//地
	Show_MYHZ12_12(25,25,0,"子网掩码");//子网掩码
	Show_MYHZ12_12(25,45,0,"网");//网
	Show_MYHZ12_12(73,45,0,"关");//关
	sprintf((char *)dtbuf,(char*)Ip_Show[0],Eleven_Ip[0][0],Eleven_Ip[0][1],Eleven_Ip[0][2],Eleven_Ip[0][3]);
	Asc8_16(89,5,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[1],Eleven_Ip[1][0],Eleven_Ip[1][1],Eleven_Ip[1][2],Eleven_Ip[1][3]);
	Asc8_16(89,25,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[2],Eleven_Ip[2][0],Eleven_Ip[2][1],Eleven_Ip[2][2],Eleven_Ip[2][3]);
	Asc8_16(89,45,dtbuf); 
}
void Demo_NET12_IPshow(void)
{
	saveflag=KeyFuncIndex;
	Asc5_8(230,5,(u8*)"Net");
	Asc5_8(238,15,(u8*)"4");
	Asc8_16(5,5,(u8*)"IP");
	Asc8_16(5,25,(u8*)"IP");
	Asc8_16(5,45,(u8*)"IP");
	Show_MYHZ12_12(25,5,0,"地");//地
	Show_MYHZ12_12(73,5,0,"址");//地
	Show_MYHZ12_12(25,25,0,"子网掩码");//子网掩码
	Show_MYHZ12_12(25,45,0,"网");//网
	Show_MYHZ12_12(73,45,0,"关");//关
	sprintf((char *)dtbuf,(char*)Ip_Show[0],Twelve_Ip[0][0],Twelve_Ip[0][1],Twelve_Ip[0][2],Twelve_Ip[0][3]);
	Asc8_16(89,5,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[1],Twelve_Ip[1][0],Twelve_Ip[1][1],Twelve_Ip[1][2],Twelve_Ip[1][3]);
	Asc8_16(89,25,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[2],Twelve_Ip[2][0],Twelve_Ip[2][1],Twelve_Ip[2][2],Twelve_Ip[2][3]);
	Asc8_16(89,45,dtbuf); 
}
void Demo_NET13_IPshow(void)
{
	saveflag=KeyFuncIndex;
	Asc5_8(230,5,(u8*)"Net");
	Asc5_8(238,15,(u8*)"5");
	Asc8_16(5,5,(u8*)"IP");
	Asc8_16(5,25,(u8*)"IP");
	Asc8_16(5,45,(u8*)"IP");
	Show_MYHZ12_12(25,5,0,"地");//地
	Show_MYHZ12_12(73,5,0,"址");//地
	Show_MYHZ12_12(25,25,0,"子网掩码");//子网掩码
	Show_MYHZ12_12(25,45,0,"网");//网
	Show_MYHZ12_12(73,45,0,"关");//关
	sprintf((char *)dtbuf,(char*)Ip_Show[0],Thirteen_Ip[0][0],Thirteen_Ip[0][1],Thirteen_Ip[0][2],Thirteen_Ip[0][3]);
	Asc8_16(89,5,dtbuf);                                                                           
  sprintf((char *)dtbuf,(char*)Ip_Show[1],Thirteen_Ip[1][0],Thirteen_Ip[1][1],Thirteen_Ip[1][2],Thirteen_Ip[1][3]);
	Asc8_16(89,25,dtbuf);                                                                          
  sprintf((char *)dtbuf,(char*)Ip_Show[2],Thirteen_Ip[2][0],Thirteen_Ip[2][1],Thirteen_Ip[2][2],Thirteen_Ip[2][3]);
	Asc8_16(89,45,dtbuf); 
}
void Demo_NET14_IPshow(void)
{
	saveflag=KeyFuncIndex;
	Asc5_8(230,5,(u8*)"Net");
	Asc5_8(238,15,(u8*)"6");
	Asc8_16(5,5,(u8*)"IP");
	Asc8_16(5,25,(u8*)"IP");
	Asc8_16(5,45,(u8*)"IP");
	Show_MYHZ12_12(25,5,0,"地");//地
	Show_MYHZ12_12(73,5,0,"址");//地
	Show_MYHZ12_12(25,25,0,"子网掩码");//子网掩码
	Show_MYHZ12_12(25,45,0,"网");//网
	Show_MYHZ12_12(73,45,0,"关");//关
	sprintf((char *)dtbuf,(char*)Ip_Show[0],Fourteen_Ip[0][0],Fourteen_Ip[0][1],Fourteen_Ip[0][2],Fourteen_Ip[0][3]);
	Asc8_16(89,5,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[1],Fourteen_Ip[1][0],Fourteen_Ip[1][1],Fourteen_Ip[1][2],Fourteen_Ip[1][3]);
	Asc8_16(89,25,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[2],Fourteen_Ip[2][0],Fourteen_Ip[2][1],Fourteen_Ip[2][2],Fourteen_Ip[2][3]);
	Asc8_16(89,45,dtbuf); 
}
void Demo_NET15_IPshow(void)
{
	saveflag=KeyFuncIndex;
	Asc5_8(230,5,(u8*)"Net");
	Asc5_8(238,15,(u8*)"7");
	Asc8_16(5,5,(u8*)"IP");
	Asc8_16(5,25,(u8*)"IP");
	Asc8_16(5,45,(u8*)"IP");
	Show_MYHZ12_12(25,5,0,"地");//地
	Show_MYHZ12_12(73,5,0,"址");//地
	Show_MYHZ12_12(25,25,0,"子网掩码");//子网掩码
	Show_MYHZ12_12(25,45,0,"网");//网
	Show_MYHZ12_12(73,45,0,"关");//关
	sprintf((char *)dtbuf,(char*)Ip_Show[0],Fifteen_Ip[0][0],Fifteen_Ip[0][1],Fifteen_Ip[0][2],Fifteen_Ip[0][3]);
	Asc8_16(89,5,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[1],Fifteen_Ip[1][0],Fifteen_Ip[1][1],Fifteen_Ip[1][2],Fifteen_Ip[1][3]);
	Asc8_16(89,25,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[2],Fifteen_Ip[2][0],Fifteen_Ip[2][1],Fifteen_Ip[2][2],Fifteen_Ip[2][3]);
	Asc8_16(89,45,dtbuf); 
}
void Demo_NET16_IPshow(void)
{
	saveflag=KeyFuncIndex;
	Asc5_8(230,5,(u8*)"Net");
	Asc5_8(238,15,(u8*)"8");
	Asc8_16(5,5,(u8*)"IP");
	Asc8_16(5,25,(u8*)"IP");
	Asc8_16(5,45,(u8*)"IP");
	Show_MYHZ12_12(25,5,0,"地");//地
	Show_MYHZ12_12(73,5,0,"址");//地
	Show_MYHZ12_12(25,25,0,"子网掩码");//子网掩码
	Show_MYHZ12_12(25,45,0,"网");//网
	Show_MYHZ12_12(73,45,0,"关");//关
	sprintf((char *)dtbuf,(char*)Ip_Show[0],Sixteen_Ip[0][0],Sixteen_Ip[0][1],Sixteen_Ip[0][2],Sixteen_Ip[0][3]);
	Asc8_16(89,5,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[1],Sixteen_Ip[1][0],Sixteen_Ip[1][1],Sixteen_Ip[1][2],Sixteen_Ip[1][3]);
	Asc8_16(89,25,dtbuf);
  sprintf((char *)dtbuf,(char*)Ip_Show[2],Sixteen_Ip[2][0],Sixteen_Ip[2][1],Sixteen_Ip[2][2],Sixteen_Ip[2][3]);
	Asc8_16(89,45,dtbuf); 
}

void DeterminePage1(void)
{
	Asc8_16(55,16,(u8*)">> ");
	Show_MYHZ12_12(79,16,0,"确定"); //保存
	Show_MYHZ12_12(155,16,0,"返回");//取消
	
	Show_MYHZ12_12(79,36,0,"手动设置本地");
	Asc8_16(175,36,(u8*)"IP");
	
}
void DeterminePage2(void)
{
	Asc8_16(131,16,(u8*)">> ");
  Show_MYHZ12_12(79,16,0,"确定"); //保存
	Show_MYHZ12_12(155,16,0,"返回");//取消
	
	Show_MYHZ12_12(79,36,0,"手动设置本地");
	Asc8_16(175,36,(u8*)"IP");
}
void DeterminePage3(void)
{
	Asc8_16(55,36,(u8*)">> ");
	Show_MYHZ12_12(79,16,0,"确定"); //保存
	Show_MYHZ12_12(155,16,0,"返回");//取消
	
	Show_MYHZ12_12(79,36,0,"手动设置本地");
	Asc8_16(175,36,(u8*)"IP");
	
	
}

void SetLocalIpPage(void)
{
	switch(saveflag)
	{
		case SetNetNightPage    :KeyFuncIndex=SetNetOnePage  ;break;
		case SetNetTenPage      :KeyFuncIndex=SetNetTwoPage  ;break;
		case SetNetElevenPage   :KeyFuncIndex=SetNetThrPage  ;break;
		case SetNetTwelvePage   :KeyFuncIndex=SetNetFourPage ;break;
		case SetNetThirteenPage :KeyFuncIndex=SetNetFivePage ;break;
		case SetNetFourteenPage :KeyFuncIndex=SetNetSixPage  ;break;
		case SetNetFifteenPage  :KeyFuncIndex=SetNetSevenPage;break;
		case SetNetSixteenPage  :KeyFuncIndex=SetNetEightPage;break;
		default:break;			
	}
}


void Demo_Sent_NET(PSTUsart com,unsigned char Local_IP[3][4],unsigned char Target_IP[3][4],char * str)
{
	char p[30]={0};
	uint8 i,j,k=0;
	uint8 check = 0;
	delay_ms(100);
	p[0]=0x5A;
	p[1]=0x01;
	p[2]=0x1C;
	for(i=0;i<3;i++)
	{
		for(j=0;j<4;j++)
		{
			k++;
			p[2+k]=Local_IP[i][j];
			p[14+k]=Target_IP[i][j];
		}
	}
	for(i=0;i<27;i++)
	{
		check^=p[i];
	}
	p[27]=check;
	//strlen(p)会把p当成字符串处理，0x00在字符串中是结束标志，在数组中就是0
	if(strcmp(str,"0")==0)
	com_send_str(com,p,28);//发送IP修改指令
	else 
	{
		p[28]=*str;
		com_send_str(com,p,29);//发送IP修改指令
	}
}
void Demo_SAVE_show(void)
{
//	BEEP = 0;
	Asc6_12(50,20,(u8*)"IP");
	Show_MYHZ12_12(70,20,0,"数据正在保存"); //数据正在保存
	Asc6_12(160,20,(u8*)".");
	switch(PageFlag)
	{
		case OptNetOnePage    :Demo_Sent_NET(&usart6,One_Ip  ,Night_Ip   ,(char *)"1");break;
		case OptNetTwoPage    :Demo_Sent_NET(&usart6,Two_Ip  ,Ten_Ip     ,(char *)"2");break;
		case OptNetThrPage    :Demo_Sent_NET(&usart6,Thr_Ip  ,Eleven_Ip  ,(char *)"3");break;
		case OptNetFourPage   :Demo_Sent_NET(&usart6,Four_Ip ,Twelve_Ip  ,(char *)"4");break;
		case OptNetFivePage   :Demo_Sent_NET(&usart6,Five_Ip ,Thirteen_Ip,(char *)"5");break;
		case OptNetSixPage    :Demo_Sent_NET(&usart4,Six_Ip  ,Fourteen_Ip,(char *)"0");break;
		case OptNetSevenPage  :Demo_Sent_NET(&usart5,Seven_Ip,Fifteen_Ip ,(char *)"0");break;
		case OptNetEightPage  :Demo_Sent_NET(&usart2,Eight_Ip,Sixteen_Ip ,(char *)"0");break;
		default:break;
	}
	IWDG_Feed();
	Asc6_12(160,20,(u8*)"..");
	delay_ms(100);
	
	delay_ms(200);
	IWDG_Feed();
	Asc6_12(160,20,(u8*)"...");
	delay_ms(200);
	delay_ms(200);
	IWDG_Feed();
	KeyFuncIndex = ContainSet1; //回上一级菜单
}

void ContainSetOne(void)
{
	Show_MYHZ12_12(10,8,0,"数据保存成功");
	Asc8_16(106,8,(u8*)"!");
	Show_MYHZ12_12(118,8,0,"是否继续设置");
	Asc8_16(214,8,(u8*)"IP");
	Asc8_16(234,8,(u8*)"?");
	Asc8_16(70,32,(u8*)">>");
	Show_MYHZ12_12(94,32,0,"是");
	Show_MYHZ12_12(142,32,0,"否");
}
void ContainSetTwo(void)
{
	Show_MYHZ12_12(10,8,0,"数据保存成功");
	Asc8_16(106,8,(u8*)"!");
	Show_MYHZ12_12(118,8,0,"是否继续设置");
	Asc8_16(214,8,(u8*)"IP");
	Asc8_16(234,8,(u8*)"?");
	Asc8_16(118,32,(u8*)">>");
	Show_MYHZ12_12(94,32,0,"是");
	Show_MYHZ12_12(142,32,0,"否");
}
void ContainSetThr(void)
{
		switch(PageFlag)
		{
			case OptNetOnePage     :KeyFuncIndex = ShowTimeOne;  break;//必须从TOD处理函数开启
			case OptNetTwoPage     :KeyFuncIndex = ShowTimeTwo;  break;
			case OptNetThrPage     :KeyFuncIndex = ShowTimeThr;  break;
			case OptNetFourPage    :KeyFuncIndex = ShowTimeFour; break;
			case OptNetFivePage    :KeyFuncIndex = ShowTimeFive; break;
			case OptNetSixPage     :KeyFuncIndex = ShowTimeSix;  break;
			case OptNetSevenPage   :KeyFuncIndex = ShowTimeSeven;break;
			case OptNetEightPage   :KeyFuncIndex = ShowTimeEight;break;
		}
}

void OptionShow1(void)
{
	Asc8_16(55,25,(u8*)">> ");
	Show_MYHZ12_12(79,25,0,"保存"); //保存
	Show_MYHZ12_12(155,25,0,"取消");//取消
}
void OptionShow2(void)
{
	Asc8_16(131,25,(u8*)">> ");
	Show_MYHZ12_12(79,25,0,"保存"); //保存
	Show_MYHZ12_12(155,25,0,"取消");//取消
}

void Returnpage0(void)
{
	if(PageFlag==TimeShow||PageFlag==OpShowTimePage1||PageFlag==OpShowTimePage2||PageFlag==OpShowTimePage3||PageFlag==OpShowTimePage4||
		PageFlag==OpShowTimePage5||PageFlag==OpShowTimePage6||PageFlag==OpShowTimePage7||PageFlag==OpShowTimePage8)
	{
		KeyFuncIndex = PageFlag;
	}
	else KeyFuncIndex=TimeShow;
}

void Returnpage(void)
{
		
	if( saveflag == SetNetNightPage||saveflag == SetNetTenPage||saveflag == SetNetElevenPage||
			saveflag == SetNetTwelvePage||saveflag == SetNetThirteenPage||saveflag == SetNetFourteenPage||
		 saveflag == SetNetFifteenPage||saveflag == SetNetSixteenPage
			)
	{
				KeyFuncIndex = saveflag;//返回相应的IP设置页面
	}
	else if(saveflag == ShowTimeOne||saveflag == ShowTimeTwo||saveflag == ShowTimeThr||
				 saveflag == ShowTimeFour||saveflag == ShowTimeFive||saveflag == ShowTimeSix||			
			   saveflag == ShowTimeSeven||saveflag == ShowTimeEight
			)//界面跳到设置IP界面时读取IP数值		
	{
				KeyFuncIndex = saveflag;//返回相应的IP设置页面
	}
	else if( returnlocip == SetNetOnePage||returnlocip == SetNetTwoPage||returnlocip == SetNetThrPage||
			returnlocip == SetNetFourPage||returnlocip == SetNetFivePage||returnlocip == SetNetSixPage||
		 returnlocip == SetNetSevenPage||returnlocip == SetNetEightPage
			)
	{
				KeyFuncIndex = returnlocip;//跳到IP保存函数
	}
}

void SaveShow(void)
{
	if( PageFlag == OptNetOnePage || PageFlag == OptNetTwoPage || PageFlag == OptNetThrPage  || PageFlag == OptNetFourPage || 
		  PageFlag == OptNetFivePage || PageFlag == OptNetSixPage || PageFlag == OptNetSevenPage  || PageFlag == OptNetEightPage
			)
	{
				KeyFuncIndex = SaveIpPage;//跳到IP保存函数
	}
}

void CancelShow(void)
{
	if( PageFlag == OptNetOnePage || PageFlag == OptNetTwoPage || PageFlag == OptNetThrPage  || PageFlag == OptNetFourPage || 
		  PageFlag == OptNetFivePage || PageFlag == OptNetSixPage || PageFlag == OptNetSevenPage  || PageFlag == OptNetEightPage 
	    )
	{
				IWDG_Feed();
				KeyFuncIndex = PageFlag;
	}
}


u8 Raw = 0 , Colum = 0; //行列
void NET_IP_Option(unsigned char IP[3][4],unsigned char buf[3][4],uint8 fg,uint8 wg)
{
	u8 Value = 0;
	
		if(Key_Proc.Key_Tri==Key_S4)
		{
			DisUpadteCancel();//消影
			Colum++;
			if(Colum > 3){Colum = 0;Raw++;}
			if(Raw > 2){Raw = 0;}	
		}
		 else if(Key_Proc.Key_Tri==Key_S3)
		{
			DisUpadteCancel();//消影
			 Colum--;
			if( Colum > 250){ Colum = 3;Raw--;}
			if( Raw > 250 ){ Raw = 2;}
		}
		 else if(Key_Proc.Key_Tri==Key_S0)//短按键 加
		{
			Value=IP[Raw][Colum];
			Value++;
			IP[Raw][Colum]=Value;
			Key_Proc.Key_Count = 0;
		}
		else if(Key_Proc.Key_Cnt==Key_S0)//长按键 加
		{
			Key_Proc.Key_Count++;
			if(Key_Proc.Key_Count>Key_Timer)
			{
				Key_Proc.Key_Flag=1;
			}
			else if((Key_Proc.Key_Count % 10 == 0) && (Key_Proc.Key_Flag != 1))//按下200ms 开始步进
			{
					Value=IP[Raw][Colum];
					Value++;
					IP[Raw][Colum]=Value;
			}
		} 
			else if(Key_Proc.Key_Tri==Key_S1)//短按键  减
		{
			Value = IP[Raw][Colum];
			Value--;
			IP[Raw][Colum]=Value;
			Key_Proc.Key_Count = 0;
		}
		else if(Key_Proc.Key_Cnt==Key_S1)//长按键 减
		{
			Key_Proc.Key_Count++;
			if(Key_Proc.Key_Count>Key_Timer)
			{
				Key_Proc.Key_Flag=1;
			}
			else if((Key_Proc.Key_Count % 10 == 0) && (Key_Proc.Key_Flag != 1))//按下200ms 开始步进
			{
					Value=IP[Raw][Colum];
					Value--;
					IP[Raw][Colum]=Value;
			}
		}
		/*按键处理*/
		if(Key_Proc.Key_Cnt==Key_S0&&Key_Proc.Key_Flag==1) //加
		{
			Value = IP[Raw][Colum];
			Value++;
			IP[Raw][Colum]=Value;
			Key_Proc.Key_Count=0;
		}
		else	if(Key_Proc.Key_Cnt==Key_S1&&Key_Proc.Key_Flag==1) //减
		{
			Value = IP[Raw][Colum];
			Value--;
			IP[Raw][Colum]=Value;
			Key_Proc.Key_Count=0;
		}
		else
		{
			Key_Proc.Key_Flag=0;
		}
		DisUpadte();
		
		if(fg==1)
		{
			buf[0][0]=IP[0][0];
			buf[0][1]=IP[0][1];
			buf[0][2]=IP[0][2];
			buf[2][0]=IP[0][0];
			buf[2][1]=IP[0][1];
			buf[2][2]=IP[0][2];
			
			buf[0][3]=wg;
			buf[1][0]=255;
			buf[1][1]=255;
			buf[1][2]=255;
			buf[1][3]=0;
			buf[2][3]=1;
		}
		
}
void NET_IP_Set(void)
{
	static u8 pValue = 0;
	BEEP = 0;
	switch(KeyFuncIndex)
	{
		case SetNetOnePage:      NET_IP_Option(One_Ip     ,One_Ip  ,0,254);break;
		case SetNetTwoPage:      NET_IP_Option(Two_Ip     ,Two_Ip  ,0,253);break;
		case SetNetThrPage:      NET_IP_Option(Thr_Ip     ,Thr_Ip  ,0,252);break;
		case SetNetFourPage:     NET_IP_Option(Four_Ip    ,Four_Ip ,0,251);break;
		case SetNetFivePage:     NET_IP_Option(Five_Ip    ,Five_Ip ,0,250);break;
		case SetNetSixPage:      NET_IP_Option(Six_Ip     ,Six_Ip  ,0,249);break;
		case SetNetSevenPage:    NET_IP_Option(Seven_Ip   ,Seven_Ip,0,248);break;
		case SetNetEightPage:    NET_IP_Option(Eight_Ip   ,Eight_Ip,0,247);break;
		case SetNetNightPage:    NET_IP_Option(Night_Ip   ,One_Ip  ,1,254);break;
		case SetNetTenPage:      NET_IP_Option(Ten_Ip     ,Two_Ip  ,1,253);break;
		case SetNetElevenPage:   NET_IP_Option(Eleven_Ip  ,Thr_Ip  ,1,252);break;
		case SetNetTwelvePage:   NET_IP_Option(Twelve_Ip  ,Four_Ip ,1,251);break;
		case SetNetThirteenPage: NET_IP_Option(Thirteen_Ip,Five_Ip ,1,250);break;
		case SetNetFourteenPage: NET_IP_Option(Fourteen_Ip,Six_Ip  ,1,249);break;
		case SetNetFifteenPage:  NET_IP_Option(Fifteen_Ip ,Seven_Ip,1,248);break;
		case SetNetSixteenPage:  NET_IP_Option(Sixteen_Ip ,Eight_Ip,1,247);break;
		default:break;
	}
	BEEP = 0;
	if(KeyFuncIndex != pValue)//判断翻页
	{
		pValue = KeyFuncIndex;
		BEEP = 0;
		DisUpadteCancel();
		Colum = 0;
		Raw = 0;
	}
}


void DisUpadte(void)
{
	 switch(Colum)
	 {
		 case 0: Fill_Block(0xff,24,29,21+(20*(Raw)),21+(20*(Raw)));break;
		 case 1: Fill_Block(0xff,32,37,21+(20*(Raw)),21+(20*(Raw)));break;
		 case 2: Fill_Block(0xff,40,45,21+(20*(Raw)),21+(20*(Raw)));break;
		 case 3: Fill_Block(0xff,48,53,21+(20*(Raw)),21+(20*(Raw)));break;
	 }		
}

void DisUpadteCancel(void)
{
	 switch(Colum)
	 {
		 case 0: Fill_Block(0x00,24,29,21+(20*(Raw)),21+(20*(Raw)));break;
		 case 1: Fill_Block(0x00,32,37,21+(20*(Raw)),21+(20*(Raw)));break;
		 case 2: Fill_Block(0x00,40,45,21+(20*(Raw)),21+(20*(Raw)));break;
		 case 3: Fill_Block(0x00,48,53,21+(20*(Raw)),21+(20*(Raw)));break;
	 }		
}



