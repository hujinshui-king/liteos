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
#ifndef CC2420CONTROLMH
#define CC2420CONTROLMH
#include "../../types/types.h"
enum cc2420controlm___nesc_unnamed4270
{
    cc2420controlm_IDLE_STATE = 0, cc2420controlm_INIT_STATE,
    cc2420controlm_INIT_STATE_DONE, cc2420controlm_START_STATE,
    cc2420controlm_START_STATE_DONE, cc2420controlm_STOP_STATE
};

/*@} */
/* \addtogroup radio */
inline result_t cc2420controlm_SplitControl_init(void);
inline result_t cc2420controlm_CCA_startWait(bool arg_0xa422588);
inline result_t cc2420controlm_CC2420Control_VREFOn(void);
inline result_t cc2420controlm_SplitControl_initDone(void);
inline void cc2420controlm_taskInitDone(void);
inline uint8_t cc2420controlm_HPLChipcon_cmd(uint8_t arg_0xa403928);
inline uint8_t cc2420controlm_HPLChipcon_write(uint8_t arg_0xa403d80, uint16_t
                                               arg_0xa403ed0);
inline result_t cc2420controlm_CC2420Control_OscillatorOn(void);
inline result_t cc2420controlm_HPLChipconControl_start(void);
inline result_t cc2420controlm_SplitControl_start(void);
inline result_t cc2420controlm_CC2420Control_RxMode(void);
inline result_t cc2420controlm_SplitControl_startDone(void);
inline result_t cc2420controlm_CC2420Control_TuneManual(uint16_t DesiredFreq);
inline result_t cc2420controlm_CC2420Control_TuneChannel(uint8_t channel);
inline result_t cc2420controlm_CC2420Control_TunePower(uint8_t powerlevel);
inline result_t cc2420controlm_HPLChipconRAM_write(uint16_t arg_0xa45ad38,
                                                   uint8_t arg_0xa45ae80,
                                                   uint8_t * arg_0xa45afe0);
inline result_t cc2420controlm_CC2420Control_setShortAddress(uint16_t addr);
inline uint16_t cc2420controlm_HPLChipcon_read(uint8_t arg_0xa4103b0);
inline bool cc2420controlm_SetRegs(void);
inline void cc2420controlm_PostOscillatorOn(void);
inline result_t cc2420controlm_CCA_fired(void);
inline result_t cc2420controlm_HPLChipconRAM_writeDone(uint16_t addr, uint8_t
                                                       length,
                                                       uint8_t * buffer);
inline result_t cc2420controlm_HPLChipconControl_init(void);
inline result_t cc2420controlm_CC2420Control_VREFOff(void);

/** \addtogroup radio*/

/** @{ */
///User system call API. It relies on r20, r21 to get the freq variable. 
void setRadioFrequencyTask();

///User system call API. It relies on r20, r21 to get the freq variable. 
void setRadioChannelTask();

///User system call API. It relies on r20, r21 to get the power level variable. 
void setRadioPowerTask();

/** @} */
#endif
