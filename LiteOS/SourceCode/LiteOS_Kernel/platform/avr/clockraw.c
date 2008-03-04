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


#include "clockraw.h"
#include "timerraw.h"
#include "../../system/threads.h"

extern volatile uint16_t * old_stack_ptr ;

extern volatile uint16_t *stackinterrupt_ptr; 

inline   
uint8_t HPLClock_Clock_readCounter(void)
{
	return TCNT0;
  //return * (volatile unsigned char *)(unsigned int )& * (volatile unsigned char *)(0x32 + 0x20);
}
inline   
uint8_t HPLClock_Clock_getInterval(void)
{
   return OCR0;
  //return * (volatile unsigned char *)(unsigned int )& * (volatile unsigned char *)(0x31 + 0x20);
}


inline  result_t HPLClock_Clock_fire(void){
  unsigned char result;
  result = TimerM_Clock_fire();
  return result;
}


inline   
void HPLClock_Clock_setInterval(uint8_t value)
{
	OCR0 = value;
  //* (volatile unsigned char *)(unsigned int )& * (volatile unsigned char *)(0x31 + 0x20) = value;
}

inline
result_t HPLClock_Clock_setRate(char interval, char scale)
{
	
  scale &= 0x7;
  scale |= 0x8;
  //shoudl be 1011 here in scale 
  { _atomic_t _atomic = _atomic_start();
    {
    	
    	
      cbi(TIMSK, TOIE0);
      cbi(TIMSK, OCIE0);     //Disable TC0 interrupt
      sbi(ASSR, AS0);        //set Timer/Counter0 to be asynchronous
      //from the CPU clock with a second external
      //clock(32,768kHz)driving it.
      outp(scale, TCCR0);    //prescale the timer to be clock/128 to make it
      outp(0, TCNT0);
      outp(interval, OCR0);
      sbi(TIMSK, OCIE0);
      
      
     /* * (volatile unsigned char *)(unsigned int )& * (volatile unsigned char *)(0x37 + 0x20) &= ~(1 << 0);
      * (volatile unsigned char *)(unsigned int )& * (volatile unsigned char *)(0x37 + 0x20) &= ~(1 << 1);
      * (volatile unsigned char *)(unsigned int )& * (volatile unsigned char *)(0x30 + 0x20) |= 1 << 3;


      * (volatile unsigned char *)(unsigned int )& * (volatile unsigned char *)(0x33 + 0x20) = scale;
      * (volatile unsigned char *)(unsigned int )& * (volatile unsigned char *)(0x32 + 0x20) = 0;
      * (volatile unsigned char *)(unsigned int )& * (volatile unsigned char *)(0x31 + 0x20) = interval;
      * (volatile unsigned char *)(unsigned int )& * (volatile unsigned char *)(0x37 + 0x20) |= 1 << 1;
      */
    }
    _atomic_end(_atomic); }
  return SUCCESS;
}




_INTERRUPT(SIG_OUTPUT_COMPARE0)
{

  _atomic_t _atomic;
  uint8_t isthreadtrue; 
  
   
  isthreadtrue = 0;
  
   _atomic = _atomic_start();
  if (is_thread())
   {
   isthreadtrue = 1; 
   SWAP_STACK_PTR(stackinterrupt_ptr, old_stack_ptr);  }
   _atomic_end(_atomic);


  { _atomic = _atomic_start();
   

    {
      if (HPLClock_set_flag) {
          HPLClock_mscale = HPLClock_nextScale;
          HPLClock_nextScale |= 0x8;
          
          outp(HPLClock_nextScale, TCCR0);
	
	        outp(HPLClock_minterval, OCR0);
	
          HPLClock_set_flag = 0;
        }
    }
    _atomic_end(_atomic); }
  HPLClock_Clock_fire();

  _atomic = _atomic_start();
 if (isthreadtrue == 1)
 {
  isthreadtrue = 0; 
  SWAP_STACK_PTR(old_stack_ptr, stackinterrupt_ptr);  
  //thread_yield();
 }
 _atomic_end(_atomic); 

}

//_INTERRUPT(SIG_OUTPUT_COMPARE0)
//void __vector_15() __attribute__ ((interrupt, spontaneous, C))
//void __attribute((interrupt))   __vector_15(void)




