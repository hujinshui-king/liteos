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
bool hplcc2420fifom_bSpiAvail;
uint8_t *hplcc2420fifom_txbuf;
uint8_t *hplcc2420fifom_rxbuf;
uint8_t hplcc2420fifom_txlength;
uint8_t hplcc2420fifom_rxlength;
bool hplcc2420fifom_rxbufBusy;
bool hplcc2420fifom_txbufBusy;
inline result_t hplcc2420fifom_HPLCC2420FIFO_TXFIFODone( uint8_t arg_0xa40cd20, uint8_t *arg_0xa40ce80 ) {
   unsigned char result;
   result = cc2420radiom_HPLChipconFIFO_TXFIFODone( arg_0xa40cd20, arg_0xa40ce80 );
   return result;
}

//-------------------------------------------------------------------------
inline 
void hplcc2420fifom_signalTXdone( void )
 {
   uint8_t _txlen;
   uint8_t *_txbuf;
    {
      _atomic_t _atomic = _atomic_start();
       {
         _txlen = hplcc2420fifom_txlength;
         _txbuf = hplcc2420fifom_txbuf;
         hplcc2420fifom_txbufBusy = FALSE;
      }
      _atomic_end( _atomic );
   }
   hplcc2420fifom_HPLCC2420FIFO_TXFIFODone( _txlen, _txbuf );
}

//-------------------------------------------------------------------------
inline 
result_t hplcc2420fifom_HPLCC2420FIFO_writeTXFIFO( uint8_t len, uint8_t *msg )
 {
   uint8_t i = 0;
   uint8_t status;
   bool returnFail = FALSE;
    {
      _atomic_t _atomic = _atomic_start();
       {
         if ( hplcc2420fifom_txbufBusy ) {
            returnFail = TRUE;
         } else {
            hplcc2420fifom_txbufBusy = TRUE;
         }
      }
      _atomic_end( _atomic );
   }
   if ( returnFail ) {
      return FAIL;
   }
    {
      _atomic_t _atomic = _atomic_start();
       {
         hplcc2420fifom_bSpiAvail = FALSE;
         hplcc2420fifom_txlength = len;
         hplcc2420fifom_txbuf = msg;
         LITE_CLR_CC_CS_PIN();
         outp( CC2420_TXFIFO, SPDR );
         while (  ! ( inp( SPSR )& 0x80 )){}
         ; //wait for spi xfr to complete
         status = inp( SPDR );
         for ( i = 0; i < hplcc2420fifom_txlength; i ++ ) {
            outp( *hplcc2420fifom_txbuf, SPDR );
            hplcc2420fifom_txbuf ++;
            while (  ! ( inp( SPSR )& 0x80 )){}
            ; //wait for spi xfr to complete
         }
         hplcc2420fifom_bSpiAvail = TRUE;
      }
      _atomic_end( _atomic );
   }
   LITE_SET_CC_CS_PIN();
   if ( postTask( hplcc2420fifom_signalTXdone, 5 ) == FAIL ) {
       {
         _atomic_t _atomic = _atomic_start();
         hplcc2420fifom_txbufBusy = FALSE;
         _atomic_end( _atomic );
      }
      return FAIL;
   }
   return status;
}

//-------------------------------------------------------------------------
inline result_t hplcc2420fifom_HPLCC2420FIFO_RXFIFODone( uint8_t arg_0xa40c690, uint8_t *arg_0xa40c7f0 ) {
   unsigned char result;
   result = cc2420radiom_HPLChipconFIFO_RXFIFODone( arg_0xa40c690, arg_0xa40c7f0 );
   return result;
}

//-------------------------------------------------------------------------
inline 
void hplcc2420fifom_signalRXdone( void )
 {
   uint8_t _rxlen;
   uint8_t *_rxbuf;
    {
      _atomic_t _atomic = _atomic_start();
       {
         _rxlen = hplcc2420fifom_rxlength;
         _rxbuf = hplcc2420fifom_rxbuf;
         hplcc2420fifom_rxbufBusy = FALSE;
      }
      _atomic_end( _atomic );
   }
   hplcc2420fifom_HPLCC2420FIFO_RXFIFODone( _rxlen, _rxbuf );
}

//-------------------------------------------------------------------------
inline 
//Seems that the transmitted len is the actual length over the air 
//The first number in the frame is the remaining part 
result_t hplcc2420fifom_HPLCC2420FIFO_readRXFIFO( uint8_t len, uint8_t *msg )
 {
   uint8_t status;
   uint8_t i;
   bool returnFail = FALSE;
    {
      _atomic_t _atomic = _atomic_start();
       {
         if ( hplcc2420fifom_rxbufBusy ) {
            returnFail = TRUE;
         } else {
            hplcc2420fifom_rxbufBusy = TRUE;
         }
      }
      _atomic_end( _atomic );
   }
   if ( returnFail ) {
      return FAIL;
   }
    {
      _atomic_t _atomic = _atomic_start();
       {
         hplcc2420fifom_bSpiAvail = FALSE;
         hplcc2420fifom_rxbuf = msg;
         LITE_CLR_CC_CS_PIN();
         outp( CC2420_RXFIFO | 0x40, SPDR ); //output Rxfifo address
         while (  ! ( inp( SPSR )& 0x80 )){}
         ; //wait for spi xfr to complete
         status = inp( SPDR );
         outp( 0, SPDR );
         while (  ! ( inp( SPSR )& 0x80 )){}
         ; //wait for spi xfr to complete
         hplcc2420fifom_rxlength = inp( SPDR );
         //My revise
         //The first byte, however, only contains a part of the whole, which needs to add 1 to mean that it is the total to be read 
         //and this also defaults to len, which is the MSG_DATA_SIZE
         if ( hplcc2420fifom_rxlength > 0 ) {
            hplcc2420fifom_rxbuf[ 0 ] = hplcc2420fifom_rxlength;
            hplcc2420fifom_rxlength ++;
            if ( hplcc2420fifom_rxlength > len ) {
               hplcc2420fifom_rxlength = len;
            }
            for ( i = 1; i < hplcc2420fifom_rxlength; i ++ ) {
               outp( 0, SPDR );
               while (  ! ( inp( SPSR )& 0x80 )){}
               ; //wait for spi xfr to complete
               hplcc2420fifom_rxbuf[ i ] = inp( SPDR );
            }
         }
         hplcc2420fifom_bSpiAvail = TRUE;
      }
      _atomic_end( _atomic );
   }
   LITE_SET_CC_CS_PIN();
   if ( postTask( hplcc2420fifom_signalRXdone, 5 ) == FAIL ) {
       {
         _atomic_t _atomic = _atomic_start();
         hplcc2420fifom_rxbufBusy = FALSE;
         _atomic_end( _atomic );
      }
      return FAIL;
   }
   return SUCCESS;
}
