/*
The following is the license of LiteOS.

This file is part of LiteOS.
Copyright Qing Cao, Hossein Ahmadi 2007-2008, University of Illinois , qcao2@uiuc.edu

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

#include "radio.h"

uint8_t buffer[32];

uint8_t radioReceiveDataReady;

uint8_t radioReceivePacketInfo[4];

thread* mythread;

void sendRadioMsg()
{
 void (*radiosendfp)() = (void (*)(void))SOCKET_RADIO_SEND_FUNCTION;
 radiosendfp();
}

//This function sends out a string

void radioSend_string(uint8_t *msg)
{
    uint8_t temp = (uint8_t)String_length((char *)msg);
	return radioSend(1, 0xffff, temp, msg);
}

void radioSend_uint16(uint16_t value)
{
  uint8_t temp1,temp2;
  temp1 = (uint8_t) (value >> 8);
  temp2 = (uint8_t) (value & 0x00ff);

  buffer[0] = temp1;
  buffer[1] = temp2;
  buffer[2] = buffer[3] = 0xee;

  return radioSend(12, 0xffff, 16, buffer);
}

radiohandletype *getCurrentRadioHandleAddr()
{
   radiohandletype *currentradioinfo;

   void (*getaddrfp)(void) = (void (*)(void))GET_CURRENT_RADIO_HANDLE_ADDR;

   asm volatile("push r20" "\n\t"
                "push r21" "\n\t"
                ::);
   getaddrfp();
   asm volatile(" mov %A0, r20" "\n\t"
	             "mov %B0, r21" "\n\t"
				 :"=r" (currentradioinfo)
				 :
                );
    asm volatile("pop r21" "\n\t"
	             "pop r20" "\n\t"
	              ::);
   return currentradioinfo;
}


void setRadioFreq(uint16_t freq)
{

   void (*getaddrfp)(void) = (void (*)(void))SET_RADIO_FREQ_FUNCTION;

   asm volatile("push r20" "\n\t"
                "push r21" "\n\t"
                ::);

   asm volatile(" mov r20, %A0" "\n\t"
	              "mov r21, %B0" "\n\t"
				 :
				 :"r" (freq)
                );
   getaddrfp();
   asm volatile("pop r21" "\n\t"
	             "pop r20" "\n\t"
	              ::);
   return;
}

void setRadioChannel(uint8_t channel)
{

  uint16_t maskchannel = channel;

  void (*getaddrfp)(void) = (void (*)(void))SET_RADIO_CHANNEL_FUNCTION;

  asm volatile("push r20" "\n\t"
               "push r21" "\n\t"
               ::);

  asm volatile(" mov r20, %A0" "\n\t"
	              "mov r21, %B0" "\n\t"
				 :
				 :"r" (maskchannel)
                );
   getaddrfp();
   asm volatile("pop r21" "\n\t"
	             "pop r20" "\n\t"
	              ::);
   return;
}

void setRadioPower(uint8_t power)
{

  uint16_t maskchannel = power;

  void (*getaddrfp)(void) = (void (*)(void))SET_RADIO_POWER_FUNCTION;

  asm volatile("push r20" "\n\t"
               "push r21" "\n\t"
               ::);

  asm volatile(" mov r20, %A0" "\n\t"
	              "mov r21, %B0" "\n\t"
				 :
				 :"r" (maskchannel)
                );
   getaddrfp();
   asm volatile("pop r21" "\n\t"
	             "pop r20" "\n\t"
	              ::);
   return;
}



radioinfotype *getCurrentRadioInfoAddr()
{
   radioinfotype *currentradioinfo;
   void (*getaddrfp)(void) = (void (*)(void))GET_CURRENT_RADIO_INFO_ADDR;
   asm volatile("push r20" "\n\t"
                "push r21" "\n\t"
                ::);
   getaddrfp();
   asm volatile(" mov %A0, r20" "\n\t"
	             "mov %B0, r21" "\n\t"
				 :"=r" (currentradioinfo)
				 :
                );
    asm volatile("pop r21" "\n\t"
	             "pop r20" "\n\t"
	              ::);
   return currentradioinfo;
}


void disableRadioState()
{

   void (*disableradiofp)(void) = (void (*)(void))RESTORE_RADIO_STATE;

   disableradiofp();

   return;
}


void radioSend(uint16_t port, uint16_t address, uint8_t length, uint8_t *msg)
{


   mutex* msend;
   thread** current_thread;
   radioinfotype *radioinfoaddr;



   msend = getRadioMutexAddress();
   current_thread = getCurrentThread();
   radioinfoaddr = getCurrentRadioInfoAddr();

   Mutex_lock(msend);

   radioinfoaddr-> socket_port = port;
   radioinfoaddr->socket_addr = address;
   radioinfoaddr->socket_msg_len  = length;
   radioinfoaddr->socket_msg  = msg;

   //pending_id = id;
   //pending_address = address;
   //pending_length = length;
   //pending_msg = msg;

   sendRadioMsg();

   sleepThread(30);

   disableRadioState();

   Mutex_unlock(msend);

   return;
}

//wakeup the current thread once an incoming packet arrives

void wakeupMe()
{
  mythread->state = STATE_ACTIVE;
  syscall_postThreadTask();
}

int radioReceive(uint16_t port, uint8_t maxlength, uint8_t *msg)
{
   return radioReceiveTimed(port, maxlength, msg, 0);
}

int radioReceiveTimed(uint16_t port, uint8_t maxlength, uint8_t *msg, uint16_t time)
{
   _atomic_t currentatomic;
   
   void (*radioRegisterFP)(void) = (void (*)(void))REGISTER_RADIO_RECEIVE_EVENT;
   thread** current_thread = getCurrentThread();
   radiohandletype *radiohandleaddr = getCurrentRadioHandleAddr();

   //set up the radiohandleaddr data structures

   radiohandleaddr->port = port;
   radiohandleaddr->maxLength = 32;
   radiohandleaddr->dataReady = &radioReceiveDataReady;
   radiohandleaddr->data = buffer;
   radiohandleaddr->packetinfo = radioReceivePacketInfo;
   radiohandleaddr->handlefunc = wakeupMe;

	//close the interrupt
	currentatomic = _atomic_start();
	
	//call the radio handle set to store the data structure into the handle vectors
	radioRegisterFP();

	//set up the current thread into sleep mode
   (*current_thread)->state = STATE_SLEEP;

	//set up mythread so that later can wake up this thread
	mythread = *current_thread;

	//open the interrupt
	_atomic_end(currentatomic);

	if (time == 0)
		yield();
	else
		sleepThread(time);
	
	uint8_t i;
	for (i = 0; i < maxlength && i < 32; i++)
		msg[i] = buffer[i];

	return radioReceiveDataReady;
}
