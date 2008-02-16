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
#include "../../platform/avr/avrhardware.h"
#include "../../platform/micaz/micazhardware.h"
#include "../../system/scheduling.h"
#include "../../platform/micaz/leds.h"
#include "../../system/amcommon.h"
#include "../../system/amradio.h"
#include "../../types/byteorder.h"
#include "cc2420const.h"
#include "cc2420controlm.h"
#include "cc2420radiom.h"
#include "../../platform/micaz/hplcc2420fifom.h"
#include "../../platform/micaz/hplcc2420interruptm.h"
#include "../../platform/micaz/hplcc2420m.h"
#include "../../platform/micaz/hpltimer1.h"
#include "../../types/types.h"
#include "../../system/nodeconfig.h"


// added for post-compile frequency changing
uint8_t CC2420_CHANNEL;
uint8_t CC2420_RFPOWER;
uint8_t cc2420controlm_state;
uint16_t cc2420controlm_gCurrentParameters[ 14 ];
inline result_t cc2420controlm_SplitControl_init( void ) {
   uint8_t _state = FALSE;
    {
      _atomic_t _atomic = _atomic_start();
       {
         if ( cc2420controlm_state == cc2420controlm_IDLE_STATE ) {
            cc2420controlm_state = cc2420controlm_INIT_STATE;
            _state = TRUE;
         }
      }
      //Qing Revision
      //IDLE state
      cc2420controlm_state = cc2420controlm_IDLE_STATE;
      CC2420_CHANNEL = CC2420_DEF_CHANNEL;
      CC2420_RFPOWER = CC2420_DEF_RFPOWER;
      _atomic_end( _atomic );
   }
   if (  ! _state ) {
      return FAIL;
   }
   cc2420controlm_HPLChipconControl_init();
   //Basically, reset everything page 64
   cc2420controlm_gCurrentParameters[ CP_MAIN ] = 0xf800;
   //Basically, check page 65. Very easy 
   cc2420controlm_gCurrentParameters[ CP_MDMCTRL0 ] = (((( 0 << 11 ) | ( 2 << 8 )) | ( 3 << 6 )) | ( 1 << 5 )) | ( 2 << 0 );
   //Page 66
   cc2420controlm_gCurrentParameters[ CP_MDMCTRL1 ] = 20 << 6;
   //reset values
   cc2420controlm_gCurrentParameters[ CP_RSSI ] = 0xE080;
   //reset
   cc2420controlm_gCurrentParameters[ CP_SYNCWORD ] = 0xA70F;
   //the last one, according to page 52 of the data sheet and page 67, default to output 0dbm 
   cc2420controlm_gCurrentParameters[ CP_TXCTRL ] = (((( 1 << 14 ) | ( 1 << 13 )) | ( 3 << 6 )) | ( 1 << 5 )) | ( CC2420_RFPOWER << 0 );
   cc2420controlm_gCurrentParameters[ CP_RXCTRL0 ] = ((((( 1 << 12 ) | ( 2 << 8 )) | ( 3 << 6 )) | ( 2 << 4 )) | ( 1 << 2 )) | ( 1 << 0 );
   cc2420controlm_gCurrentParameters[ CP_RXCTRL1 ] = ((((( 1 << 11 ) | ( 1 << 9 )) | ( 1 << 6 )) | ( 1 << 4 )) | ( 1 << 2 )) | ( 2 << 0 );
   //PAGE 51 of the manual 
   cc2420controlm_gCurrentParameters[ CP_FSCTRL ] = ( 1 << 14 ) | (( 357+5 *( CC2420_CHANNEL - 11 )) << 0 );
   cc2420controlm_gCurrentParameters[ CP_SECCTRL0 ] = ((( 1 << 8 ) | ( 1 << 7 )) | ( 1 << 6 )) | ( 1 << 2 );
   cc2420controlm_gCurrentParameters[ CP_SECCTRL1 ] = 0;
   cc2420controlm_gCurrentParameters[ CP_BATTMON ] = 0;
   //fifop and cca polarity are inversed
   cc2420controlm_gCurrentParameters[ CP_IOCFG0 ] = ( 127 << 0 ) | ( 1 << 9 );
   cc2420controlm_gCurrentParameters[ CP_IOCFG1 ] = 0;
    {
      _atomic_t _atomic = _atomic_start();
      cc2420controlm_state = cc2420controlm_INIT_STATE_DONE;
      _atomic_end( _atomic );
   }
   postTask( cc2420controlm_taskInitDone, 5 );
   return SUCCESS;
}

//-------------------------------------------------------------------------
inline result_t cc2420controlm_CCA_startWait( bool arg_0xa422588 ) {
   unsigned char result;
   result = hplcc2420interruptm_CCA_startWait( arg_0xa422588 );
   return result;
}

//-------------------------------------------------------------------------
inline 
result_t cc2420controlm_CC2420Control_VREFOn( void )
 {
   LITE_SET_CC_VREN_PIN();
   LITE_uwait( 600 );
   return SUCCESS;
}

//-------------------------------------------------------------------------
inline 
result_t cc2420controlm_CC2420Control_VREFOff( void )
 {
   LITE_CLR_CC_VREN_PIN();
   LITE_uwait( 600 );
   return SUCCESS;
}

//-------------------------------------------------------------------------
inline result_t cc2420controlm_SplitControl_initDone( void ) {
   unsigned char result;
   result = cc2420radiom_CC2420SplitControl_initDone();
   return result;
}

//-------------------------------------------------------------------------
inline 
void cc2420controlm_taskInitDone( void )
 {
   cc2420controlm_SplitControl_initDone();
}

//-------------------------------------------------------------------------
inline uint8_t cc2420controlm_HPLChipcon_cmd( uint8_t arg_0xa403928 ) {
   unsigned char result;
   result = HPLCC2420M_HPLCC2420_cmd( arg_0xa403928 );
   return result;
}

//-------------------------------------------------------------------------
inline uint8_t cc2420controlm_HPLChipcon_write( uint8_t arg_0xa403d80, uint16_t arg_0xa403ed0 ) {
   unsigned char result;
   result = HPLCC2420M_HPLCC2420_write( arg_0xa403d80, arg_0xa403ed0 );
   return result;
}

//-------------------------------------------------------------------------
inline 
result_t cc2420controlm_CC2420Control_OscillatorOn( void )
 {
   uint16_t i;
   uint8_t status;
   i = 0;
   cc2420controlm_HPLChipcon_write( 0x1D, 24 );
   cc2420controlm_CCA_startWait( TRUE );
   status = cc2420controlm_HPLChipcon_cmd( 0x01 );
   return SUCCESS;
}

//-------------------------------------------------------------------------
inline result_t cc2420controlm_HPLChipconControl_start( void ) {
   unsigned char result;
   result = HPLTimer1M_StdControl_start();
   return result;
}

//-------------------------------------------------------------------------
inline 
result_t cc2420controlm_SplitControl_start( void )
 {
   result_t status;
   uint8_t _state = FALSE;
    {
      _atomic_t _atomic = _atomic_start();
       {
         if ( cc2420controlm_state == cc2420controlm_INIT_STATE_DONE ) {
            cc2420controlm_state = cc2420controlm_START_STATE;
            _state = TRUE;
         }
      }
      _atomic_end( _atomic );
   }
   if (  ! _state ) {
      return FAIL;
   }
   cc2420controlm_HPLChipconControl_start();
   cc2420controlm_CC2420Control_VREFOn();
   LITE_CLR_CC_RSTN_PIN();
   wait_cycle();
   LITE_SET_CC_RSTN_PIN();
   wait_cycle();
   status = cc2420controlm_CC2420Control_OscillatorOn();
   return status;
}

//-------------------------------------------------------------------------
inline 
result_t cc2420controlm_CC2420Control_RxMode( void )
 {
   cc2420controlm_HPLChipcon_cmd( 0x03 );
   return SUCCESS;
}

//-------------------------------------------------------------------------
inline result_t cc2420controlm_SplitControl_startDone( void ) {
   unsigned char result;
   result = cc2420radiom_CC2420SplitControl_startDone();
   return result;
}

//-------------------------------------------------------------------------
inline 
result_t cc2420controlm_CC2420Control_TuneManual( uint16_t DesiredFreq )
 {
   int fsctrl;
   uint8_t status;
   fsctrl = DesiredFreq - 2048;
   cc2420controlm_gCurrentParameters[ CP_FSCTRL ] = ( cc2420controlm_gCurrentParameters[ CP_FSCTRL ]& 0xfc00 ) | ( fsctrl << 0 );
   status = cc2420controlm_HPLChipcon_write( 0x18, cc2420controlm_gCurrentParameters[ CP_FSCTRL ] );
   //IF THE oscillator is running, turn on the rx mode 
   // STATUS bit 06 means that the oscillator is running or not 
   if ( status& ( 1 << 6 )) {
      cc2420controlm_HPLChipcon_cmd( 0x03 );
   }
   return SUCCESS;
}
//the channel must be 11 to 26
inline result_t cc2420controlm_CC2420Control_TuneChannel( uint8_t channel )
 {
   uint16_t freq;
   freq = 2405+5 *( channel - 11 );
   cc2420controlm_CC2420Control_TuneManual( freq );
   return SUCCESS;
}

//-------------------------------------------------------------------------
inline 
result_t cc2420controlm_CC2420Control_TunePower( uint8_t powerlevel )
 {
   uint8_t status;
   //  cc2420controlm_gCurrentParameters[CP_FSCTRL] = (cc2420controlm_gCurrentParameters[CP_FSCTRL] & 0xfc00) | (fsctrl << 0);
   cc2420controlm_gCurrentParameters[ CP_TXCTRL ] = ( cc2420controlm_gCurrentParameters[ CP_TXCTRL ]& 0xffe0 ) | ( powerlevel& 0x1f );
   status = cc2420controlm_HPLChipcon_write( 0x15, cc2420controlm_gCurrentParameters[ CP_FSCTRL ] );
   //IF THE oscillator is running, turn on the rx mode 
   // STATUS bit 06 means that the oscillator is running or not 
   if ( status& ( 1 << 6 )) {
      cc2420controlm_HPLChipcon_cmd( 0x03 );
   }
   return SUCCESS;
}

//-------------------------------------------------------------------------
void setRadioFrequencyTask() {
   volatile uint16_t freq;
   asm volatile( "mov  %A0, r20""\n\t""mov  %B0, r21""\n\t": "=r"( freq ):  );
   cc2420controlm_CC2420Control_TuneManual( freq );
   return ;
}

//-------------------------------------------------------------------------
void setRadioChannelTask() {
   volatile uint16_t freq;
   asm volatile( "mov  %A0, r20""\n\t""mov  %B0, r21""\n\t": "=r"( freq ):  );
   cc2420controlm_CC2420Control_TuneChannel( freq );
   return ;
}

//-------------------------------------------------------------------------
void setRadioPowerTask() {
   volatile uint16_t powerlevel;
   asm volatile( "mov  %A0, r20""\n\t""mov  %B0, r21""\n\t": "=r"( powerlevel ):  );
   cc2420controlm_CC2420Control_TunePower( powerlevel );
   return ;
}

//-------------------------------------------------------------------------
inline 
result_t cc2420controlm_HPLChipconRAM_writeDone( uint16_t addr, uint8_t length, uint8_t *buffer )
 {
   return SUCCESS;
}

//-------------------------------------------------------------------------
inline result_t cc2420controlm_HPLChipconRAM_write( uint16_t arg_0xa45ad38, uint8_t arg_0xa45ae80, uint8_t *arg_0xa45afe0 ) {
   unsigned char result;
   result = HPLCC2420M_HPLCC2420RAM_write( arg_0xa45ad38, arg_0xa45ae80, arg_0xa45afe0 );
   return result;
}

//-------------------------------------------------------------------------
inline 
result_t cc2420controlm_CC2420Control_setShortAddress( uint16_t addr )
 {
   addr = toLSB16( addr );
   return cc2420controlm_HPLChipconRAM_write( 0x16A, 2, ( uint8_t* )& addr );
}

//-------------------------------------------------------------------------
inline uint16_t cc2420controlm_HPLChipcon_read( uint8_t arg_0xa4103b0 ) {
   unsigned int result;
   result = HPLCC2420M_HPLCC2420_read( arg_0xa4103b0 );
   return result;
}

//-------------------------------------------------------------------------
inline 
bool cc2420controlm_SetRegs( void )
 {
   uint16_t data;
   cc2420controlm_HPLChipcon_write( 0x10, cc2420controlm_gCurrentParameters[ CP_MAIN ] );
   cc2420controlm_HPLChipcon_write( 0x11, cc2420controlm_gCurrentParameters[ CP_MDMCTRL0 ] );
   data = cc2420controlm_HPLChipcon_read( 0x11 );
   if ( data != cc2420controlm_gCurrentParameters[ CP_MDMCTRL0 ] ) {
      return FALSE;
   }
   cc2420controlm_HPLChipcon_write( 0x12, cc2420controlm_gCurrentParameters[ CP_MDMCTRL1 ] );
   cc2420controlm_HPLChipcon_write( 0x13, cc2420controlm_gCurrentParameters[ CP_RSSI ] );
   cc2420controlm_HPLChipcon_write( 0x14, cc2420controlm_gCurrentParameters[ CP_SYNCWORD ] );
   cc2420controlm_HPLChipcon_write( 0x15, cc2420controlm_gCurrentParameters[ CP_TXCTRL ] );
   cc2420controlm_HPLChipcon_write( 0x16, cc2420controlm_gCurrentParameters[ CP_RXCTRL0 ] );
   cc2420controlm_HPLChipcon_write( 0x17, cc2420controlm_gCurrentParameters[ CP_RXCTRL1 ] );
   cc2420controlm_HPLChipcon_write( 0x18, cc2420controlm_gCurrentParameters[ CP_FSCTRL ] );
   cc2420controlm_HPLChipcon_write( 0x19, cc2420controlm_gCurrentParameters[ CP_SECCTRL0 ] );
   cc2420controlm_HPLChipcon_write( 0x1A, cc2420controlm_gCurrentParameters[ CP_SECCTRL1 ] );
   cc2420controlm_HPLChipcon_write( 0x1C, cc2420controlm_gCurrentParameters[ CP_IOCFG0 ] );
   cc2420controlm_HPLChipcon_write( 0x1D, cc2420controlm_gCurrentParameters[ CP_IOCFG1 ] );
   cc2420controlm_HPLChipcon_cmd( 0x09 );
   cc2420controlm_HPLChipcon_cmd( 0x08 );
   return TRUE;
}

//-------------------------------------------------------------------------
inline 
void cc2420controlm_PostOscillatorOn( void )
 {
   //This fucntion sets up all the registers of the radio module 
   cc2420controlm_SetRegs();
   //This function sets up the short address of the node, and therefore, if the mac frame includes a short address, it should be matched
   cc2420controlm_CC2420Control_setShortAddress( CURRENT_NODE_ID );
   //tHIS TURNS THE CHANELL
   cc2420controlm_CC2420Control_TuneManual((( cc2420controlm_gCurrentParameters[ CP_FSCTRL ] << 0 )& 0x1FF ) + 2048 );
    {
      _atomic_t _atomic = _atomic_start();
      cc2420controlm_state = cc2420controlm_START_STATE_DONE;
      _atomic_end( _atomic );
   }
   cc2420controlm_SplitControl_startDone();
}

//-------------------------------------------------------------------------
inline result_t cc2420controlm_CCA_fired( void )
 {
   cc2420controlm_HPLChipcon_write( 0x1D, 0 );
   postTask( cc2420controlm_PostOscillatorOn, 5 );
   return FAIL;
}

//-------------------------------------------------------------------------
inline result_t cc2420controlm_HPLChipconControl_init( void ) {
   unsigned char result;
   result = HPLCC2420M_StdControl_init();
   result = rcombine( result, HPLTimer1M_StdControl_init());
   return result;
}
