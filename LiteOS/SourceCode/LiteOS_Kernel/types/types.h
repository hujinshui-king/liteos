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
#ifndef TYPESH
#define TYPESH

/**\addtogroup type */

/**@{*/
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef int int16_t;
typedef unsigned int uint16_t;
typedef long int32_t;
typedef unsigned long uint32_t;
typedef long long int64_t;
typedef unsigned long long uint64_t;
typedef unsigned int size_t;
typedef unsigned char bool;
typedef unsigned char boolean;
typedef unsigned char bool2; 
typedef uint8_t result_t;
typedef uint8_t _atomic_t;

#ifndef TRUE
#define TRUE  1
#endif
#ifndef true
#define true  1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef false
#define false 0
#endif
enum
{
    FAIL = 0, SUCCESS = 1
};
enum
{
    YES = 1, NO = 0
};
inline result_t rcombine(result_t r1, result_t r2);

#ifndef NULL
#define NULL 0
#endif
enum
{
    MILLISECOND = 1, SECOND = 1000, MINUTE = 60000
};

/** @} */

/** \ingroup type */
#define isLetter(c) (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'))

/** \ingroup type */
#define isDigit(c) (('0' <= c && c <= '9'))

/** \ingroup type */
uint16_t hex2value(uint8_t hex);

/** \ingroup type */
char dec2asciichar(uint8_t value);

/** \ingroup type */
_atomic_t _atomic_start(void);

/** \ingroup type */
void _atomic_end(_atomic_t oldSreg);

#define STACK_TOP(stack)    \
(&(((uint16_t *)stack)[(sizeof(stack) / sizeof(uint16_t)) - 1]))

inline void *nmemset(void *to, int val, size_t n);

#endif
