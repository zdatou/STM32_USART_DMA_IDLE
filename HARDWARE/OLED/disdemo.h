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

typedef enum {
	SelectMenu1,
	SelectMenu2,
	SelectMenu3,
	SelectMenu4,
	DataShowMenu,
	ChannelTrigSet,
	RefrenceSet,
	RefrenceChaneelSelect,
} MenuPage; 


typedef struct{
	u16 ms;
	u16 us;
	u16 ns;
	u16 ps;
}timeshow_t;

typedef struct{
	u8 _int;
	u8 _deci;
}tri_t;


void DisDemo(uint8_t Key_Value);//选择界面
void Demo_OpMenu1(void);
void Demo_OpMenu2(void);
void Demo_OpMenu3(void);
void Demo_OpMenu4(void);
void Demo_DataShow(void);
void Demo_TriSet(void);
void Demo_E_I_Set(void);
void Demo_RefChannel(void);
float Value_Opt(float value,u8 opt,u8 addorsub);
#endif

