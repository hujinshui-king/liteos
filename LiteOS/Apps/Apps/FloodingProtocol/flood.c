/* LiteOS Version 0.2 */
/*
The following is the license of LiteOS.

This file is part of LiteOS.
Copyright Qing Cao, 2007, University of Illinois , qcao2@uiuc.edu

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



/*
 Limitations:
 It is known that the current implementation uses random number + nodeid to track duplicates. If a node reboots, it may reuse the random numbers and 
 the nodeid, therefore generating packets that may look like duplicates. This is a known limitation due to node reboot. 
*/


//Register port 20 for data packets. No neighbor exchanges
/*Each packet format:

      nodeid  2 bytes

     randomsig  2 bytes
   
     data content 
     


*/


#include "leds.h"
#include "thread.h"
#include "adc.h"
#include "radio.h"
#include "serial.h"
#include "file.h"
#include "system.h"
#include "thread.h"

uint16_t nodeid; 

uint16_t packetsreceived, packetsdelivered; 


//Pointer to the current thread data structure. Useful for waking up the current thread. 
thread **thiscurrentthread;
thread *thisthread; 


typedef struct
{
  uint16_t randomsig; 
  uint16_t nodeid; 
  uint8_t valid; 
}
historypacket; 


#define HISTORYBUFFERSIZE 10

//The current index
uint8_t indexOfPackets;

//Neighbor table 
static historypacket historypackettable[ HISTORYBUFFERSIZE ];



//This function initlizes the neighhood table 
void initTable() {
   int i;
   for ( i = 0; i < HISTORYBUFFERSIZE; i ++ ) {
      historypackettable[ i ].valid = 0;
   } 
   indexOfPackets = 0;
}


uint8_t historycheck(uint16_t nodeid, uint16_t randomid)
{
   uint8_t i; 
   for (i=0; i < HISTORYBUFFERSIZE; i++) {
   	 if (historypackettable[i].valid == 0)
	 	continue;
	 if ((historypackettable[i].randomsig == randomid ) && (historypackettable[i].nodeid == nodeid))
	 	return 1; 
   	}
   return 0; 
}


void inserthistorytable(uint16_t nodeid, uint16_t randomid)
{

   historypackettable[indexOfPackets].valid = 1; 
   historypackettable[indexOfPackets].nodeid = nodeid; 
   historypackettable[indexOfPackets].randomsig = randomid; 
   indexOfPackets = (indexOfPackets + 1)%HISTORYBUFFERSIZE;
   return;  
}



//-------------------------------------------------------------------------
static uint8_t incomingMsg[ 32 ];
static uint8_t incomingDataLength;
static uint8_t incomingPacketInfo[ 4 ];


//Handles incoming data msg. This usually wakesup the current thread 
void wakeupMeOnDataMsg() {

   thisthread->state = STATE_ACTIVE;
   syscall_postThreadTask();    
}


//Registers the data packet for receving incoming packets. This enables receving both remote and local data packets

void registerDataPacket() {

   _atomic_t currentatomic;
   radiohandletype *radiohandleaddr;
   
   void (*getaddrfp)(void) = (void (*)(void))SETCURRENTRADIOHANDLE; 

   //Finds the radio handle, so that when this code is ported to be externally used, it will be obtained via a system call 
   radiohandleaddr =  getCurrentRadioHandleAddr();

   incomingDataLength = 0;
   
   //set up the radiohandleaddr data structures
   radiohandleaddr->port = 20;
   radiohandleaddr->maxLength = 32;
   radiohandleaddr->dataReady = &incomingDataLength;
   radiohandleaddr->data = incomingMsg;
   radiohandleaddr->packetinfo = incomingPacketInfo;
   radiohandleaddr->handlefunc = wakeupMeOnDataMsg;

   //close the interrupt     
   currentatomic = _atomic_start();
   
   //call the radio handle set to store the data structure into the handle vectors 
   getaddrfp();     
   //syscall_registerEvent();
   
   //open the interrupt 
   _atomic_end( currentatomic );
   return ;
}




//This is the entry point. It broadcasts neighbor exchanges perodically, or handles incoming packets (non-update neighbor)

int main() {

   uint8_t portnum;
   uint16_t nodeid; 

   uint16_t randomsig;
   uint16_t incomingnodeid; 

   uint8_t temp;
   uint8_t jitter; 
   uint16_t comingnodeid; 
  

   initTable();
   nodeid = getnodeID();
   
   //register both the negibhorhood update and the message incoming 

   thiscurrentthread = getCurrentThread(); 
   thisthread = *thiscurrentthread; 
      
   registerDataPacket();

   packetsreceived = packetsdelivered = 0; 


   while ( 1 ) {

         hibernateThread();
		 incomingnodeid = 0; 
         //handles the data message by restructing it and broadcast it  
         //If the packet is new, send it both locally and remotely

		 temp  = incomingMsg[ 0 ];
		 
		 incomingnodeid += ((uint16_t)temp); 
		 temp = incomingMsg[ 1 ];
		 incomingnodeid += ((uint16_t)temp) * 256;

         randomsig = 0; 
		 temp = incomingMsg[ 2 ];
		 randomsig += ((uint16_t)temp);

   	     temp = incomingMsg[ 3 ];
		 randomsig += ((uint16_t)temp) * 256;

		 portnum = incomingMsg[4]; 

		 if ((nodeid == incomingnodeid)&&(historycheck(incomingnodeid, randomsig) == 0))
		 	{
    		 	inserthistorytable(incomingnodeid, randomsig); 
				incomingMsg[5] = nodeid%256; 
				incomingMsg[6] = nodeid/256; 
				
				jitter = rnd()%20;
				sleepThread(jitter); 
				radioSend(20, 0xffff, incomingDataLength, incomingMsg);
				packetsreceived ++; 
		    }
		 

		 else if (historycheck(incomingnodeid, randomsig) == 0)
		 	{
             
				comingnodeid = incomingMsg[5];
				if ((comingnodeid != nodeid -1 )&&( comingnodeid!=nodeid+1))
					{
					}
				else
					{
                radioSend(portnum, 0, incomingDataLength, incomingMsg); 
				
				inserthistorytable(incomingnodeid, randomsig); 

				incomingMsg[5] = nodeid%256;
				
				jitter = rnd()%20;
				sleepThread(jitter); 
				radioSend(20, 0xffff, incomingDataLength, incomingMsg);
				packetsdelivered++; 
					}
			  
					}
		 else
		 	{
             
		 	}
   	}
   return 0; 
}


			 
        

