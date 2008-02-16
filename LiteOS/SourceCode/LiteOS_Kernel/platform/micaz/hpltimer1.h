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
 

#ifndef HPLTIMERH
#define HPLTIMERH


#include "../../types/types.h"
#include "../../system/amcommon.h"

/* \ingroup clock */
/* @{ */

enum __nesc_unnamed4266 {
  TCLK_CPU_OFF = 0, 
  TCLK_CPU_DIV1 = 1, 
  TCLK_CPU_DIV8 = 2, 
  TCLK_CPU_DIV64 = 3, 
  TCLK_CPU_DIV256 = 4, 
  TCLK_CPU_DIV1024 = 5
};
enum __nesc_unnamed4267 {
  TIMER1_DEFAULT_SCALE = TCLK_CPU_DIV64, 
  TIMER1_DEFAULT_INTERVAL = 255
};


/* @} */


/* \addtogroup clock */
/* @{ */

inline result_t HPLTimer1M_StdControl_init(void);
inline result_t HPLTimer1M_StdControl_start(void);
inline result_t HPLTimer1M_StdControl_stop(void);
inline result_t HPLTimer1M_Timer1_setRate(uint16_t interval, char scale);
inline void HPLTimer1M_CaptureT1_enableEvents(void);
inline void HPLTimer1M_CaptureT1_clearOverflow(void);
inline void HPLTimer1M_CaptureT1_disableEvents(void);
inline result_t HPLTimer1M_Timer1_default_fire(void);
inline result_t HPLTimer1M_Timer1_fire(void);
inline uint16_t HPLTimer1M_CaptureT1_getEvent(void);
inline void HPLTimer1M_CaptureT1_captured(uint16_t arg_0xa4d7ac0);
inline bool HPLTimer1M_CaptureT1_isOverflowPending(void);
inline void HPLTimer1M_CaptureT1_setEdge(uint8_t LowToHigh);

/*@}*/
#endif 

