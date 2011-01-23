#include "scheduling.h"
#include "avrhardware.h"

typedef struct {
  void (*tp) (void);
  uint8_t priority;
} TOSH_sched_entry_T;


enum {

  TOSH_MAX_TASKS = 16,
  TOSH_TASK_BITMASK = (TOSH_MAX_TASKS - 1)

};


volatile TOSH_sched_entry_T TOSH_queue[TOSH_MAX_TASKS];

volatile uint8_t TOSH_sched_num;

void initScheduling(void)
{
  int i;
  TOSH_sched_num = 0;
  for (i = 0; i < TOSH_MAX_TASKS; i++)
    TOSH_queue[i].tp = (void *)0;
}



bool postTask(void (*tp) (void), uint8_t priority)  {
  _atomic_t fInterruptFlags;
  uint8_t tmp;


  fInterruptFlags = _atomic_start();


  if (TOSH_sched_num == TOSH_MAX_TASKS)
  
  {
    _atomic_end(fInterruptFlags);
    return FALSE;
  }

 
  for (tmp = 0; tmp < TOSH_MAX_TASKS; tmp++)
  {
    if (TOSH_queue[tmp].tp == NULL)
	  {
	     TOSH_queue[tmp].tp = tp;
	     TOSH_queue[tmp].priority = priority; 
		 TOSH_sched_num++;
         _atomic_end(fInterruptFlags);
         return TRUE;
	  }
  }
  _atomic_end(fInterruptFlags);
 return FALSE; 
}




bool runNextTask ()
{
  _atomic_t fInterruptFlags;
 
  uint8_t currentpriority;
  uint8_t tmp;
  uint8_t reserved;

  void (*func)(void);
  
  fInterruptFlags = _atomic_start();
  
  currentpriority = 0;

  func = NULL; 
  
  if (TOSH_sched_num == 0)
  
  {
    _atomic_end(fInterruptFlags);
    __asm volatile ("sei");
	__asm volatile ("sleep");
    return FALSE;
  }

 for (tmp = 0; tmp < TOSH_MAX_TASKS; tmp++)
  {
    if ((TOSH_queue[tmp].tp != NULL)&&(TOSH_queue[tmp].priority > currentpriority))
	  {
	     func =  TOSH_queue[tmp].tp;
		 reserved = tmp; 	
	     currentpriority = TOSH_queue[tmp].priority; 
		
	  }   
  }		
		
	if (func!=NULL)
	 {TOSH_sched_num--;
	  TOSH_queue[reserved].tp = NULL; 
     }
  
  _atomic_end(fInterruptFlags);

  if (func!=NULL)
  func();
  
  return TRUE;
	
}

