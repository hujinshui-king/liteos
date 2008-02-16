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


#include "mutex.h"
#include "system.h"
#include "thread.h"
#include "liteoscommon.h"

void Mutex_lock(mutex *m)
{
  
    thread **current_thread; 
    uint8_t threadindex; 
    

	_atomic_t currentatomic;
    
	current_thread = getCurrentThread(); 
	threadindex = getCurrentThreadIndex(); 

	currentatomic = _atomic_start();
	
    if(m->lock){  
        (*current_thread)->state = STATE_BLOCKED;
        (*current_thread)->data.m = m;
        
      
        m->waiting++;
       
        yield();
     
        m->waiting--;
      }
      m->lock = 1;
      m->lockingthreadid = threadindex; 
      
    _atomic_end(currentatomic);  


}



void Mutex_unlock(mutex *m)
{
     
   void (*getaddrfp)(void) = (void (*)(void))MUTEXUNLOCKFUNCTION; 
   asm volatile("push r20" "\n\t"
                "push r21" "\n\t"
                ::);
      
   asm volatile(" mov r20, %A0" "\n\t"
	              " mov r21, %B0" "\n\t"
				 :
				 :"r" (m)
                );

   getaddrfp();
   asm volatile( "pop r21" "\n\t"
	             "pop r20" "\n\t"
	              ::);
   return;  
}



 


void Barrier_block(uint8_t type, uint8_t id){
   
    _atomic_t currentatomic;
	thread **current_thread; 

     
	current_thread = getCurrentThread(); 

    currentatomic = _atomic_start();
    
      (*current_thread)->state = STATE_IO;
      (*current_thread)->data.iostate.type = type;
      (*current_thread)->data.iostate.id = id;
      yield();
    
    _atomic_end(currentatomic);
}



mutex *getRadioMutexAddress()
{
   mutex *msend;   
   void (*getaddrfp)(void) = (void (*)(void))GETRADIOMUTEXFUNCTION; 
   asm volatile("push r20" "\n\t"
                "push r21" "\n\t"
                ::);
   getaddrfp();     
   asm volatile(" mov %A0, r20" "\n\t"
	             "mov %B0, r21" "\n\t"
				 :"=r" (msend)
				 :
                );
    asm volatile("pop r21" "\n\t"
	             "pop r20" "\n\t"
	              ::);
    return msend; 
}
 


mutex *getSerialMutexAddress()
{
   mutex *msend;   

   void (*getaddrfp)(void) = (void (*)(void))GETSERIALMUTEXFUNCTION;
   
   asm volatile("push r20" "\n\t"
                "push r21" "\n\t"
                ::);
   getaddrfp();     
   asm volatile(" mov %A0, r20" "\n\t"
	             "mov %B0, r21" "\n\t"
				 :"=r" (msend)
				 :
                );
   asm volatile("pop r21" "\n\t"
	             "pop r20" "\n\t"
	              ::);
   return msend; 
}
 



mutex *getFileMutexAddress()
{
   mutex *mfile;   
   void (*getaddrfp)(void) = (void (*)(void))GETFILEMUTEXADDRESS; 
   asm volatile("push r20" "\n\t"
                "push r21" "\n\t"
                ::);
   getaddrfp();     
   asm volatile(" mov %A0, r20" "\n\t"
	             "mov %B0, r21" "\n\t"
				 :"=r" (mfile)
				 :
                );
    asm volatile("pop r21" "\n\t"
	             "pop r20" "\n\t"
	              ::);
    return mfile; 
}
 
