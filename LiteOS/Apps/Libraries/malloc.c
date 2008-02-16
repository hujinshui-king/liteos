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


#include "malloc.h"

void *malloc(uint16_t size)
{
 
   void (*mallocfunc)(void) = (void (*)(void))MALLOCHANDLE;
   void *resultaddr; 

   asm volatile("push r20" "\n\t"
                "push r21" "\n\t"
                ::);
   asm volatile(" mov r20, %A0" "\n\t"
	              " mov r21, %B0" "\n\t"
				        :
				 	      :"r" (size)				 
               );
                            	
   mallocfunc();     
   
   asm volatile(" mov %A0, r20" "\n\t"
	               "mov %B0, r21" "\n\t"
				        :"=r" (resultaddr)
				        :
                );
    asm volatile("pop r21" "\n\t"
	             "pop r20" "\n\t"
	              ::);
   return (void*)resultaddr; 
}
	
	

void free(void *ptr)
{
  	
	 void (*mallocfunc)(void) = (void (*)(void))FREEMALLOCHANDLE;

   asm volatile("push r20" "\n\t"
                "push r21" "\n\t"
                ::);
   asm volatile(" mov r20, %A0" "\n\t"
	              " mov r21, %B0" "\n\t"
				        :
				 	      :"r" (ptr)				 
               );
                            	
   mallocfunc();     
   
   asm volatile("pop r21" "\n\t"
	             "pop r20" "\n\t"
	              ::);
   return; 	
}

