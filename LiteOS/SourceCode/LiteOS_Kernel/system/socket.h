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
#ifndef SOCKETH
#define SOCKETH
#include "../types/types.h"
#include "threaddata.h"

/** \addtogroup radio 
\par Overview
To operate radio, LiteOS provides the following key functions to send, receive and manage radio hardware. Note that, however,
LiteOS follows a split-model where the kernel development and user side applications follow different way to invoke radio functions. This section discusses how 
such split model is organized and how different functions are organized together. 
\par Initilization
LiteOS provides simple radio initilization, by calling the SocketInit() function. 
\par Send Operations
To send a packet, use the StandardSocketSend() function. To invoke it, it must be served with port number and next hop address. If the parameter 
msglength is set as 0, then the msg will be read to find the exact number of bytes, ending with 0. If the address parameter is set as 0, then this packet is assumed 
to be local, and will be directly sent to the Standard_Receive_Packet() function for processing. 
While the kernel may directly invoke StandardSocketSend(), for the user application, it must use a different function, SocketRadioSend(). To invoke this, however, it
must first use getRadioInfo() to get the internal RadioInfoType data structure address, then populate this piece of memory, then call SocketRadioSend(). It is suggested that, 
after each send operation, the function disableSocketRadioState() should be called (this function is already called in StandardSocketSend()). 
\par Receive Operations
The LiteOS radio module implements a registering based receive structure. The following functions are provided to achieve this goal. To receive a packet, the function
initRadioHandle() must first be called to initilize the radio handle for receiving. Then, the registerEvent() function is called to reigster. The application may also call
deRegisterEvent() to stop registering for a particular packet. 
If the user application runs outside the kernel, then the function getHandleInfo() is provided to return the internal address of the receive function handle. Furthermore, 
the function registerReceiverHandle_syscall() is provided to register the handle for user side applications. 
\note the function Standard_Receive_Packet is not intended to be used by user. It is called by the radio stack to initiate a packet scheduling procedure to deliver packet to 
the corresponding handler that registers this packet.       
*/

/** \ingroup radio 
The radio information internal representation for user applications 
*/
typedef struct
{
    uint16_t socket_port;
    uint16_t socket_addr;
    uint8_t socket_msg_len;
    uint8_t *socket_msg;
} radioinfotype;

/** \ingroup serial 
The serial information internal representation for user applications
*/
typedef struct
{
    uint8_t socket_msg_len;
    uint8_t *socket_msg;
} serialinfotype;

/** \ingroup radio 
API for initilization
\param NONE
\return NONE
*/
int SocketInit();

/** \ingroup radio 
Standard radio send interface
*/
void StandardSocketSend(uint16_t port, uint16_t address, uint8_t msglength,
                        uint8_t * msg);

/** \ingroup radio
Get radio data structure for user applications
*/
void *getRadioInfo();           //__attribute__((naked)

/** \ingroup radio 
For sending out packet through radio by user application through system call, and the standardsend, which is by kernel
*/
void SocketRadioSend();

/** \ingroup serial 
for user application to get the internal data structure to populate and call the send function 
*/
void *getSerialInfo();

/** \ingroup serial 
for sending out packet through serial by user application through system call . for the kernel, it will directly call print 
*/
void SocketSerialSend();

//lock the mutex 
void Socket_Mutex_lock(volatile mutex * m);

//unlock the mutex 
void Socket_Mutex_unlock(volatile mutex * m);
void Mutex_unlock(volatile mutex * m);

//__attribute__((naked)) 
void *getRadioMutexAddress();
void *getSerialMutexAddress();
void releaseMutexLockUponThreadKill(uint8_t index);
#endif
