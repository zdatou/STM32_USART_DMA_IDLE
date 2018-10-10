#ifndef _TASK_H_
#define _TASK_H_
#include "sys.h"
typedef  void * (*FuncTask)(void * msg);
typedef struct _st_task
{
	u8 state;//当前任务状态标志
	u16 cnt;//当前计数值
	u16 aim_val;//目标值
	u8 enable;//使能标志
	FuncTask TaskCallBack;//任务回调函数
	void *msg;//信息	
}stTask,*pstTask;

extern u32 sys_time;
void InitTask(void);
void TaskRun(void);
void TaskTick(void);
#endif
