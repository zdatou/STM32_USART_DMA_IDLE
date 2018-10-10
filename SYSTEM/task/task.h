#ifndef _TASK_H_
#define _TASK_H_
#include "sys.h"
typedef  void * (*FuncTask)(void * msg);
typedef struct _st_task
{
	u8 state;//��ǰ����״̬��־
	u16 cnt;//��ǰ����ֵ
	u16 aim_val;//Ŀ��ֵ
	u8 enable;//ʹ�ܱ�־
	FuncTask TaskCallBack;//����ص�����
	void *msg;//��Ϣ	
}stTask,*pstTask;

extern u32 sys_time;
void InitTask(void);
void TaskRun(void);
void TaskTick(void);
#endif
