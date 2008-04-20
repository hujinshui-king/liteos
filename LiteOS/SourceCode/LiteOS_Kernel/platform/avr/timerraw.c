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


 
/*									tab:4
 * "Copyright (c) 2000-2003 The Regents of the University  of California.  
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice, the following
 * two paragraphs and the author appear in all copies of this software.
 * 
 * IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF
 * CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
 *
 * Copyright (c) 2002-2003 Intel Corporation
 * All rights reserved.
 *
 * This file is distributed under the terms in the attached INTEL-LICENSE     
 * file. If you do not find these files, copies can be found by writing to
 * Intel Research Berkeley, 2150 Shattuck Avenue, Suite 1300, Berkeley, CA, 
 * 94704.  Attention:  Intel License Inquiry.
 */
/*
 * Authors:		Jason Hill, David Gay, Philip Levis, Nelson Lee
 * Date last modified:  6/25/02
 *
 *
 */
 /*
 * Copyright (c) 2002-2003 Intel Corporation
 * All rights reserved.
 *
 * This file is distributed under the terms in the attached INTEL-LICENSE     
 * file. If you do not find these files, copies can be found by writing to
 * Intel Research Berkeley, 2150 Shattuck Avenue, Suite 1300, Berkeley, CA, 
 * 94704.  Attention:  Intel License Inquiry.
 */
/* 
 * Authors:  Su Ping,  (converted to nesC by Sam Madden)
 *           David Gay,      Intel Research Berkeley Lab
 *           Phil Levis
 * Date:     4/12/2002
 * NesC conversion: 6/28/2002
 * interface cleanup: 7/16/2002
 * Configuration:     8/12/2002
 */

/**
 * @author Su Ping
 * @author (converted to nesC by Sam Madden)
 * @author David Gay
 * @author Intel Research Berkeley Lab
 * @author Phil Levis
 */



#if defined(PLATFORM_AVR_IRIS)
 #include "clockraw_iris.h"
#elif defined(PLATFORM_AVR)
 #include "clockraw.h"
#endif


#include  "../../system/scheduling.h"
#include "avrhardware.h"
#include "timerraw.h"
#include "../../system/threads.h"
#include "../micaz/hplcc2420interruptm.h"
#include "../../system/generictimer.h"

uint32_t TimerM_mState;

uint8_t TimerM_setIntervalFlag;
uint8_t TimerM_mScale;

uint8_t TimerM_mInterval;

int8_t TimerM_queue_head;
int8_t TimerM_queue_tail;

uint8_t TimerM_queue_size;
uint8_t TimerM_queue[NUM_TIMERS];

volatile uint16_t TimerM_interval_outstanding;


struct TimerM_timer_s {
  uint8_t type;
  int32_t ticks;
  int32_t ticksLeft;
} TimerM_mTimerList[NUM_TIMERS];

enum  {
  TimerM_maxTimerInterval = 230
};




inline  
result_t TimerM_StdControl_init(void)
{
  TimerM_mState = 0;
  TimerM_setIntervalFlag = 0;
  TimerM_queue_head = TimerM_queue_tail = -1;
  TimerM_queue_size = 0;
  TimerM_mScale = 3;
  TimerM_mInterval = TimerM_maxTimerInterval;
  return TimerM_Clock_setRate(TimerM_mInterval, TimerM_mScale);
}


inline uint8_t TimerM_Timer_stop(uint8_t id)
{
   if (id > NUM_TIMERS)
      return FAIL; 
   if (TimerM_mState & (0x1L << id)) { 
     _atomic_t _atomic = _atomic_start();
	 TimerM_mState &= ~(0x1L << id);
	 _atomic_end(_atomic); 

	 if (!TimerM_mState){
	   TimerM_setIntervalFlag = 1; 
	  }
	  return SUCCESS;
  }
  return FAIL;      
      

}



inline  
result_t TimerM_Timer_start(uint8_t id, char type, 
uint32_t interval)
{
  uint8_t diff;

  if (id >= NUM_TIMERS) {
    return FAIL;
    }
  if (type > TIMER_ONE_SHOT) {
    return FAIL;
    }

  if (type == TIMER_REPEAT && interval <= 2) {
    return FAIL;
    }
  TimerM_mTimerList[id].ticks = interval;
  TimerM_mTimerList[id].type = type;

  { _atomic_t _atomic = _atomic_start();
    {
      diff = TimerM_Clock_readCounter();
      interval += diff;
      TimerM_mTimerList[id].ticksLeft = interval;
      TimerM_mState |= 0x1L << id;
      if (interval < TimerM_mInterval) {
          TimerM_mInterval = interval;
          TimerM_Clock_setInterval(TimerM_mInterval);
          TimerM_setIntervalFlag = 0;
         
        }
    }
    _atomic_end(_atomic); }
  return SUCCESS;
}



inline result_t TimerM_Clock_setRate(char arg_0xa369e18, char arg_0xa369f58){
  unsigned char result;
  result = HPLClock_Clock_setRate(arg_0xa369e18, arg_0xa369f58);
  return result;
}



void TimerM_adjustInterval(void)
{
  uint8_t i;
  uint8_t val = TimerM_maxTimerInterval;

  if (TimerM_mState) {
      for (i = 0; i < NUM_TIMERS; i++) {
          if (TimerM_mState & (0x1L << i) && TimerM_mTimerList[i].ticksLeft < val) {
              val = TimerM_mTimerList[i].ticksLeft;
            }
        }
      { _atomic_t _atomic = _atomic_start();
        {
          i = TimerM_Clock_readCounter() + 3;
          if (val < i) {
              val = i;
            }
          TimerM_mInterval = val;
          TimerM_Clock_setInterval(TimerM_mInterval);
          TimerM_setIntervalFlag = 0;
        }
        _atomic_end(_atomic); }
    }
  else {
      { _atomic_t _atomic = _atomic_start();
        {
          TimerM_mInterval = TimerM_maxTimerInterval;
          TimerM_Clock_setInterval(TimerM_mInterval);
          TimerM_setIntervalFlag = 0;
        }
        _atomic_end(_atomic); }
    }
}

inline 
void TimerM_enqueue(uint8_t value)
{
  if (TimerM_queue_tail == NUM_TIMERS - 1) {
    TimerM_queue_tail = -1;
    }
  TimerM_queue_tail++;
  TimerM_queue_size++;
  TimerM_queue[(uint8_t )TimerM_queue_tail] = value;
}





inline   
result_t TimerM_Timer_default_fired(uint8_t id)
{
  return SUCCESS;
}


inline result_t TimerM_Timer_fired(uint8_t id){
  GenericTimerFired(id); 
  return SUCCESS;
}


inline 
uint8_t TimerM_dequeue(void)
{
  if (TimerM_queue_size == 0) {
    return NUM_TIMERS;
    }
  if (TimerM_queue_head == NUM_TIMERS - 1) {
    TimerM_queue_head = -1;
    }
  TimerM_queue_head++;
  TimerM_queue_size--;
  return TimerM_queue[(uint8_t )TimerM_queue_head];
}


inline  void TimerM_signalOneTimer(void)
{
  uint8_t itimer = TimerM_dequeue();

  if (itimer < NUM_TIMERS) {
    TimerM_Timer_fired(itimer);
    }
}

inline  
void TimerM_HandleFire(void)
{
  uint8_t i;
  uint16_t int_out;

  TimerM_setIntervalFlag = 1;


  { _atomic_t _atomic = _atomic_start();
    {
      int_out = TimerM_interval_outstanding;
      TimerM_interval_outstanding = 0;
    }
    _atomic_end(_atomic); }
  if (TimerM_mState) {
      for (i = 0; i < NUM_TIMERS; i++) {
          if (TimerM_mState & (0x1L << i)) {
              TimerM_mTimerList[i].ticksLeft -= int_out;
              if (TimerM_mTimerList[i].ticksLeft <= 2) {


                  if (postTask(TimerM_signalOneTimer, 7)) {
                      if (TimerM_mTimerList[i].type == TIMER_REPEAT) {
                          TimerM_mTimerList[i].ticksLeft += TimerM_mTimerList[i].ticks;
                        }
                      else 
                        {
                          TimerM_mState &= ~(0x1L << i);
                        }
                      TimerM_enqueue(i);
                    }
                  else {
                      {
                      }
                      ;


                      TimerM_mTimerList[i].ticksLeft = TimerM_mInterval;
                    }
                }
            }
        }
    }


  { _atomic_t _atomic = _atomic_start();
    int_out = TimerM_interval_outstanding;
    _atomic_end(_atomic); }
  if (int_out == 0) {
    TimerM_adjustInterval();
    }
}

inline result_t TimerM_Clock_fire(void)
{
  { _atomic_t _atomic = _atomic_start();
    {

      if (TimerM_interval_outstanding == 0) {
        postTask(TimerM_HandleFire,7);
        }
      else {
        }
      ;

      TimerM_interval_outstanding += TimerM_Clock_getInterval() + 1;
    }
    _atomic_end(_atomic); }
  return SUCCESS;
}


inline  
result_t TimerM_StdControl_start(void)
{
  return SUCCESS;
}

inline uint8_t TimerM_Clock_readCounter(void){
  unsigned char result;
  result = HPLClock_Clock_readCounter();
  return result;
}


inline uint8_t TimerM_Clock_getInterval(void){
  unsigned char result;

  result = HPLClock_Clock_getInterval();

  return result;
}

inline void TimerM_Clock_setInterval(uint8_t arg_0xa362928){
  HPLClock_Clock_setInterval(arg_0xa362928);
}

