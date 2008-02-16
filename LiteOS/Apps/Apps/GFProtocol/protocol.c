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


//Registers port 10 for packets, port 11 for neighbor broadcasts 


#include "leds.h"
#include "thread.h"
#include "adc.h"
#include "radio.h"
#include "Serial.h"
#include "File.h"
#include "System.h"
#include "thread.h"

uint16_t nodeid; 



//Pointer to the current thread data structure. Useful for waking up the current thread. 
thread **thiscurrentthread;
thread *currentthread; 

uint16_t packetslocal;
uint16_t packetsremote;
uint16_t packetsdeliver; 

//The neighbor table
typedef struct {
   uint16_t nodeid;
   uint8_t local_x;
   uint8_t local_y;
   uint8_t valid;
} nbr_table_entry;


//The maximum size of the neighbor table 
#define NBR_SIZE 7

//The current number of neighbors
uint8_t numOfNeighbors;

//Neighbor table 
static nbr_table_entry nbtTable[ NBR_SIZE ];


//Find out whether there is packet pending or not
static uint8_t iswakenup;

//Every five seconds, send out neighbor update message (beacons)
#define NBR_UPDATE_CYCLE 5000

//This function initlizes the neighhood table 
void initTable() {
   int i;
   for ( i = 0; i < NBR_SIZE; i ++ ) {
      nbtTable[ i ].valid = 0;
   } 
   numOfNeighbors = 0;
   iswakenup = 0;
}




//-------------------------------------------------------------------------
static uint8_t incomingMsg[ 32 ];
static uint8_t incomingDataLength;
static uint8_t incomingPacketInfo[ 4 ];


//Handles incoming data msg. This usually wakesup the current thread 
void wakeupMeOnDataMsg() {

   //(*thiscurrentthread)->state = STATE_ACTIVE;
   currentthread->state = STATE_ACTIVE; 
   iswakenup = 1;
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
   radiohandleaddr->port = 10;
   radiohandleaddr->maxLength = 32;
   radiohandleaddr->dataReady = &incomingDataLength;
   radiohandleaddr->data = incomingMsg;
   radiohandleaddr->packetinfo = incomingPacketInfo;
   radiohandleaddr->handlefunc = wakeupMeOnDataMsg;

   //close the interrupt     
   currentatomic = _atomic_start();
   
   iswakenup = 0;
   //call the radio handle set to store the data structure into the handle vectors 
   getaddrfp();     
   //syscall_registerEvent();
   
   //open the interrupt 
   _atomic_end( currentatomic );
   return ;
}



//-------------------------------------------------------------------------
static uint8_t incomingNbrMsg[ 16 ];
static uint8_t incomingNbrLength;
static uint8_t incomingNbrPacketInfo[ 4 ];


//handles the incoming message from a neighbor. This usually scanns the neighborhood table and sets up stuff 

void wakeupMeOnNbrMsg() {
   int i;
   uint8_t neighbornodeid; 
   for ( i = 0; i < numOfNeighbors; i ++ ) {
      if (( nbtTable[ i ].valid == 1 ) && ( nbtTable[ i ].nodeid == incomingNbrMsg[ 0 ] )) {
         return ;
      }
   }

   neighbornodeid = incomingNbrMsg[0]; 

   if ((neighbornodeid != nodeid -1 )&&( neighbornodeid != nodeid+1))
      return; 

   nbtTable[ numOfNeighbors ].nodeid = incomingNbrMsg[ 0 ];
   nbtTable[ numOfNeighbors ].local_x = incomingNbrMsg[ 1 ];
   nbtTable[ numOfNeighbors ].local_y = incomingNbrMsg[ 2 ];
   nbtTable[ numOfNeighbors ].valid = 1;
   numOfNeighbors ++;
   return ;
}

//-------------------------------------------------------------------------
void registerNeighborhoodPacket() {

   _atomic_t currentatomic;
   radiohandletype *radiohandleaddr;
   void (*getaddrfp)(void) = (void (*)(void))SETCURRENTRADIOHANDLE; 

   radiohandleaddr = getCurrentRadioHandleAddr(); 
   
   incomingNbrLength = 0;
   //set up the radiohandleaddr data structures
   radiohandleaddr->port = 11;
   radiohandleaddr->maxLength = 16;
   radiohandleaddr->dataReady = &incomingNbrLength;
   radiohandleaddr->data = incomingNbrMsg;
   radiohandleaddr->packetinfo = incomingNbrPacketInfo;
   radiohandleaddr->handlefunc = wakeupMeOnNbrMsg;
   //close the interrupt     
   currentatomic = _atomic_start();
   //call the radio handle set to store the data structure into the handle vectors 
   getaddrfp(); 
   //open the interrupt 
   _atomic_end( currentatomic );
   return ;
}

//-------------------------------------------------------------------------
static uint8_t nbrUpdateMsg[ 16 ];

void setNeighborMsg() {
   nbrUpdateMsg[ 0 ] = nodeid;
   nbrUpdateMsg[ 1 ] = nodeid % 256;
   nbrUpdateMsg[ 2 ] = nodeid / 256;
   return ;
}

//This is the entry point. It broadcasts neighbor exchanges perodically, or handles incoming packets (non-update neighbor)

void main() {

   uint8_t destx, desty, from, to;
   uint8_t i;
   uint16_t maxdist;
   uint8_t index;
   uint8_t portnum;

   packetsremote = packetslocal = packetsdeliver = 0;

   index = 100;
   initTable();
   nodeid = getnodeID();
   
   //register both the negibhorhood update and the message incoming 

   thiscurrentthread = getCurrentThread(); 
   currentthread = *thiscurrentthread; 
   
   registerDataPacket();
   registerNeighborhoodPacket();



   while ( 1 ) {

	  if (iswakenup == 1)
	  {
       //If event occurs then skip the current sleep 
	  }
	  else
      sleepThread( NBR_UPDATE_CYCLE );
	  
      //wakes up and check the new data message?


	  if ( iswakenup == 1 ) {
	  	
         //handles the data message by restructing it and broadcast it based on the neighborhood table information 
         //sends out the new modified message to the next hop and waits for its complete
         //This must be a data packet, whether remote or local!!!

		 iswakenup = 0;

		 destx = incomingMsg[ 0 ];
         desty = incomingMsg[ 1 ];
         from = incomingMsg[ 2 ];
         to = incomingMsg[ 3 ];
         portnum = incomingMsg[ 4 ];

		 if ( to == nodeid ) {
            //StandardSocketSend( portnum, 0, incomingDataLength, incomingMsg );
            radioSend(portnum, 0, incomingDataLength, incomingMsg); 
			packetsdeliver ++; 
         }
          else
        {
            index = 100;
			maxdist = 32000;
            for ( i = 0; i < numOfNeighbors; i ++ ) {
               if ( nbtTable[ i ].valid == 1 ) {
                  uint16_t tempdist;
                  tempdist = ( nbtTable[ i ].local_x - destx )*( nbtTable[ i ].local_x - destx ) + ( nbtTable[ i ].local_y - desty )*( nbtTable[ i ].local_y - desty );
                  if ( tempdist < maxdist ) {
                     index = i;
                     maxdist = tempdist;
                  }
               }
            }
            if ( index != 100 ) {
               radioSend( 10, nbtTable[ index ].nodeid, incomingDataLength, incomingMsg );
			   greenToggle(); 
               incomingDataLength = 0;
               if (from == nodeid)
			   	packetslocal ++;
			   else 
			   packetsremote ++; 
            }
         }
      }
       else 

	     {
         //sends out hte neighborhood update message in case the previous action has used up the time slot anyway. The next
         //sleep will be another NBR_UPDATE_CYCLE 

		 setNeighborMsg();
         radioSend(11, 0xffff, 16, nbrUpdateMsg);  	
		 
         }
   }
}

