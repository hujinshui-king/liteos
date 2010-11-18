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

#include "system.h"
#include "liteoscommon.h"
#include "types.h"
#include <avr/io.h>


volatile thread ** current_thread;

volatile mutex* msend;

volatile uint16_t * old_stack_ptr ;


_atomic_t _atomic_start(void)
{
  _atomic_t result = SREG;
  asm volatile("cli" "\n\t"::);
  return result;
}



void _atomic_end(_atomic_t oldSreg)
{
  SREG = oldSreg;
}



int rnd()
{

	 int ret;
   void (*getrandomfp)(void) = (void (*)(void))GET_RANDOM_NUMBER_FUNCTION;
   asm volatile("push r20" "\n\t"
                "push r21" "\n\t"
                ::);
   getrandomfp();
   asm volatile(" mov %A0, r20" "\n\t"
	              "mov %B0, r21" "\n\t"
				 :"=r" (ret)
				 :
                );
   asm volatile("pop r21" "\n\t"
	             "pop r20" "\n\t"
	              ::);
   return ret;

}



uint16_t getnodeID()
	{

	   int ret;
	   void (*fp)(void) = (void (*)(void))GET_NODE_ID_FUNCTION;
	   asm volatile("push r20" "\n\t"
					"push r21" "\n\t"
					::);
	   fp();
	   asm volatile(" mov %A0, r20" "\n\t"
					  "mov %B0, r21" "\n\t"
					 :"=r" (ret)
					 :
					);
	   asm volatile("pop r21" "\n\t"
					 "pop r20" "\n\t"
					  ::);
	   return ret;

	}


void setnodeID(uint16_t nodeid)
	{



	   void (*fp)(void) = (void (*)(void))SET_NODE_ID_FUNCTION;
	   asm volatile("push r20" "\n\t"
					"push r21" "\n\t"
					::);

	   asm volatile(" mov r20, %A0" "\n\t"
					  "mov r21, %B0" "\n\t"
					 :
					 :"r" (nodeid)
					);
	   fp();
	   asm volatile("pop r21" "\n\t"
					 "pop r20" "\n\t"
					  ::);
	   return;

	}







