/*
The following is the license of LiteOS.

This file is part of LiteOS.
Copyright Qing Cao, 2007-2008, University of Illinois , qcao2@uiuc.edu

LiteOS is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

LiteOS is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with LiteOS.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "thread.h"
#include <avr/io.h>
#include "mutex.h" 	     





//Warning: this returns the address of the currentthread pointer. 

thread **getCurrentThread()
{
   thread **current_thread; 
   void (*getaddrfp)(void) = (void (*)(void))GET_CURRENT_THREAD_ADDRESS_FUNCTION; 
   asm volatile("push r20" "\n\t"
                "push r21" "\n\t"
                ::);
   getaddrfp();     
   asm volatile(" mov %A0, r20" "\n\t"
	             "mov %B0, r21" "\n\t"
				 :"=r" (current_thread)
				 :
                );
    asm volatile("pop r21" "\n\t"
	             "pop r20" "\n\t"
	              ::);
   return current_thread; 
}


void **getKernelStackPtr()
	{
   void **ptr;
   void (*getaddrfp)(void) = (void (*)(void))GET_KERNEL_STACK_PTR_ADDRESS; 
   asm volatile("push r20" "\n\t"
                "push r21" "\n\t"
                ::);
   getaddrfp();     
   asm volatile(" mov %A0, r20" "\n\t"
	             "mov %B0, r21" "\n\t"
				 :"=r" (ptr)
				 :
                );
    asm volatile("pop r21" "\n\t"
	             "pop r20" "\n\t"
	              ::);
   return ptr; 
}







uint8_t getCurrentThreadIndex()
{
   
   uint16_t index; 
   void (*getaddrfp)(void) = (void (*)(void))GET_CURRENT_THREAD_INDEX_FUNCTION; 
   asm volatile("push r20" "\n\t"
                "push r21" "\n\t"
                ::);
   getaddrfp();     
   asm volatile(" mov %A0, r20" "\n\t"
	             "mov %B0, r21" "\n\t"
				 :"=r" (index)
				 :
                );
    asm volatile("pop r21" "\n\t"
	             "pop r20" "\n\t"
	              ::);
   return (uint8_t)index; 
}


void sleepThread(int milliseconds)
{
/*
   void (*getaddrfp)(void) = (void (*)(void))GET_CURRENT_THREAD_ADDRESS_FUNCTION; 

   asm volatile("push r24" "\n\t"
	             "push r25" "\n\t"
	              ::);
   getaddrfp();     

   asm volatile(" mov %A0, r24" "\n\t"
	             "mov %B0, r25" "\n\t"
				 :"=r" (current_thread)
				 :
                );
    asm volatile("pop r24" "\n\t"
	             "pop r25" "\n\t"
	              ::);
 */
 
   thread **current_thread; 
   current_thread = getCurrentThread(); 
     
   (*current_thread)->state = 4;
   (*current_thread)->data.sleepstate.sleeptime = milliseconds;
   
   yield(); 
}


void hibernateThread()
{
 
   thread **current_thread; 
   current_thread = getCurrentThread(); 
     
   (*current_thread)->state = 5; 
   yield(); 
}


void yield()
{
 void (*yieldfp)(void) = (void (*)(void))YIELD_FUNCTION; 
 yieldfp();                              
}



void syscall_postThreadTask()
{
 void (*postthreadfp)(void) = (void (*)(void))POST_NEW_THREAD_TASK; 
 postthreadfp();                              
}

void syscall_postThreadTask_nologging()
{
 void (*postthreadfp)(void) = (void (*)(void))POST_NEW_THREAD_TASK_NOLOG; 
 postthreadfp();                              
}


void debugValue(uint16_t v1, uint16_t v2, uint16_t v3)
{

  void (*fp)(void) = (void (*)(void))DEBUG_VALUE_SYSCALL; 
   asm volatile("push r8" "\n\t"
                "push r9" "\n\t"
                "push r10" "\n\t"
                "push r11" "\n\t"
				"push r12" "\n\t"
				"push r13" "\n\t"
                ::);
   
   asm volatile(" mov r8, %A0" "\n\t"
	             "mov r9, %B0" "\n\t"
				 :
				 :"r" (v1)
                );


   asm volatile(" mov r10, %A0" "\n\t"
	             "mov r11, %B0" "\n\t"
				 :
				 :"r" (v2)
                );
 
   asm volatile(" mov r12, %A0" "\n\t"
	             "mov r13, %B0" "\n\t"
				 :
				 :"r" (v3)
                );


  fp(); 

  asm volatile("pop r13" "\n\t"
	           "pop r12" "\n\t"
	              ::);

  asm volatile("pop r11" "\n\t"
	           "pop r10" "\n\t"
	              ::);

  asm volatile("pop r9" "\n\t"
	           "pop r8" "\n\t"
	              ::);
  return; 


}


void postTask(void (*tp) (void), uint16_t priority)
{
   void (*fp)(void) = (void (*)(void))POST_TASK_SYSCALL; 
   asm volatile("push r20" "\n\t"
                "push r21" "\n\t"
				"push r22" "\n\t"
				"push r23" "\n\t"
                ::);
   
   asm volatile(" mov r20, %A0" "\n\t"
	             "mov r21, %B0" "\n\t"
				 :
				 :"r" (tp)
                );


   asm volatile(" mov r22, %A0" "\n\t"
	             "mov r23, %B0" "\n\t"
				 :
				 :"r" (priority)
                );

  fp(); 

  asm volatile("pop r23" "\n\t"
	           "pop r22" "\n\t"
	              ::);

  asm volatile("pop r21" "\n\t"
	           "pop r20" "\n\t"
	              ::);
  return; 


}




void postTaskWithoutLogging(void (*tp) (void), uint16_t priority)
{
   void (*fp)(void) = (void (*)(void))POST_TASK_SYSCALL; 
   asm volatile("push r20" "\n\t"
                "push r21" "\n\t"
				"push r22" "\n\t"
				"push r23" "\n\t"
                ::);
   
   asm volatile(" mov r20, %A0" "\n\t"
	             "mov r21, %B0" "\n\t"
				 :
				 :"r" (tp)
                );


   asm volatile(" mov r22, %A0" "\n\t"
	             "mov r23, %B0" "\n\t"
				 :
				 :"r" (priority)
                );

  fp(); 

  asm volatile("pop r23" "\n\t"
	           "pop r22" "\n\t"
	              ::);

  asm volatile("pop r21" "\n\t"
	           "pop r20" "\n\t"
	              ::);
  return; 


}


thread_create_block_type *getCurrentThreadControlBlockAddr()
{
   thread_create_block_type *blockinfo; 
   void (*getaddrfp)(void) = (void (*)(void))GET_THREAD_CONTROL_BLOCK; 
   asm volatile("push r20" "\n\t"
                "push r21" "\n\t"
                ::);
   getaddrfp();     
   asm volatile(" mov %A0, r20" "\n\t"
	              " mov %B0, r21" "\n\t"
				 :"=r" (blockinfo)
				 :
               );
    asm volatile("pop r21" "\n\t"
	               "pop r20" "\n\t"
	               ::);
   return blockinfo; 
}


void *getCurrentThreadControlMutexAddr()
{
   void *blockinfo; 
   void (*getaddrfp)(void) = (void (*)(void))GET_THREAD_CONTROL_MUTEX; 
   asm volatile("push r20" "\n\t"
                "push r21" "\n\t"
                ::);
   getaddrfp();     
   asm volatile(" mov %A0, r20" "\n\t"
	              " mov %B0, r21" "\n\t"
				 :"=r" (blockinfo)
				 :
               );
    asm volatile("pop r21" "\n\t"
	               "pop r20" "\n\t"
	               ::);
   return blockinfo; 
}





void createThread(void (*fp)(), uint8_t* ram_start, uint8_t *stack_ptr, uint8_t priority, const char *threadname)
{
 
 
   mutex* createthreadmutex; 
   thread_create_block_type *threadblock;
   void (*getaddrfp)(void) = (void (*)(void))CREATE_THERAD_SYSCALL;

  
   threadblock = getCurrentThreadControlBlockAddr(); 
   createthreadmutex = (mutex *)getCurrentThreadControlMutexAddr(); 
   
   Mutex_lock(createthreadmutex);
     
   threadblock-> fp = fp; 
   threadblock->ram_start = ram_start; 
   threadblock->stack_ptr = stack_ptr; 
   threadblock->priority = priority;
   threadblock->threadname = threadname; 

   getaddrfp(); 
   
   sleepThread(20); 
   
  
   Mutex_unlock(createthreadmutex); 

   return; 
}




void setThreadTerminateHandler(void (*tp) (void))
{
   void (*fp)(void) = (void (*)(void))SYSCALL_USER_THREAD_TERMINATE_TASK; 
   asm volatile("push r20" "\n\t"
                "push r21" "\n\t"
				        ::);
   
   asm volatile(" mov r20, %A0" "\n\t"
	             "mov r21, %B0" "\n\t"
				 :
				 :"r" (tp)
                );


  fp(); 

  asm volatile("pop r21" "\n\t"
	             "pop r20" "\n\t"
	             ::);

  return; 


}


void setTimerFunction(uint16_t period, uint8_t type, void (*callback)(void))
{
	
	 void (*fp)(void) = (void (*)(void))SYSCALL_TIMER_FIRE_TASK; 
 	 uint16_t type16; 

   type16 = type; 
   
   asm volatile("push r18" "\n\t"
                "push r19" "\n\t"
								"push r20" "\n\t"
								"push r21" "\n\t"
								"push r22" "\n\t"
								"push r23" "\n\t"								
                ::);
   
   asm volatile(" mov r18, %A0" "\n\t"
	               "mov r19, %B0" "\n\t"
				 :
				 :"r" (period)
                );



   asm volatile(" mov r20, %A0" "\n\t"
	               "mov r21, %B0" "\n\t"
				 :
				 :"r" (type16)
                );


   asm volatile(" mov r22, %A0" "\n\t"
	              " mov r23, %B0" "\n\t"
				 :
				 :"r" (callback)
                );

  fp(); 

  asm volatile("pop r23" "\n\t"
	             "pop r22" "\n\t"
	              ::);

  asm volatile("pop r21" "\n\t"
	             "pop r20" "\n\t"
	              ::);
	              	
	asm volatile("pop r19" "\n\t"
	             "pop r18" "\n\t"
	              ::);
  return; 
	
	
}




