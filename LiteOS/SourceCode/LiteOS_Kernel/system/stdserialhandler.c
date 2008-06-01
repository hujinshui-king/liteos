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
#include "stdserialhandler.h"
#include "scheduling.h"
//For the receving handles. Whenever a packet is received, content of this vector is compared and executed
enum
{
    SERIAL_RECEIVE_NUM = 4,
};

//The definition of a radio handle for user use 
typedef struct serial_handle
{
    uint16_t port;
    uint8_t maxLength;
    uint8_t *data;
    uint8_t handlevalid;
    void (*handlefunc) (void);
} serial_handle;

//the handles definition
serial_handle serialreceivehandles[SERIAL_RECEIVE_NUM];

//the handleinfo is defined as a buffer for incoming user requests 
static serial_handle handleinfo;

//get the address of the handle structure for system call to populate 
void *getSerialHandleInfo()
{
    return &handleinfo;
    //asm volatile( "mov r20, %A0""\n\t""mov r21, %B0""\n\t": : "r"( addr ) );
    // asm volatile("ret"::); 
}

//system call interface for registering an event 
void syscall_registerEventSerial()
{
    registerEventSerial(handleinfo.port, handleinfo.maxLength, handleinfo.data,
                        handleinfo.handlefunc);
}

//init all handles by setting their handle valid indicator as 0
void initSerialHandles()
{
    uint8_t i;

    for (i = 0; i < SERIAL_RECEIVE_NUM; i++)
    {
        serialreceivehandles[i].handlevalid = 0;
    }
}

//Register A New Handle
void registerEventSerial(uint16_t port, uint8_t maxLength, uint8_t * data, void
                         (*handlefunc) (void))
{
    uint8_t i;

    for (i = 0; i < SERIAL_RECEIVE_NUM; i++)
        if (serialreceivehandles[i].handlevalid == 0)
        {
            serialreceivehandles[i].port = port;
            serialreceivehandles[i].maxLength = maxLength;
            serialreceivehandles[i].data = data;
            serialreceivehandles[i].handlefunc = handlefunc;
            serialreceivehandles[i].handlevalid = 1;
            return;
        }
}

//-------------------------------------------------------------------------
void deRegisterEventSerial(uint16_t port)
{
    uint8_t i;

    for (i = 0; i < SERIAL_RECEIVE_NUM; i++)
        if ((serialreceivehandles[i].handlevalid == 1)
            && (serialreceivehandles[i].port == port))
        {
            serialreceivehandles[i].handlevalid = 0;
            serialreceivehandles[i].port = 0;
            return;
        }
}

//Now this OS has a new packet, needs to deliver it to the correct thread for processing, and return the packet as soon as possible 
void serialDeliverPacket(uint16_t port, uint8_t length, uint8_t * msg)
{
    uint8_t i;

    for (i = 0; i < SERIAL_RECEIVE_NUM; i++)
    {
        if ((serialreceivehandles[i].port == port)
            && (serialreceivehandles[i].handlevalid == 1))
        {
            uint8_t temp;
            uint8_t j;
            uint8_t *buf;

            buf = (uint8_t *) serialreceivehandles[i].data;
            temp =
                (length >
                 (serialreceivehandles[i].
                  maxLength) ? (serialreceivehandles[i].maxLength) : length);
            for (j = 0; j < temp; j++)
            {
                *buf = msg[j];
                buf++;
            }
            //For all incoming buffer, its last byte is always set as 0. Further, the last byte of the messsage, if it is shorter, is also set as 0. 
            if (length < serialreceivehandles[i].maxLength)
            {
                buf = (uint8_t *) serialreceivehandles[i].data;
                buf[length] = 0;
            }
            buf[serialreceivehandles[i].maxLength - 1] = 0;
            if (serialreceivehandles[i].handlefunc != NULL)
            {
                postTask(serialreceivehandles[i].handlefunc, 6);
            }
            return;
        }
    }
    return;
}
