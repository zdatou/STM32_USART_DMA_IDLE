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



void DisDemo(uint8_t Key_Value);//ѡ�����
void Demo_OpMenu1(void);
void Demo_OpMenu2(void);
void Demo_OpMenu3(void);
void Demo_OpMenu4(void);
void Demo_DataShow(void);
#endif

