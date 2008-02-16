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


#ifndef STDSERIALHANDLERH
#define STDSERIALHANDLERH

#include "../types/types.h"
      
/**\ingroup serial
     Get the internal serial register handle 
*/

void* getSerialHandleInfo();

/** \ingroup serial
      Register a serial packet handler through the system call
*/

void syscall_registerEventSerial();

/** \ingroup serial
      Init the serial handle data
*/
void initSerialHandles();

/** \ingroup serial
      Register the serial packet handler by providing necessary information 
*/
void registerEventSerial(uint16_t port, uint8_t maxLength, uint8_t *data, void (*handlefunc)(void));

/** \ingroup serial 
      DeRegister the serial packet handler
*/

void deRegisterEventSerial(uint16_t port);


/** \ingroup serial 
      Deliver a message to the corresponding port. 
*/

void serialDeliverPacket(uint16_t port, uint8_t length, uint8_t *msg);



#endif



