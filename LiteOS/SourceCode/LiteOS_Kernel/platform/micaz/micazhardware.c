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
 
#include "micazhardware.h"

void inline LITE_wait_250ns() {
   asm volatile( "nop":: );
   asm volatile( "nop":: );
}

//-------------------------------------------------------------------------
void inline LITE_uwait( int u_sec ) {
   while ( u_sec > 0 ) {
      asm volatile( "nop":: );
      asm volatile( "nop":: );
      asm volatile( "nop":: );
      asm volatile( "nop":: );
      asm volatile( "nop":: );
      asm volatile( "nop":: );
      asm volatile( "nop":: );
      asm volatile( "nop":: );
      u_sec --;
   }
}
// LED assignments
LITE_ASSIGN_PIN( RED_LED, A, 2 );
LITE_ASSIGN_PIN( GREEN_LED, A, 1 );
LITE_ASSIGN_PIN( YELLOW_LED, A, 0 );
LITE_ASSIGN_PIN( SERIAL_ID, A, 4 );
LITE_ASSIGN_PIN( BAT_MON, A, 5 );
LITE_ASSIGN_PIN( THERM_PWR, A, 7 );

void inline CC2420_FIFOP_INT_MODE( bool LowToHigh ) {
   sbi( EICRB, ISC61 ); // edge mode
   if ( LowToHigh ) {
      sbi( EICRB, ISC60 );
   }  //trigger on rising level
   else {
      cbi( EICRB, ISC60 );
   }  //trigger on falling level
}

//-------------------------------------------------------------------------
LITE_ASSIGN_PIN( CC_RSTN, A, 6 ); // chipcon reset
LITE_ASSIGN_PIN( CC_VREN, A, 5 ); // chipcon power enable
//LITE_ASSIGN_PIN(CC_FIFOP1, D, 7);  // fifo interrupt
LITE_ASSIGN_PIN( CC_FIFOP, E, 6 ); // fifo interrupt
LITE_ASSIGN_PIN( CC_FIFOP1, E, 6 ); // fifo interrupt
LITE_ASSIGN_PIN( CC_CCA, D, 6 ); // 
LITE_ASSIGN_PIN( CC_SFD, D, 4 ); // chipcon packet arrival
LITE_ASSIGN_PIN( CC_CS, B, 0 ); // chipcon enable
LITE_ASSIGN_PIN( CC_FIFO, B, 7 ); // chipcon fifo
LITE_ASSIGN_PIN( RADIO_CCA, D, 6 ); // 
// Flash assignments
LITE_ASSIGN_PIN( FLASH_SELECT, A, 3 );
LITE_ASSIGN_PIN( FLASH_CLK, D, 5 );
LITE_ASSIGN_PIN( FLASH_OUT, D, 3 );
LITE_ASSIGN_PIN( FLASH_IN, D, 2 );
// interrupt assignments
LITE_ASSIGN_PIN( INT0, E, 4 );
LITE_ASSIGN_PIN( INT1, E, 5 );
LITE_ASSIGN_PIN( INT2, E, 6 );
LITE_ASSIGN_PIN( INT3, E, 7 );
// spibus assignments 
LITE_ASSIGN_PIN( MOSI, B, 2 );
LITE_ASSIGN_PIN( MISO, B, 3 );
//LITE_ASSIGN_PIN(SPI_OC1C, B, 7);
LITE_ASSIGN_PIN( SPI_SCK, B, 1 );
// power control assignments
LITE_ASSIGN_PIN( PW0, C, 0 );
LITE_ASSIGN_PIN( PW1, C, 1 );
LITE_ASSIGN_PIN( PW2, C, 2 );
LITE_ASSIGN_PIN( PW3, C, 3 );
LITE_ASSIGN_PIN( PW4, C, 4 );
LITE_ASSIGN_PIN( PW5, C, 5 );
LITE_ASSIGN_PIN( PW6, C, 6 );
LITE_ASSIGN_PIN( PW7, C, 7 );
// i2c bus assignments
LITE_ASSIGN_PIN( I2C_BUS1_SCL, D, 0 );
LITE_ASSIGN_PIN( I2C_BUS1_SDA, D, 1 );
// uart assignments
LITE_ASSIGN_PIN( UART_RXD0, E, 0 );
LITE_ASSIGN_PIN( UART_TXD0, E, 1 );
LITE_ASSIGN_PIN( UART_XCK0, E, 2 );
LITE_ASSIGN_PIN( AC_NEG, E, 3 ); // RFID Reader Red LED
LITE_ASSIGN_PIN( UART_RXD1, D, 2 );
LITE_ASSIGN_PIN( UART_TXD1, D, 3 );
LITE_ASSIGN_PIN( UART_XCK1, D, 5 );
LITE_ALIAS_PIN( TONE_DECODE_SIGNAL, INT3 );
LITE_ALIAS_PIN( PHOTO_CTL, INT1 );
LITE_ALIAS_PIN( TEMP_CTL, INT2 );
LITE_ALIAS_OUTPUT_ONLY_PIN( MIC_CTL, PW3 );
LITE_ALIAS_OUTPUT_ONLY_PIN( SOUNDER_CTL, PW2 );
LITE_ALIAS_OUTPUT_ONLY_PIN( ACCEL_CTL, PW4 );
LITE_ALIAS_OUTPUT_ONLY_PIN( MAG_CTL, PW5 );
LITE_ALIAS_OUTPUT_ONLY_PIN( MIC_MUX_SEL, PW6 );
void LITE_SET_PIN_DIRECTIONS( void ) {
   /*  outp(0x00, DDRA);
   outp(0x00, DDRB);
   outp(0x00, DDRD);
   outp(0x02, DDRE);
   outp(0x02, PORTE);
    */
   LITE_MAKE_RED_LED_OUTPUT();
   LITE_MAKE_YELLOW_LED_OUTPUT();
   LITE_MAKE_GREEN_LED_OUTPUT();
   LITE_MAKE_PW7_OUTPUT();
   LITE_MAKE_PW6_OUTPUT();
   LITE_MAKE_PW5_OUTPUT();
   LITE_MAKE_PW4_OUTPUT();
   LITE_MAKE_PW3_OUTPUT();
   LITE_MAKE_PW2_OUTPUT();
   LITE_MAKE_PW1_OUTPUT();
   LITE_MAKE_PW0_OUTPUT();
   //CC2420 pins  
   LITE_MAKE_MISO_INPUT();
   LITE_MAKE_MOSI_OUTPUT();
   LITE_MAKE_SPI_SCK_OUTPUT();
   LITE_MAKE_CC_RSTN_OUTPUT();
   LITE_MAKE_CC_VREN_OUTPUT();
   LITE_MAKE_CC_CS_INPUT();
   LITE_MAKE_CC_FIFOP1_INPUT();
   LITE_MAKE_CC_CCA_INPUT();
   LITE_MAKE_CC_SFD_INPUT();
   LITE_MAKE_CC_FIFO_INPUT();
   LITE_MAKE_RADIO_CCA_INPUT();
   LITE_MAKE_SERIAL_ID_INPUT();
   LITE_CLR_SERIAL_ID_PIN(); // Prevent sourcing current
   LITE_MAKE_FLASH_SELECT_OUTPUT();
   LITE_MAKE_FLASH_OUT_OUTPUT();
   LITE_MAKE_FLASH_CLK_OUTPUT();
   LITE_SET_FLASH_SELECT_PIN();
   LITE_SET_RED_LED_PIN();
   LITE_SET_YELLOW_LED_PIN();
   LITE_SET_GREEN_LED_PIN();
}
