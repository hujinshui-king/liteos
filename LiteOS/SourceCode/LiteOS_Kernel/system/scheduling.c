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
 * Authors:		Jason Hill, Philip Levis
 * Revision:		$Id: sched.c,v 1.8 2005/06/20 22:08:24 idgay Exp $
 * Modifications:       Removed unecessary code, cleanup.(5/30/02)
 *
 *                      Moved from non-blocking list to simple
 *                      critical section.  Changed task queue to
 *                      length 8 (more efficient). (3/10/02)
 */



#include "scheduling.h"
#include "nodeconfig.h"
#include "stdserial.h"


typedef struct {
   void( *tp )( void );
   uint8_t priority;
} LITE_sched_entry_T;


enum {
   LITE_MAX_TASKS = 16, LITE_TASK_BITMASK = ( LITE_MAX_TASKS - 1 )
};

volatile LITE_sched_entry_T LITE_queue[ LITE_MAX_TASKS ];


volatile uint8_t LITE_sched_num;
 
 
#ifdef PLATFORM_CPU_MEASURE
uint32_t cpucounter;
#endif  

void initScheduling( void ) {
   int i;
   LITE_sched_num = 0;
   for ( i = 0; i < LITE_MAX_TASKS; i ++ ) {
      LITE_queue[ i ].tp = ( void* )0;
   } 
   
   #ifdef PLATFORM_CPU_MEASURE
 	 cpucounter = 0; 
	 #endif  

}

//-------------------------------------------------------------------------
bool postTask( void( *tp )( void ), uint8_t priority ) {

   _atomic_t fInterruptFlags;

   uint8_t tmp;

   fInterruptFlags = _atomic_start();
   if ( LITE_sched_num == LITE_MAX_TASKS )
    {
      _atomic_end( fInterruptFlags );
      return FALSE;
   }
   for ( tmp = 0; tmp < LITE_MAX_TASKS; tmp ++ ) {
      if ( LITE_queue[ tmp ].tp == NULL ) {
         LITE_queue[ tmp ].tp = tp;
         LITE_queue[ tmp ].priority = priority;
         LITE_sched_num ++;
         _atomic_end( fInterruptFlags );
         return TRUE;
      }
   }
   _atomic_end( fInterruptFlags );
   return FALSE;
}


//-------------------------------------------------------------------------
bool runNextTask() {
   _atomic_t fInterruptFlags;
   uint8_t currentpriority;
   uint8_t tmp;
   uint8_t reserved;
   
   void( *func )( void );
   reserved = 0;
   fInterruptFlags = _atomic_start();
   currentpriority = 0;
   func = NULL;
   
   if ( LITE_sched_num == 0 )
    {
      _atomic_end( fInterruptFlags );
      _enable_interrupt(); 
       
       #ifdef PLATFORM_CPU_MEASURE
 	     cpucounter ++;
 	     #else
	      nodeSleep(); 
       //   printfstr("ABOUT TO SLEEP!!\n");
          
	     #endif
	     
      return FALSE;
   }
   for ( tmp = 0; tmp < LITE_MAX_TASKS; tmp ++ ) {
      if (( LITE_queue[ tmp ].tp != NULL ) && ( LITE_queue[ tmp ].priority > currentpriority )) {
         func = LITE_queue[ tmp ].tp;
         reserved = tmp;
         currentpriority = LITE_queue[ tmp ].priority;
      }
   }
   if ( func != NULL ) {
      LITE_sched_num --;
      LITE_queue[ reserved ].tp = NULL;
   }
   _atomic_end( fInterruptFlags );
   if ( func != NULL ) {
      func();
   } 
   return TRUE;
}
