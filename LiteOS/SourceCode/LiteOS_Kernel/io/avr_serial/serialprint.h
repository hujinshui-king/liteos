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



#ifndef TESTUSART 
#define TESTUSART

#include "../../types/types.h"

#ifdef PLATFORM_AVR_IRIS
#include "../../system/amcommon.h"
#endif

/** \defgroup serial Serial port operations 

      \par Overview

      The serial port operations contain both sending and receiving packets over the serial port. This module is useful for debugging purposes, among others. 

      \par Initilization

      There is no explicit initilization needed. The usart hardware is initilized the first time a byte is sent over it. 
      
      \par Sending operations 

       Basically to send, it may use the following functions to do this purpose. 
         - printInteger()
         - printString()
         - printStringN()
         - usartPrint()

       On the other hand, if the user application intends to send through serial over the system calls, it needs to first call getSerialInfo(), then call SocketSerialSend() to 
       send out the data. 

       \par Receiving operations
       
      For receiving packets through the serial port, the following function APIs are provided. 

      First, the initSerialHandles() function must be invoked to init the serial data structure. 

      Then, the registerEventSerial() and deRegisterEventSerial() are used by the kernel to register event handlers for the serial packet.

      Finally, the getSerialHandleInfo() and syscall_registerEventSerial() are used by user applications to invoke the receive handler setup routines. 

 */

/** @{ */


///Print through the serial port 
void printInteger(int32_t a, int32_t b, int32_t c, int32_t d);

///Print string through the serial port 
void printString(char *p);

///Print string with a number of bytes through the serial port
void printStringN(char *p, uint8_t n); 


void printInteger32(int32_t a);


void printIntegerU32(uint32_t l);


///Print a char through the serial port 
void usartPrint(uint8_t c);

void initUSART();


void usartPutLong( uint32_t l );





#ifdef PLATFORM_AVR_IRIS

Radio_MsgPtr Broadcast2SerialAlternative(Radio_MsgPtr msg);


#endif

/** @} */
#endif 
