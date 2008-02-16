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


#ifndef THREADDATAH
#define THREADDATAH


#ifndef TINYTHREAD_THREADCOUNT
#define TINYTHREAD_THREADCOUNT 8
#endif

#include "../types/types.h"

/**\ingroup thread*/

//Our mutexes

typedef struct mutex{
  volatile uint8_t lock;
  volatile uint8_t waiting;
  volatile uint8_t lockingthreadid; 
}mutex;


typedef struct thread{
  volatile uint16_t * sp;
  volatile uint8_t state;

  uint8_t priority;
  volatile uint8_t remaincredits; 
  uint8_t threadName[12];

  uint16_t *ramstart;
  uint16_t *ramend; 
  uint16_t sizeofBss; 
  
  volatile union{
    void (*tp) ();
   
    volatile mutex *m;
    
    struct {	 
	  uint16_t sleeptime;
	  uint8_t sleeptype;
	  }sleepstate;

  /*  struct {
	  uint8_t id; 
	  uint16_t address;
	  uint8_t length;
	  uint8_t *msg;
	 }radiostate; 
	 */
	     
    struct {
      uint8_t type, id;
	  uint8_t *msg; 
    }iostate;

    struct {
	 int adcreading;
	}adcstate; 

   }data;

  volatile union{
   	struct {
	  uint8_t *fileptr;
	  uint8_t *bufferptr; 
	  uint16_t bytes;
	}filestate;
	 
	struct {
	  uint8_t *fileptr;
	  int offset; 
	  int position; 
	}fileseekstate;

	}filedata; 

}thread;



//This must be a power of 2
/**\ingroup thread*/
#define LITE_MAX_THREADS      TINYTHREAD_THREADCOUNT

/**\ingroup thread*/
#define LITE_MAX_THREADS_MASK (LITE_MAX_THREADS - 1)

/**\ingroup thread*/
enum {
  STATE_NULL = 0,     //There is no thread here
  STATE_READY = 1,    //This thread has been created but never executed before
  STATE_ACTIVE = 2,   //There is a thread here, not blocked or sleeping
  STATE_BLOCKED = 3,  //This thread is blocked on a mutex
  STATE_PRESLEEP = 4,    //This thread is about to sleep
  STATE_SLEEP = 5,
  STATE_IO = 6,       //This thread is blocked until I/O Completion
  STATE_FILE = 7,
  STATE_BREAK = 8,
  STATE_MEM_ERROR = 9
};


/**\ingroup thread*/
void thread_yield();


/**\ingroup thread*/
void call_fcn_ptr(void (*tp)());



#endif
