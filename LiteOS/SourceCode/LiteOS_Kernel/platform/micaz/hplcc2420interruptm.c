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
 *
 * Authors:		Jason Hill, David Gay, Philip Levis
 * Date last modified:  6/25/02
 *
 */

//This is an AM messaging layer implementation that understands multiple
// output devices.  All packets addressed to TOS_UART_ADDR are sent to the UART
// instead of the radio.


/**
 * @author Jason Hill
 * @author David Gay
 * @author Philip Levis
 */
 
 
 /*									tab:4
 *  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.  By
 *  downloading, copying, installing or using the software you agree to
 *  this license.  If you do not agree to this license, do not download,
 *  install, copy or use the software.
 *
 *  Intel Open Source License 
 *
 *  Copyright (c) 2002 Intel Corporation 
 *  All rights reserved. 
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are
 *  met:
 * 
 *	Redistributions of source code must retain the above copyright
 *  notice, this list of conditions and the following disclaimer.
 *	Redistributions in binary form must reproduce the above copyright
 *  notice, this list of conditions and the following disclaimer in the
 *  documentation and/or other materials provided with the distribution.
 *      Neither the name of the Intel Corporation nor the names of its
 *  contributors may be used to endorse or promote products derived from
 *  this software without specific prior written permission.
 *  
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 *  PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE INTEL OR ITS
 *  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 *  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  POCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * 
 */
/*
 *
 * Authors:		Joe Polastre
 * Date last modified:  $Revision: 1.6 $
 *
 * 
 */
/**
 * @author Joe Polastre
 * @author Alan Broad
 */
 
 
#include <avr/interrupt.h>

//platform independent inclusions



#include "../../system/scheduling.h"
#include "../../system/generictimer.h"
#include "../../system/amcommon.h"
#include "../../system/amradio.h"
#include "../../types/byteorder.h"
#include "../../system/threads.h"


extern volatile uint16_t *old_stack_ptr;

//platform depedent inclusions 


#include "../avr/avrhardware.h"
#include "micazhardware.h"
#include "leds.h"
#include "../../io/cc2420/cc2420const.h"
#include "../../io/cc2420/cc2420controlm.h"
#include "../../io/cc2420/cc2420radiom.h"
#include "hplcc2420fifom.h"
#include "hplcc2420interruptm.h"
#include "hplcc2420m.h"
#include "hpltimer1.h"




extern volatile uint16_t *stackinterrupt_ptr; 

uint8_t hplcc2420interruptm_FIFOWaitForState;
uint8_t hplcc2420interruptm_FIFOLastState;
uint8_t hplcc2420interruptm_CCAWaitForState;
uint8_t hplcc2420interruptm_CCALastState;


//This function is the start position of the receving pacekts
//void  __vector_7(void) __attribute__((signal, used,   externally_visible)); 
//void __attribute((signal, used, externally_visible)) __vector_7(void)

SIGNAL( INT6_vect )
 {
   
   result_t val = SUCCESS;
   uint8_t isthreadtrue;


   _atomic_t _atomic ; 

   isthreadtrue = 0; 
    
    _atomic = _atomic_start();

   if (is_thread())
   {
   isthreadtrue = 1; 
   SWAP_STACK_PTR(stackinterrupt_ptr, old_stack_ptr);  
   
   }

   _atomic_end( _atomic );

   

   val = hplcc2420interruptm_FIFOP_fired();
   if ( val == FAIL ) {
      *( volatile unsigned char* )( unsigned int )& *( volatile unsigned char* )( 0x39 + 0x20 ) &=  ~ ( 1 << 6 );
      *( volatile unsigned char* )( unsigned int )& *( volatile unsigned char* )( 0x38 + 0x20 ) |= 1 << 6;
   }

     _atomic = _atomic_start();
   
    if (isthreadtrue == 1)
   {
     isthreadtrue = 0; 
     SWAP_STACK_PTR( old_stack_ptr, stackinterrupt_ptr);
     //thread_yield();   
   }
    _atomic_end( _atomic );

	

}



//-------------------------------------------------------------------------
inline 
result_t hplcc2420interruptm_FIFOP_startWait( bool low_to_high )
 {
    {
      _atomic_t _atomic = _atomic_start();
       {
         CC2420_FIFOP_INT_MODE( low_to_high );
         *( volatile unsigned char* )( unsigned int )& *( volatile unsigned char* )( 0x39 + 0x20 ) |= 1 << 6;
      }
      _atomic_end( _atomic );
   }
   return SUCCESS;
}

//-------------------------------------------------------------------------
inline 
result_t hplcc2420interruptm_FIFOP_disable( void )
 {
   *( volatile unsigned char* )( unsigned int )& *( volatile unsigned char* )( 0x39 + 0x20 ) &=  ~ ( 1 << 6 );
   return SUCCESS;
}

//-------------------------------------------------------------------------
inline result_t hplcc2420interruptm_FIFOP_fired( void ) {
   unsigned char result;
   result = cc2420radiom_FIFOP_fired();
   return result;
}

//-------------------------------------------------------------------------
inline result_t hplcc2420interruptm_FIFOTimer_start( char arg_0xa315138, uint32_t arg_0xa315290 ) {
   unsigned char result;
   result = GenericTimerStart( 10, arg_0xa315138, arg_0xa315290 );
   return result;
}

//-------------------------------------------------------------------------
inline 
result_t hplcc2420interruptm_FIFO_default_fired( void )
 {
   return FAIL;
}

//-------------------------------------------------------------------------
inline result_t hplcc2420interruptm_FIFO_fired( void ) {
   unsigned char result;
   result = hplcc2420interruptm_FIFO_default_fired();
   return result;
}

//-------------------------------------------------------------------------
inline 
result_t hplcc2420interruptm_FIFOTimer_fired( void )
 {
   uint8_t FIFOState;
   result_t val = SUCCESS;
   FIFOState = LITE_READ_CC_FIFO_PIN();
   if ( hplcc2420interruptm_FIFOLastState != hplcc2420interruptm_FIFOWaitForState && FIFOState == hplcc2420interruptm_FIFOWaitForState ) {
      val = hplcc2420interruptm_FIFO_fired();
      if ( val == FAIL ) {
         return SUCCESS;
      }
   }
   hplcc2420interruptm_FIFOLastState = FIFOState;
   return hplcc2420interruptm_FIFOTimer_start( TIMER_ONE_SHOT, 1 );
}

//-------------------------------------------------------------------------
inline result_t hplcc2420interruptm_CCA_fired( void ) {
   unsigned char result;
   result = cc2420controlm_CCA_fired();
   return result;
}

//-------------------------------------------------------------------------
inline 
result_t hplcc2420interruptm_CCATimer_fired( void )
 {
   uint8_t CCAState;
   result_t val = SUCCESS;
   CCAState = LITE_READ_CC_CCA_PIN();
   if ( hplcc2420interruptm_CCALastState != hplcc2420interruptm_CCAWaitForState && CCAState == hplcc2420interruptm_CCAWaitForState ) {
      val = hplcc2420interruptm_CCA_fired();
      if ( val == FAIL ) {
         return SUCCESS;
      }
   }
   hplcc2420interruptm_CCALastState = CCAState;
   return hplcc2420interruptm_CCATimer_start( TIMER_ONE_SHOT, 1 );
}

//-------------------------------------------------------------------------
inline 
result_t hplcc2420interruptm_CCA_startWait( bool low_to_high )
 {
    {
      _atomic_t _atomic = _atomic_start();
      hplcc2420interruptm_CCAWaitForState = low_to_high;
      _atomic_end( _atomic );
   }
   hplcc2420interruptm_CCALastState = LITE_READ_CC_CCA_PIN();
   return hplcc2420interruptm_CCATimer_start( TIMER_ONE_SHOT, 1 );
}

//-------------------------------------------------------------------------
inline result_t hplcc2420interruptm_CCATimer_start( char arg_0xa315138, uint32_t arg_0xa315290 ) {
   unsigned char result;
   result = GenericTimerStart( 11, arg_0xa315138, arg_0xa315290 );
   return result;
}

//-------------------------------------------------------------------------
inline void hplcc2420interruptm_SFDCapture_enableEvents( void ) {
   HPLTimer1M_CaptureT1_enableEvents();
}

//-------------------------------------------------------------------------
inline void hplcc2420interruptm_SFDCapture_clearOverflow( void ) {
   HPLTimer1M_CaptureT1_clearOverflow();
}

//-------------------------------------------------------------------------
inline void hplcc2420interruptm_SFDCapture_setEdge( uint8_t arg_0xa4c5e80 ) {
   HPLTimer1M_CaptureT1_setEdge( arg_0xa4c5e80 );
}

//-------------------------------------------------------------------------
inline void hplcc2420interruptm_SFDCapture_disableEvents( void ) {
   HPLTimer1M_CaptureT1_disableEvents();
}

//-------------------------------------------------------------------------
inline 
result_t hplcc2420interruptm_SFD_enableCapture( bool low_to_high )
 {
    {
      _atomic_t _atomic = _atomic_start();
       {
         hplcc2420interruptm_SFDCapture_disableEvents();
         hplcc2420interruptm_SFDCapture_setEdge( low_to_high );
         hplcc2420interruptm_SFDCapture_clearOverflow();
         hplcc2420interruptm_SFDCapture_enableEvents();
      }
      _atomic_end( _atomic );
   }
   return SUCCESS;
}

//-------------------------------------------------------------------------
inline 
result_t hplcc2420interruptm_SFD_disable( void )
 {
   hplcc2420interruptm_SFDCapture_disableEvents();
   return SUCCESS;
}

//-------------------------------------------------------------------------
inline result_t hplcc2420interruptm_SFD_captured( uint16_t arg_0xa41e788 ) {
   unsigned char result;
   result = cc2420radiom_SFD_captured( arg_0xa41e788 );
   return result;
}

//-------------------------------------------------------------------------
inline bool hplcc2420interruptm_SFDCapture_isOverflowPending( void ) {
   unsigned char result;
   result = HPLTimer1M_CaptureT1_isOverflowPending();
   return result;
}

//-------------------------------------------------------------------------
void hplcc2420interruptm_SFDCapture_captured( uint16_t time )
 {
   result_t val = SUCCESS;
   val = hplcc2420interruptm_SFD_captured( time );
   if ( val == FAIL ) {
      hplcc2420interruptm_SFDCapture_disableEvents();
   } else {
      if ( hplcc2420interruptm_SFDCapture_isOverflowPending()) {
         hplcc2420interruptm_SFDCapture_clearOverflow();
      }
   }
}
