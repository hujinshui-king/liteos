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
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
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
 
 
#include "../avr/avrhardware.h"
#include "micazhardware.h"
#include "../../system/scheduling.h"
#include "leds.h"
//#include "Timer.h"
#include "../../system/amcommon.h"
#include "../../system/amradio.h"
#include "../../types/byteorder.h"
#include "../../io/cc2420/cc2420const.h"
#include "../../io/cc2420/cc2420controlm.h"
#include "../../io/cc2420/cc2420radiom.h"
#include "hplcc2420fifom.h"
#include "hplcc2420interruptm.h"
#include "hplcc2420m.h"
#include "hpltimer1.h"
#include "../../types/types.h"

bool HPLCC2420M_bSpiAvail;
uint8_t *HPLCC2420M_rambuf;
uint8_t HPLCC2420M_ramlen;
uint16_t HPLCC2420M_ramaddr;
//set up basic settings

inline result_t HPLCC2420M_StdControl_init( void ) {
   HPLCC2420M_bSpiAvail = TRUE;
   LITE_MAKE_MISO_INPUT();
   LITE_MAKE_MOSI_OUTPUT();
   LITE_MAKE_SPI_SCK_OUTPUT();
   LITE_MAKE_CC_RSTN_OUTPUT();
   LITE_MAKE_CC_VREN_OUTPUT();
   LITE_MAKE_CC_CS_OUTPUT();
   LITE_MAKE_CC_FIFOP1_INPUT();
   LITE_MAKE_CC_CCA_INPUT();
   LITE_MAKE_CC_SFD_INPUT();
   LITE_MAKE_CC_FIFO_INPUT();
    {
      _atomic_t _atomic = _atomic_start();
       {
         LITE_MAKE_SPI_SCK_OUTPUT();
         LITE_MAKE_MISO_INPUT();
         LITE_MAKE_MOSI_OUTPUT();
         sbi( SPSR, SPI2X ); // Double speed spi clock
         sbi( SPCR, MSTR ); // Set master mode
         cbi( SPCR, CPOL ); // Set proper polarity...
         cbi( SPCR, CPHA ); // ...and phase
         cbi( SPCR, SPR1 ); // set clock, fosc/2 (~3.6 Mhz)
         cbi( SPCR, SPR0 );
         //    sbi(SPCR, SPIE);	           // enable spi port interrupt
         sbi( SPCR, SPE ); // enable spie port
      }
      _atomic_end( _atomic );
   }
   return SUCCESS;
}

//-------------------------------------------------------------------------
inline result_t HPLCC2420M_HPLCC2420RAM_writeDone( uint16_t arg_0xa45b460, uint8_t arg_0xa45b5a8, uint8_t *arg_0xa45b708 ) {
   unsigned char result;
   result = cc2420controlm_HPLChipconRAM_writeDone( arg_0xa45b460, arg_0xa45b5a8, arg_0xa45b708 );
   return result;
}

//-------------------------------------------------------------------------
inline 
void HPLCC2420M_signalRAMWr( void )
 {
   HPLCC2420M_HPLCC2420RAM_writeDone( HPLCC2420M_ramaddr, HPLCC2420M_ramlen, HPLCC2420M_rambuf );
}

//-------------------------------------------------------------------------
inline result_t HPLCC2420M_HPLCC2420RAM_write( uint16_t addr, uint8_t length, uint8_t *buffer )
 {
   uint8_t i = 0;
   uint8_t status;
   if (  ! HPLCC2420M_bSpiAvail ) {
      return FALSE;
   }
    {
      _atomic_t _atomic = _atomic_start();
       {
         HPLCC2420M_bSpiAvail = FALSE;
         HPLCC2420M_ramaddr = addr;
         HPLCC2420M_ramlen = length;
         HPLCC2420M_rambuf = buffer;
         LITE_CLR_CC_CS_PIN();
         outp((( HPLCC2420M_ramaddr& 0x7F ) | 0x80 ), SPDR ); //ls address	and set RAM/Reg flagbit
         while (  ! ( inp( SPSR )& 0x80 )){}
         ; //wait for spi xfr to complete
         status = inp( SPDR );
         outp((( HPLCC2420M_ramaddr >> 1 )& 0xC0 ), SPDR ); //ms address
         while (  ! ( inp( SPSR )& 0x80 )){}
         ; //wait for spi xfr to complete
         status = inp( SPDR );
         for ( i = 0; i < HPLCC2420M_ramlen; i ++ ) {
            //buffer write
            outp( HPLCC2420M_rambuf[ i ], SPDR );
            //        call USARTControl.tx(rambuf[i]);
            while (  ! ( inp( SPSR )& 0x80 )){}
            ; //wait for spi xfr to complete
         }
      }
      _atomic_end( _atomic );
   }
   HPLCC2420M_bSpiAvail = TRUE;
   return postTask( HPLCC2420M_signalRAMWr, 5 );
}

//-------------------------------------------------------------------------
inline result_t HPLCC2420M_HPLCC2420_write( uint8_t addr, uint16_t data )
 {
   uint8_t status;
    {
      _atomic_t _atomic = _atomic_start();
       {
         HPLCC2420M_bSpiAvail = FALSE;
         LITE_CLR_CC_CS_PIN();
         outp( addr, SPDR );
         while (  ! ( inp( SPSR )& 0x80 )){}
         ; //wait for spi xfr to complete
         status = inp( SPDR );
         if ( addr > CC2420_SAES ) {
            outp( data >> 8, SPDR );
            while (  ! ( inp( SPSR )& 0x80 )){}
            ; //wait for spi xfr to complete
            outp( data& 0xff, SPDR );
            while (  ! ( inp( SPSR )& 0x80 )){}
            ; //wait for spi xfr to complete
         }
         HPLCC2420M_bSpiAvail = TRUE;
      }
      _atomic_end( _atomic );
   }
   LITE_SET_CC_CS_PIN(); //disable chip select
   return status;
}

//-------------------------------------------------------------------------
inline uint8_t HPLCC2420M_HPLCC2420_cmd( uint8_t addr )
 {
   uint8_t status;
    {
      _atomic_t _atomic = _atomic_start();
       {
         LITE_CLR_CC_CS_PIN();
         outp( addr, SPDR );
         while (  ! ( inp( SPSR )& 0x80 )){}
         ; //wait for spi xfr to complete
         status = inp( SPDR );
      }
      _atomic_end( _atomic );
   }
   LITE_SET_CC_CS_PIN();
   return status;
}

//-------------------------------------------------------------------------
inline uint16_t HPLCC2420M_HPLCC2420_read( uint8_t addr )
 {
   uint16_t data = 0;
   uint8_t status;
    {
      _atomic_t _atomic = _atomic_start();
       {
         HPLCC2420M_bSpiAvail = FALSE;
         LITE_CLR_CC_CS_PIN(); //enable chip select
         outp( addr | 0x40, SPDR );
         while (  ! ( inp( SPSR )& 0x80 )){}
         ; //wait for spi xfr to complete
         status = inp( SPDR );
         outp( 0, SPDR );
         while (  ! ( inp( SPSR )& 0x80 )){}
         ; //wait for spi xfr to complete
         data = inp( SPDR );
         outp( 0, SPDR );
         while (  ! ( inp( SPSR )& 0x80 )){}
         ; //wait for spi xfr to complete
         data = ( data << 8 ) | inp( SPDR );
         LITE_SET_CC_CS_PIN(); //disable chip select
         HPLCC2420M_bSpiAvail = TRUE;
      }
      _atomic_end( _atomic );
   }
   return data;
}
