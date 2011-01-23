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

#ifndef SERIALH
#define SERIALH


#include "types.h"


/** \defgroup serial Serial Port API

      This module implements serial port handling. The following is a code sample showing how we test this module. 

      \code

 	#include "leds.h"
	#include "thread.h"
	#include "adc.h"
	#include "radio.h"
	#include "Serial.h"

	uint8_t receivedata[7]; 
	uint8_t temp,i; 

	int main()
	{
	 __asm__ __volatile__("sei" ::); 
	
	  receivedata[6] = 0; 
  
	  while (1)
	  { 
	  greenToggle();

	//  serialSend_string("hello"); 

	  serialReceive(3, 6, receivedata); 
	  for (i=0;i<3;i++)
		 {
		    temp = receivedata[i];
		    receivedata[i] = receivedata[5-i];
		    receivedata[5-i] = temp;

		  }
	  serialSend_string(receivedata); 
	  sleepThread(1000);
	}
	return 0; 
      }

     \endcode
      
*/


/** \ingroup eeprom 

      Write a string to the serial port
      \param msg The message to be sent to the serial port
      \return NONE
*/
void serialSend_string(uint8_t *msg);

/** \ingroup eeprom 

      Write an integer to the serial port
      \param value The value to be written to the serial port
      \return NONE
*/
void serialSend_uint16(uint16_t value);

/** \ingroup eeprom 

      Write a message with a length to the serial port
      \param length The length of the message
      \param msg The message content 
      \return NONE
*/
void serialSend(uint8_t length, uint8_t *msg);



/** \ingroup eeprom 

      Receive a message from the serial port
      \param port The port to receive the message 
      \param maxlength The maximal number of bytes to be received 
      \param msg The content of the message 
      \return VOID
*/

void serialReceive(uint16_t port, uint8_t maxlength, uint8_t *msg); 




#endif

