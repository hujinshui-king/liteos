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
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * 
 */
 



#ifndef LITE_HARDWARE_H
#define LITE_HARDWARE_H


#include "../avr/avrhardware.h"
#include "micazhardware.h"
#include "../../types/types.h"




#define LITE_CYCLE_TIME_NS 136


void inline LITE_wait_250ns(void);

void inline LITE_uwait(int u_sec);


// LED assignments
LITE_ASSIGN_PIN_H(RED_LED, A, 2);
LITE_ASSIGN_PIN_H(GREEN_LED, A, 1);
LITE_ASSIGN_PIN_H(YELLOW_LED, A, 0);

LITE_ASSIGN_PIN_H(SERIAL_ID, A, 4);
LITE_ASSIGN_PIN_H(BAT_MON, A, 5);
LITE_ASSIGN_PIN_H(THERM_PWR, A, 7);

// ChipCon control assignments

#define LITE_CC_FIFOP_INT SIG_INTERRUPT6
// CC2420 Interrupt definition
#define CC2420_FIFOP_INT_ENABLE()  sbi(EIMSK , INT6)
#define CC2420_FIFOP_INT_DISABLE() cbi(EIMSK , INT6)
#define CC2420_FIFOP_INT_CLEAR() sbi(EIFR, INTF6)

void inline CC2420_FIFOP_INT_MODE(bool LowToHigh);


LITE_ASSIGN_PIN_H(CC_RSTN, A, 6); // chipcon reset
LITE_ASSIGN_PIN_H(CC_VREN, A, 5); // chipcon power enable
//LITE_ASSIGN_PIN_H(CC_FIFOP1, D, 7);  // fifo interrupt
LITE_ASSIGN_PIN_H(CC_FIFOP, E, 6);  // fifo interrupt
LITE_ASSIGN_PIN_H(CC_FIFOP1, E, 6);  // fifo interrupt

LITE_ASSIGN_PIN_H(CC_CCA, D, 6);	  // 
LITE_ASSIGN_PIN_H(CC_SFD, D, 4);	  // chipcon packet arrival
LITE_ASSIGN_PIN_H(CC_CS, B, 0);	  // chipcon enable
LITE_ASSIGN_PIN_H(CC_FIFO, B, 7);	  // chipcon fifo

LITE_ASSIGN_PIN_H(RADIO_CCA, D, 6);	  // 

// Flash assignments
LITE_ASSIGN_PIN_H(FLASH_SELECT, A, 3);
LITE_ASSIGN_PIN_H(FLASH_CLK,  D, 5);
LITE_ASSIGN_PIN_H(FLASH_OUT,  D, 3);
LITE_ASSIGN_PIN_H(FLASH_IN,  D, 2);

// interrupt assignments
LITE_ASSIGN_PIN_H(INT0, E, 4);
LITE_ASSIGN_PIN_H(INT1, E, 5);
LITE_ASSIGN_PIN_H(INT2, E, 6);
LITE_ASSIGN_PIN_H(INT3, E, 7);

// spibus assignments 
LITE_ASSIGN_PIN_H(MOSI,  B, 2);
LITE_ASSIGN_PIN_H(MISO,  B, 3);
//LITE_ASSIGN_PIN_H(SPI_OC1C, B, 7);
LITE_ASSIGN_PIN_H(SPI_SCK,  B, 1);

// power control assignments
LITE_ASSIGN_PIN_H(PW0, C, 0);
LITE_ASSIGN_PIN_H(PW1, C, 1);
LITE_ASSIGN_PIN_H(PW2, C, 2);
LITE_ASSIGN_PIN_H(PW3, C, 3);
LITE_ASSIGN_PIN_H(PW4, C, 4);
LITE_ASSIGN_PIN_H(PW5, C, 5);
LITE_ASSIGN_PIN_H(PW6, C, 6);
LITE_ASSIGN_PIN_H(PW7, C, 7);

// i2c bus assignments
LITE_ASSIGN_PIN_H(I2C_BUS1_SCL, D, 0);
LITE_ASSIGN_PIN_H(I2C_BUS1_SDA, D, 1);

// uart assignments
LITE_ASSIGN_PIN_H(UART_RXD0, E, 0);
LITE_ASSIGN_PIN_H(UART_TXD0, E, 1);
LITE_ASSIGN_PIN_H(UART_XCK0, E, 2);
LITE_ASSIGN_PIN_H(AC_NEG, E, 3);  // RFID Reader Red LED

LITE_ASSIGN_PIN_H(UART_RXD1, D, 2);
LITE_ASSIGN_PIN_H(UART_TXD1, D, 3);
LITE_ASSIGN_PIN_H(UART_XCK1, D, 5);

void LITE_SET_PIN_DIRECTIONS(void);





#endif //LITE_HARDWARE_H




