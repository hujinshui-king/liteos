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
 * Authors:             Jason Hill, David Gay, Philip Levis, Nelson Lee
 * Date last modified:  6/25/02
 *
 */

/**
* @author Jason Hill
* @author David Gay
* @author Philip Levis
* @author Nelson Lee
*/
#include "types.h"
#ifdef PLATFORM_AVR
#include "../hardware/avrhardware.h"
#endif
inline result_t rcombine(result_t r1, result_t r2)
{
    return r1 == FAIL ? FAIL : r2;
}

//-------------------------------------------------------------------------
uint16_t hex2value(uint8_t hex)
{
    char a = (char)hex;
    uint16_t high;

    if ((a >= '0') && (a <= '9'))
    {
        high = a - '0';
    }
    else
    {
        high = a - 'A' + 10;
    }

    return high;
}

//-------------------------------------------------------------------------
char dec2asciichar(uint8_t value)
{
    if ( /*( value >= 0 ) && */ (value <= 9))
    {
        return (char)(value + 0x30);
    }
    else
    {
        return 0;
    }
}

//-------------------------------------------------------------------------
_atomic_t _atomic_start(void)
{
#ifdef PLATFORM_AVR
    _atomic_t result = _atomic_start_avr();
#endif
    return result;
}

//-------------------------------------------------------------------------
void _atomic_end(_atomic_t oldSreg)
{
#ifdef PLATFORM_AVR
    _atomic_end_avr(oldSreg);
#endif
}

//-------------------------------------------------------------------------
inline void *nmemset(void *to, int val, size_t n)
{
    char *cto = to;

    while (n--)
    {
        *cto++ = val;
    }

    return to;
}
