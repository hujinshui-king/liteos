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

#ifndef THREADH
#define THREADH

/** \defgroup thread Thread Operations

      This module implements LiteOS thread operations
*/



#include "liteoscommon.h"

/** \ingroup thread
 
    Sleep the current thread for a period of time
*/

void sleepThread(int milliseconds);

/** \ingroup thread

   Yield the current thread
*/

void yield(); 

/** \ingroup thread

   Get the current thread internal pointer
*/

thread **getCurrentThread();


/** \ingroup thread
 
    Get the index of the current thread in the internal thread table 
*/
uint8_t getCurrentThreadIndex();


/** \ingroup thread
  
    Post a task to the kernel scheduler. This task will be running in the context of the kernel
*/
void postTask(void (*tp) (void), uint16_t priority);


/** \ingroup thread
  
    Post a thread scheduling task in the kernel
    
*/
void syscall_postThreadTask(); 


void debugValue(uint16_t v1, uint16_t v2, uint16_t v3); 


/** \ingroup thread
 
    Hibernate the current thread until it is waken up
*/
void hibernateThread();


#endif 
