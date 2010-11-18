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

void lib_sleep_thread(uint16_t milliseconds);

/** \ingroup thread

   Yield the current thread
*/

void lib_yield(); 

/** \ingroup thread

   Get the current thread internal pointer
*/

lib_thread **lib_get_current_thread();


/** \ingroup thread
 
    Get the index of the current thread in the internal thread table 
*/
uint8_t lib_get_current_thread_index();


/** \ingroup thread
  
    Post a task to the kernel scheduler. This task will be running in the context of the kernel
*/
void lib_post_new_task(void (*tp) (void), uint16_t priority);


/** \ingroup thread
  
    Post a thread scheduling task in the kernel
    
*/
void syscall_post_thread_task(); 





/** \ingroup thread
 
    Hibernate the current thread until it is waken up
*/
void lib_hibernate_thread();


/** \ingroup thread
 
   Create a new thread 
*/
void lib_create_new_thread(void (*fp)(), uint8_t* ram_start, uint8_t *stack_ptr, uint8_t priority, const char *threadname);





void lib_set_thread_termination_handler(void (*tp) (void));


void lib_set_timer_function(uint16_t period, uint8_t type, void (*callback)(void));


void syscall_post_thread_task_no_logging();

void **lib_get_kernel_stack_ptr(); 

lib_thread *lib_get_thread_table_start();

#endif 
