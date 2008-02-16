/* LiteOS Version 0.3 */
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

 

#ifndef THREADSH
#define THREADSH


#include "threaddata.h"

extern volatile thread *current_thread;; 

extern thread thread_table[ LITE_MAX_THREADS ];

/**\defgroup thread Thread operations
     This module defines various thread operations, including add/remove/wakeup/sleep, etc 

*/

/**\ingroup  thread */

/** @{ */

/**\ingroup  thread */
///init thread      
void thread_init();

/**\ingroup  thread */
///create thread
int create_thread(void (*fcn)(), uint16_t *ram_start, uint16_t * stack_ptr, uint16_t staticdatasize, uint8_t priority, char *threadName);


/**\ingroup  thread */
///destroy thread
void destroy_user_thread();


//thread behavior 
/**\ingroup  thread */
void thread_yield() __attribute__((noinline));

/**\ingroup  thread */
void lite_switch_to_user_thread()  __attribute__((noinline));


/**\ingroup  thread */
void sleepThread( uint16_t milli) ;


/**\ingroup  thread */
void  threads_handle_service()  __attribute__((noinline));

/**\ingroup  thread */
void   thread_func_dispatcher() __attribute__((naked));

/**\ingroup  thread */
uint8_t is_thread();

/**\ingroup  thread */
void thread_task();

/**\ingroup  thread */
void thread_wakeup(uint8_t id);

/**\ingroup  thread */
void postNewThreadTask(); 

/**\ingroup  thread */
void ServiceTimerFired(uint8_t id);

/**\ingroup  thread */
thread** getThreadAddress(); 

/**\ingroup  thread */
//__attribute__((naked)) 
int getThreadIndexAddress();

/**\ingroup  thread */
void Barrier_unblock( uint8_t type, uint8_t id );

/**\ingroup  thread */
void break_point_function();




#endif
