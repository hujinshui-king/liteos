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


#ifndef EEPROMH
#define EEPROMH
#include "liteoscommon.h"

/** \defgroup eeprom EEPROM Handling API

      This module implements reading/writing EEPROM of the motes.

      Testing code is as follows. 

      \code 
      #include "leds.h"
			#include "thread.h"
			#include "adc.h"
			#include "radio.h"
			#include "Serial.h"

			uint8_t receivedata[20]; 
			uint8_t temp;
			uint16_t i; 
			uint16_t j; 
			int main()
			{
			 __asm__ __volatile__("sei" ::); 
		  for (i=0;i<20;i++)
  		 receivedata[i] = i*6; 

		  for (i=3200;i<4000;i+=20)
	    writeToEEPROM(i, 20, receivedata);

	  	for (i=3200;i<4000;i+=20)
	  	{
	    for (j=0;j<20;j++)
   		 receivedata[j] = 0; 

			 readFromEEPROM(i, 20, receivedata);

			 serialSend(20, receivedata); 
	    }
  
    	return 0; 
	   }

    \endcode	
*/




/** \ingroup eeprom

      Read part of EEPROM into local buffer
      \param addr The start address of the EEPROM
      \param nBytes The number of bytes read from EEPROM
      \param buffer Pointer to the internal buffer to keep EEPROM data
      \return NONE
      \note The first 3200 bytes of EEPROM is occupied by the LiteFS file system
*/
void readFromEEPROM(uint16_t addr, uint16_t nBytes, uint8_t *buffer);


/** \ingroup eeprom

     Write part of EEPROM using local buffer
      \param addr The start address of the EEPROM
      \param nBytes The number of bytes written to EEPROM
      \param buffer Pointer to the internal buffer to retrive data to be written
      \return NONE
      \note The first 3200 bytes of EEPROM is occupied by the LiteFS file system
*/ 

void writeToEEPROM(uint16_t addr, uint16_t nBytes, uint8_t *buffer);


#endif
