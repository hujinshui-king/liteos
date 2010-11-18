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

#include "file.h"
#include "stringutil.h"
#include "thread.h"
#include "mutex.h"
#include "liteoscommon.h"

volatile uint16_t globalpagenum;
volatile uint16_t globalpageoffset;
volatile uint8_t *globalbuffer;

void *getEventBlockAddress()
{
   
   void *retaddr; 
   void (*getaddrfp)(void) = (void (*)(void))GET_LOGGER_BLOCK_ADDRESS;
   asm volatile("push r20" "\n\t"
                "push r21" "\n\t"
                ::);
   getaddrfp();
   asm volatile(" mov %A0, r20" "\n\t"
	             "mov %B0, r21" "\n\t"
				 :"=r" (retaddr)
				 :);
    asm volatile("pop r21" "\n\t"
	             "pop r20" "\n\t"
	              ::);
    return retaddr;
}



void enableTracing()
{    
   void (*getaddrfp)(void) = (void (*)(void))ENABLE_TRACING_SYSCALL;
   getaddrfp();
   return;
}



void disableTracing()
{     
   void (*getaddrfp)(void) = (void (*)(void))DISABLE_TRACING_SYSCALL;
   getaddrfp();
   return;
}





void addUserTracePoint(uint16_t pagenum, uint16_t pageoffset)
{
   void (*fp)(void) = (void (*)(void))SYSCALL_USER_ADD_TRACE_POINT; 
   globalpagenum = pagenum;
   globalpageoffset = pageoffset; 
   
   asm volatile("push r20" "\n\t"
                "push r21" "\n\t"
								"push r22" "\n\t"
								"push r23" "\n\t"
                ::);
   
   asm volatile(" mov r20, %A0" "\n\t"
	             	" mov r21, %B0" "\n\t"
				 :
				 :"r" (globalpagenum)
                );


   asm volatile(" mov r22, %A0" "\n\t"
	             	" mov r23, %B0" "\n\t"
				 :
				 :"r" (globalpageoffset)
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



void addUserTracePoint_longinstruction(uint16_t pagenum, uint16_t pageoffset)
{

	 
   void (*fp)(void) = (void (*)(void))SYSCALL_USER_ADD_TRACE_POINT_LONG; 
   	 globalpagenum = pagenum;
	 globalpageoffset = pageoffset; 
   asm volatile("push r20" "\n\t"
                "push r21" "\n\t"
								"push r22" "\n\t"
								"push r23" "\n\t"
                ::);
   
   asm volatile(" mov r20, %A0" "\n\t"
	             	" mov r21, %B0" "\n\t"
				 :
				 :"r" (globalpagenum)
                );


   asm volatile(" mov r22, %A0" "\n\t"
	             	" mov r23, %B0" "\n\t"
				 :
				 :"r" (globalpageoffset)
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



void removeUserTracePoint(uint16_t pagenum, uint16_t pageoffset, uint8_t *buffer)
{
	
	  
   void (*fp)(void) = (void (*)(void))SYSCALL_USER_REMOVE_TRACE_POINT; 
   globalpagenum = pagenum;
   globalpageoffset = pageoffset; 
   globalbuffer = buffer; 
   
   asm volatile(
   							"push r18" "\n\t"
   							"push r19" "\n\t"
   							"push r20" "\n\t"
                "push r21" "\n\t"
								"push r22" "\n\t"
								"push r23" "\n\t"
                ::);
                	
                	
   
   asm volatile(" mov r20, %A0" "\n\t"
	             	" mov r21, %B0" "\n\t"
				 :
				 :"r" (globalpagenum)
                );


   asm volatile(" mov r22, %A0" "\n\t"
	             	" mov r23, %B0" "\n\t"
				 :
				 :"r" (globalpageoffset)
                );
                
   asm volatile(" mov r18, %A0" "\n\t"
	             	" mov r19, %B0" "\n\t"
				 :
				 :"r" (globalbuffer)
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



void removeUserTracePoint_longinstruction(uint16_t pagenum, uint16_t pageoffset, uint8_t *buffer)
{
   void (*fp)(void) = (void (*)(void))SYSCALL_USER_REMOVE_TRACE_POINT_LONG; 
   
   globalpagenum = pagenum;
   globalpageoffset = pageoffset; 
   globalbuffer = buffer; 
   asm volatile("push r18" "\n\t"
   							"push r19" "\n\t"
   							"push r20" "\n\t"
                "push r21" "\n\t"
								"push r22" "\n\t"
								"push r23" "\n\t"
                ::);
   
   asm volatile(" mov r20, %A0" "\n\t"
	             	" mov r21, %B0" "\n\t"
				 :
				 :"r" (globalpagenum)
                );


   asm volatile(" mov r22, %A0" "\n\t"
	             	" mov r23, %B0" "\n\t"
				 :
				 :"r" (globalpageoffset)
                );
                
   asm volatile(" mov r18, %A0" "\n\t"
	             	" mov r19, %B0" "\n\t"
				 :
				 :"r" (globalbuffer)
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


