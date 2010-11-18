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
 * Authors:             Jason Hill, David Gay, Philip Levis
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
/*                                                                      tab:4
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
 *      Redistributions of source code must retain the above copyright
 *  notice, this list of conditions and the following disclaimer.
 *      Redistributions in binary form must reproduce the above copyright
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
 * Authors:             Joe Polastre
 * Date last modified:  $Revision: 1.6 $
 *
 * 
 */

/**
* @author Joe Polastre
* @author Alan Broad
*/
#ifndef CC2420RADIOMH
#define CC2420RADIOMH
#include "../radio/amcommon.h"
#include "../../types/types.h"
/* \addtogroup radio */
/*@{ */
inline result_t cc2420radiom_SplitControl_default_initDone(void);
inline result_t cc2420radiom_CC2420SplitControl_initDone(void);
inline result_t cc2420radiom_SplitControl_init(void);
inline result_t cc2420radiom_CC2420SplitControl_start(void);
inline result_t cc2420radiom_SplitControl_start(void);
inline void cc2420radiom_startRadio(void);
inline result_t cc2420radiom_StdControl_start(void);
inline result_t cc2420radiom_SFD_enableCapture(bool arg_0xa41e260);
inline uint8_t cc2420radiom_HPLChipcon_cmd(uint8_t arg_0xa403928);
inline void cc2420radiom_sendPacket(void);
inline void cc2420radiom_tryToSend(void);
inline result_t cc2420radiom_HPLChipconFIFO_TXFIFODone(uint8_t length,
                                                       uint8_t * data);
inline result_t cc2420radiom_HPLChipconFIFO_writeTXFIFO(uint8_t arg_0xa40c010,
                                                        uint8_t *
                                                        arg_0xa40c170);
inline void cc2420radiom_startSend(void);
inline result_t cc2420radiom_Send_send(Radio_MsgPtr pMsg);
inline result_t cc2420radiom_SplitControl_default_startDone(void);
inline result_t cc2420radiom_FIFOP_startWait(bool arg_0xa422588);
inline result_t cc2420radiom_CC2420Control_RxMode(void);
inline result_t cc2420radiom_CC2420SplitControl_startDone(void);
inline uint16_t cc2420radiom_HPLChipcon_read(uint8_t arg_0xa4103b0);
inline result_t cc2420radiom_FIFOP_disable(void);
inline void cc2420radiom_delayedRXFIFOtask(void);
inline result_t cc2420radiom_FIFOP_fired(void);
inline Radio_MsgPtr cc2420radiom_Receive_receive(Radio_MsgPtr arg_0xa31b650);
inline void cc2420radiom_PacketRcvd(void);
inline result_t cc2420radiom_HPLChipconFIFO_RXFIFODone(uint8_t length,
                                                       uint8_t * data);
inline result_t cc2420radiom_HPLChipconFIFO_readRXFIFO(uint8_t arg_0xa4118a0,
                                                       uint8_t *
                                                       arg_0xa411a00);
inline result_t cc2420radiom_Send_sendDone(Radio_MsgPtr arg_0xa3c3710,
                                           result_t arg_0xa3c3860);
inline void cc2420radiom_PacketSent(void);
inline result_t cc2420radiom_SFD_disable(void);
inline result_t cc2420radiom_SFD_captured(uint16_t time);
inline void cc2420radiom_sendFailed(void);
inline void cc2420radiom_flushRXFIFO(void);
inline void cc2420radiom_delayedRXFIFO(void);
inline result_t cc2420radiom_SplitControl_initDone(void);
inline void restorecc2420state();

/*@} */
#endif
