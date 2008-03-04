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


#ifndef RADIOH
#define RADIOH

#include "types.h"
#include "radio.h"
#include "system.h"
#include "liteoscommon.h"
#include "mutex.h"
#include "string.h"
#include "thread.h"


/** \defgroup radio Radio Operations API 

      This module implements sending and receiving through radio. 
*/

/** \ingroup radio

      Get the internal radio handle address. 
      \param NONE
      \param address 
      
 */

radiohandletype *GET_CURRENT_RADIO_HANDLE_ADDRAddr();



/** \ingroup radio

      Send a packet over the radio. 
      \param port The port number of the radio sending operation
      \param address The next hop address
      \param length The length of the message to be sent out 
      \param msg The start address of the message to be sent out 
      \return NONE
      
 */
 
void radioSend(uint16_t port, uint16_t address, uint8_t length, uint8_t *msg); 


/** \ingroup radio

      Send a packet over the radio by broadcast 
      \param msg The message to be sent out. 
      \return NONE 
     
 */

void radioSend_string(uint8_t *msg);


/** \ingroup radio 

      Send an integer over the radio by broadcast
      \param value The integer to be sent out
      \return NONE
*/
void radioSend_uint16(uint16_t value);



/** \ingroup radio

       Receive a message through the radio. 
       \param port The port number listening to. 
       \param maxlength The maximal number of bytes provided
       \param msg The address to store the incoming message
       \return int The number of bytes received through radio 
*/
int radioReceive(uint16_t port, uint8_t maxlength, uint8_t *msg);

/** \ingroup radio

      Set the radio frequency directly using a number
      \param freq The frequency of the radio intended to work with
      \return NONE
*/     
void setRadioFreq(uint16_t freq);


/** \ingroup radio 

      Set the channel of the radio, 11 to 26
      \param channel The channel of the radio that is intended to work in. 
      \return NONE
*/

void setRadioChannel(uint8_t channel);


/** \ingroup radio
      Set the power level of the radio. 
      \param power The power level of the radio intended to work in.
      \return NONE
*/

void setRadioPower(uint8_t power);

#endif
