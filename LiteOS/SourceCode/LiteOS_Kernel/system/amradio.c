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
//These included files are platform independent 
#include "nodeconfig.h"
#include "scheduling.h"
#include "amcommon.h"
#include "amradio.h"
#include "generictimer.h"
#include "../types/byteorder.h"
#include "../types/types.h"
#include "packethandler.h"
#include "../utilities/eventlogger.h"
//These included files are platform dependent and only apply for cc2420 on MicaZ 
#if defined(PLATFORM_AVR) && defined(RADIO_CC2420)
#include "../io/cc2420/cc2420const.h"
#include "../io/cc2420/cc2420controlm.h"
#include "../io/cc2420/cc2420radiom.h"
#include "../platform/micaz/hplcc2420fifom.h"
#include "../platform/micaz/hplcc2420interruptm.h"
#include "../platform/micaz/hplcc2420m.h"
#include "../platform/micaz/hpltimer1.h"
#endif

#if defined(PLATFORM_AVR) && defined(RADIO_RF230)
#include "../io/rf230/rf230radiom.h"
#endif



bool AMStandard_state;
Radio_MsgPtr AMStandard_buffer;
uint16_t AMStandard_lastCount;
uint16_t AMStandard_counter;

//-------------------------------------------------------------------------
inline bool AMStandard_Control_init(void)
{
    result_t ok2;
	ok2 = 0; 


#if defined(PLATFORM_AVR) && defined(RADIO_CC2420)
    ok2 = cc2420radiom_SplitControl_init();
#endif

#if defined(PLATFORM_AVR) && defined(RADIO_RF230)
    ok2 = trx_init();
#endif 

    AMStandard_state = FALSE;
    AMStandard_lastCount = 0;
    AMStandard_counter = 0;
    return ok2;
}

//-------------------------------------------------------------------------
inline result_t AMStandard_RadioControl_start(void)
{
    unsigned char result;
	result = 0; 

#if defined(PLATFORM_AVR) && defined(RADIO_CC2420)
    result = cc2420radiom_StdControl_start();
#endif
    return result;
}

//-------------------------------------------------------------------------
inline bool AMStandard_Control_start(void)
{
    result_t ok2 = AMStandard_RadioControl_start();

    AMStandard_state = FALSE;
    return ok2;
}

//-------------------------------------------------------------------------
inline result_t AMStandard_RadioSend_send(Radio_MsgPtr arg_0xa3c31f8)
{
    unsigned char result;

#if defined(PLATFORM_AVR) && defined(RADIO_CC2420)
    result = cc2420radiom_Send_send(arg_0xa3c31f8);
#endif

#if defined(PLATFORM_AVR)&& defined(RADIO_RF230)
   trx_init();
   result = rf230radio_Send_send(arg_0xa3c31f8);
#endif

    return result;
}

//-------------------------------------------------------------------------
inline void AMStandard_sendTask(void)
{
    result_t ok;
    Radio_MsgPtr buf;

    buf = AMStandard_buffer;
    ok = AMStandard_RadioSend_send(buf);
    if (ok == FAIL)
    {
        AMStandard_reportSendDone(AMStandard_buffer, FAIL);
    }
}

//addr means the current broadcast address, et. id is the port 
//Send out a message and takes a while to complete 
result_t AMStandard_SendMsg_send(uint16_t port, uint16_t addr, uint8_t length,
                                 Radio_MsgPtr data)
{
#ifdef TRACE_ENABLE
#ifdef TRACE_ENABLE_RADIOEVENT
    addTrace(TRACE_RADIOEVENT_SENDPACKET, 100);
#endif
#endif
    if (!AMStandard_state)
    {
        AMStandard_state = TRUE;
        if (length > DATA_LENGTH)
        {
            AMStandard_state = FALSE;
            return FAIL;
        }
        if (!postTask(AMStandard_sendTask, 20))
        {
            {
            }
            ;
            AMStandard_state = FALSE;
            return FAIL;
        }
        else
        {
            //length is the first one that means the actual data length
            //adr is the next hop id
            //type is the port
            //group is manmade result 
            AMStandard_buffer = data;
            data->length = length;
            data->addr = addr;
            data->port = port;
        }
        return SUCCESS;
    }
    return FAIL;
}

//-------------------------------------------------------------------------
inline result_t AMStandard_SendMsg_default_sendDone(uint8_t id, Radio_MsgPtr
                                                    msg, result_t success)
{
    return SUCCESS;
}

//-------------------------------------------------------------------------
inline result_t AMStandard_SendMsg_sendDone(uint16_t arg_0xa3b8f90,
                                            Radio_MsgPtr arg_0xa31a0a0,
                                            result_t arg_0xa31a1f0)
{
    return 0;
}

//-------------------------------------------------------------------------
inline result_t AMStandard_default_sendDone(void)
{
    return SUCCESS;
}

//-------------------------------------------------------------------------
inline result_t AMStandard_sendDone(void)
{
    unsigned char result;

    result = AMStandard_default_sendDone();
    return result;
}

//-------------------------------------------------------------------------
inline Radio_MsgPtr AMStandard_ReceiveMsg_default_receive(uint8_t id,
                                                          Radio_MsgPtr msg)
{
    return msg;
}

//Another critical function that bridges to the application 
inline Radio_MsgPtr AMStandard_ReceiveMsg_receive(uint16_t port, Radio_MsgPtr
                                                  msg)
{
    return msg;
}

//This function is really really critical to the correct behaviro of the radio stack 
//Basically it returns a radio message pointer that must be reused 
//and the content of the packet is the correct packet parsed, and is useful 
//The content starts with a length that is the actual payload length and all information are correct
//Must copy this content to the user supplied buffer, put the user action into a task, and then return this buffer to the recevie module 
Radio_MsgPtr received(Radio_MsgPtr packet)
{
    uint16_t addr = CURRENT_NODE_ID;

#ifdef TRACE_ENABLE
#ifdef TRACE_ENABLE_RADIOEVENT
    addTrace(TRACE_RADIOEVENT_RECEIVEPACKET, 100);
#endif
#endif
    AMStandard_counter++;
    if (packet->crc == 1 && (packet->addr == BCAST_ADDRESS || packet->addr ==
                             addr))
    {
        uint16_t port = packet->port;
        Radio_MsgPtr tmp;

        tmp = Standard_Receive_Packet(port, packet);
        if (tmp)
        {
            packet = tmp;
        }
    }
    return packet;
}




//IRIS-This function is really really critical to the correct behaviro of the radio stack 
//Basically it returns a radio message pointer that must be reused 
//and the content of the packet is the correct packet parsed, and is useful 
//The content starts with a length that is the actual payload length and all information are correct
//Must copy this content to the user supplied buffer, put the user action into a task, and then return this buffer to the recevie module 
Radio_MsgPtr received_iris( Radio_MsgPtr packet )
 {
   uint16_t addr = CURRENT_NODE_ID;
  
   
   #ifdef TRACE_ENABLE
      #ifdef TRACE_ENABLE_RADIOEVENT
       addTrace(TRACE_RADIOEVENT_RECEIVEPACKET, 100);     
      #endif
   #endif


   AMStandard_counter ++;
   
   //Debug
   /*
        printfstr ("\n #####Rcv Packet CRC####### \n "); 
        printfintegeru32((uint32_t)packet->crc);
        printfstr ("\n #####Rcv Packet ADDR####### \n "); 
        printfintegeru32((uint32_t)packet->addr);
        
   */
   
   #if defined(PLATFORM_AVR_IRIS) && defined(PLATFORM_IRIS_BASE)
        {
        Radio_MsgPtr tmp;
        if ((packet->addr == BCAST_ADDRESS)&&(packet->port == 0xefef))
        	 {
        	 	packet->length = packet->length + MSG_HEADER_SIZE; 
        	  tmp = Broadcast2SerialAlternative(packet); 
        	   if ( tmp ) {
               packet = tmp;
             } 
             return packet; 
        	 }
         } 	 
    
   
   #endif 
   
   
   if ( ( packet->addr == BCAST_ADDRESS || packet->addr == addr )) { //Not check CRC Now
      uint16_t port = packet->port;
      Radio_MsgPtr tmp;
      tmp = Standard_Receive_Packet( port, packet );
      
        
      if ( tmp ) {
      	//Debug
      	//printfstr ("\n ######Packet=Tmp####### \n "); 
         packet = tmp;
      }
   }
    // Leds_yellowToggle();  //Upon Receiving Packet in received fun
     
      //printfstr ("\n #####Rcv Packet PORT####### \n "); 
       //printfintegeru32((uint32_t)packet->port);
     
   //Debug
 
    /*    printfstr ("\n ######Receive Packet Length####### \n "); 
        printfintegeru32((uint32_t)packet->length);
   	  	printfstr ("\n----Received Packet Data----- \n"); 
        for (i=0;i<packet->length-MSG_HEADER_SIZE;i++)
        {
        printfintegeru32((uint32_t) packet->data[i]); 
        printfstr ("\n"); 
        }
    */
   return packet;
}



//-------------------------------------------------------------------------
inline Radio_MsgPtr AMStandard_RadioReceive_receive(Radio_MsgPtr packet)
{
   #if defined(PLATFORM_AVR) && defined(RADIO_CC2420)
   return received( packet );
   #endif
   
   #if defined(PLATFORM_AVR) && defined(RADIO_RF230)
   return received_iris( packet );
   #endif

     
}

//-------------------------------------------------------------------------
inline result_t AMStandard_RadioSend_sendDone(Radio_MsgPtr msg, result_t
                                              success)
{
    return AMStandard_reportSendDone(msg, success);
}

//-------------------------------------------------------------------------
result_t AMStandard_reportSendDone(Radio_MsgPtr msg, result_t success)
{
    AMStandard_state = FALSE;
    //  AMStandard_SendMsg_sendDone(msg->port, msg, success);
    //  AMStandard_sendDone();
    return SUCCESS;
}

//-------------------------------------------------------------------------
inline void restoreRadioState()
{
    AMStandard_state = FALSE;
#if defined(PLATFORM_AVR) && defined (RADIO_CC2420)
    restorecc2420state();
#endif
}
