#ifndef _task_h_
#define _task_h_
#include "sys.h"

typedef void (*pstr)(void);

typedef struct {
	
	uint32_t Count;
	uint32_t Period;
  pstr CallBackFuntion;
}Task;

void run_task(void);

#endif

