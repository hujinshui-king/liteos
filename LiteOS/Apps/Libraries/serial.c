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
//#include <avr/io.h>
//#include <avr/interrupt.h>
#include "serial.h"
#include "system.h"
#include "liteoscommon.h"
#include "mutex.h"
#include "stringutil.h"
#include "thread.h"


uint8_t serial_buffer[32];

thread* mythreadserial;

void sendSerialMsg()
{
 void (*serialsendfp)() = (void (*)(void))SERIAL_SEND_FUNCTION;
 serialsendfp();
}









//This function sends out a string

void serialSend_string(uint8_t *msg)
{

    uint8_t temp = (uint8_t)String_length((char *)msg);
	return serialSend(temp, msg);

}





void serialSend_uint16(uint16_t value)
{
  uint8_t temp1,temp2;
  temp1 = (uint8_t) (value >> 8);
  temp2 = (uint8_t) (value & 0x00ff);

  serial_buffer[0] = temp1;
  serial_buffer[1] = temp2;
  serial_buffer[2] = serial_buffer[3] = 0xee;

  return serialSend(16, serial_buffer);

}




serialhandletype *getCurrentSerialHandleAddr()
{
   serialhandletype *currentserialhandle;

   void (*getaddrfp)(void) = (void (*)(void))GET_SERIAL_RECEIVE_HANDLE;

   asm volatile("push r20" "\n\t"
                "push r21" "\n\t"
                ::);
   getaddrfp();
   asm volatile(" mov %A0, r20" "\n\t"
	             "mov %B0, r21" "\n\t"
				 :"=r" (currentserialhandle)
				 :
                );
    asm volatile("pop r21" "\n\t"
	             "pop r20" "\n\t"
	              ::);
   return currentserialhandle;
}






serialinfotype* getCurrentSerialInfo()
{
   serialinfotype *currentserialinfo;
   void (*getaddrfp)(void) = (void (*)(void))GET_SERIAL_SEND_STRUCTURE_ADDRESS;
   asm volatile("push r20" "\n\t"
                "push r21" "\n\t"
                ::);
   getaddrfp();
   asm volatile(" mov %A0, r20" "\n\t"
	             "mov %B0, r21" "\n\t"
				 :"=r" (currentserialinfo)
				 :
                );
    asm volatile("pop r21" "\n\t"
	             "pop r20" "\n\t"
	              ::);
   return currentserialinfo;
}



void serialSend(uint8_t length, uint8_t *msg)
{


   mutex* mserialsend;
   thread** current_thread;
   serialinfotype *serialinfoaddr;



   mserialsend = getSerialMutexAddress();
   current_thread = getCurrentThread();
   serialinfoaddr = getCurrentSerialInfo();

   Mutex_lock(mserialsend);

   serialinfoaddr->socket_msg_len = length;
   serialinfoaddr->socket_msg = msg;


   sendSerialMsg();

   sleepThread(20);


   Mutex_unlock(mserialsend);

   return;
}



//wakeup the current thread once an incoming packet arrives

void wakeupMeSerial()
{
  mythreadserial->state = STATE_ACTIVE;
  syscall_postThreadTask();


}



//In serial receive, the port is above 0, which means that such messages are delivered for the user applications.


void serialReceive(uint16_t port, uint8_t maxlength, uint8_t *msg)

{


   thread** current_thread;

   _atomic_t currentatomic;

   serialhandletype *serialhandleaddr;

   void (*getaddrfp)(void) = (void (*)(void))REGISTER_SERIAL_RECEIVE_EVENT;

   current_thread = getCurrentThread();

   serialhandleaddr = getCurrentSerialHandleAddr();

   //set up the radiohandleaddr data structures

   serialhandleaddr->port = port;
   serialhandleaddr->maxLength = maxlength;
   serialhandleaddr->data = msg;
   serialhandleaddr->handlefunc = wakeupMeSerial;


   //close the interrupt
	currentatomic = _atomic_start();

   //call the radio handle set to store the data structure into the handle vectors
    getaddrfp();


   //set up the current thread into sleep mode
   (*current_thread)->state = STATE_SLEEP;

   //set up mythread so that later can wake up this thread
   mythreadserial = *current_thread;


   //open the interrupt
   _atomic_end(currentatomic);

    yield();


   return;

}

