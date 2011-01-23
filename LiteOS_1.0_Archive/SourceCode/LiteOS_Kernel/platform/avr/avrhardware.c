/* The LiteOS Operating System Kernel */
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
/*                                                                      tab:4
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
 * Authors:             Jason Hill, Philip Levis, Nelson Lee
 *
 *
 */

/**
* @author Jason Hill
* @author Philip Levis
* @author Nelson Lee
*/
#include "avrhardware.h"
void wait_cycle()
{
    asm volatile ("nop");
    asm volatile ("nop");
}

//-------------------------------------------------------------------------
inline void _atomic_sleep()
{
    /* Atomically enable interrupts and sleep */
    sei();                      // Make sure interrupts are on, so we can wake up!
    asm volatile ("sleep");

    wait_cycle();
}

//-------------------------------------------------------------------------
inline void _avr_sleep()
{
    asm volatile ("sleep");
}

//-------------------------------------------------------------------------
inline void _avr_enable_interrupt()
{
    sei();
}

//-------------------------------------------------------------------------
inline void _avr_disable_interrupt()
{
    cli();
}

//-------------------------------------------------------------------------
_atomic_t _atomic_start_avr(void)
{
    _atomic_t result = inp(SREG);

    cli();
    return result;
}

//-------------------------------------------------------------------------
void _atomic_end_avr(_atomic_t oldSreg)
{
    outp(oldSreg, SREG);
}

//-------------------------------------------------------------------------
void avr_resetNode()
{
    asm volatile ("ldi	r30, 0x00"::);
    asm volatile ("ldi	r31, 0x00"::);
    asm volatile ("icall"::);
}
