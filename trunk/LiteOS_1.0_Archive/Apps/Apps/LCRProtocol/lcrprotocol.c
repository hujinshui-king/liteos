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
 
//This is a prototype implementation of the LCR routing protocol that uses logical coordinates to deliver packets
//in wireless sensor networks, it uses a series of landmakrs to send out beacons, which are used to construct
//logical coordinates of nodes, which are in turn used for forwarding packets. 
//Registers port 10 for data packets, port 11 for neighbor broadcasts , port 12 for lcr protocol packets (from landmarks)


#include "leds.h"
#include "thread.h"
#include "adc.h"
#include "radio.h"
#include "serial.h"
#include "file.h"
#include "system.h"
#include "thread.h"

//this is the nodeid of the current node. 

static uint16_t nodeid;

//Pointer to the current thread data structure. Useful for waking up the current thread. 

thread **thiscurrentthread;
thread *currentthread;

uint16_t packetslocal;
uint16_t packetsremote;
uint16_t packetsdeliver;


//logical coordinates of the current node, at the beginning set as -1 

uint8_t coordinates[4];

//The neighbor table

typedef struct
{
    uint16_t nodeid;
    uint8_t coord[4];
    uint8_t valid;
} nbr_table_entry;

//The maximum size of the neighbor table 

#define NBR_SIZE 5

//The current number of neighbors
uint8_t numOfNeighbors;

//Neighbor table 
static nbr_table_entry nbtTable[NBR_SIZE];

//Find out whether there is packet pending or not, and for what reason, wake up, either 1 or 2 , which means data, or lcr

static uint8_t iswakenup;

//Every five seconds, send out neighbor update message (beacons)
#define NBR_UPDATE_CYCLE 5000

//landmarknode says whether this is a landmark or not 
//landmarkupdatecount says whether this is a cycle for sending out the flood message 
//by default, the landmark node will be 1, 2, 3, or 4 
//the landmark count is set as 5 and decreased for each neighbor update cycle 

static uint8_t landmarknode;
static uint8_t landmarkupdatecount;



//This function initlizes the neighhood table 
void initTable()
{
    int i;
    for (i = 0; i < NBR_SIZE; i++)
    {
        nbtTable[i].valid = 0;
    }
    numOfNeighbors = 0;
    iswakenup = 0;
}

//-------------------------------------------------------------------------
void initLCRParameters()
{
    landmarknode = 0;
    landmarkupdatecount = 5;
    coordinates[0] = coordinates[1] = coordinates[2] = coordinates[3] = 100;
}

//-------------------------------------------------------------------------
uint8_t abs(uint8_t a, uint8_t b)
{
    if (a > b)
    {
        return a - b;
    }
    else if (a < b)
    {
        return b - a;
    }
    else
    {
        return 0;
    }
}

//-------------------------------------------------------------------------
uint8_t checkIfNeighbor(uint8_t nodeid, uint8_t currentid)
{
    uint8_t nodex, nodey;
    uint8_t currentx, currenty;

    nodex = (nodeid - 1) % 3;
    nodey = (nodeid - 1) / 3;
    currentx = (currentid - 1) % 3;
    currenty = (currentid - 1) / 3;
    if (abs(nodex, currentx)  + abs(nodey, currenty) <= 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//-------------------------------------------------------------------------
static uint8_t incomingMsg[32];
static uint8_t incomingDataLength;
static uint8_t incomingPacketInfo[4];

//Handles incoming data msg. This usually wakesup the current thread 
void wakeupMeOnDataMsg()
{
    //(*thiscurrentthread)->state = STATE_ACTIVE;
    currentthread->state = STATE_ACTIVE;
    iswakenup = 1;
    syscall_postThreadTask();
}

//Registers the data packet for receving incoming packets. This enables receving both remote and local data packets
void registerDataPacket()
{
    _atomic_t currentatomic;
    radiohandletype *radiohandleaddr;
    void (*getaddrfp) (void) = (void (*)(void))REGISTER_RADIO_RECEIVE_EVENT;

    //Finds the radio handle, so that when this code is ported to be externally used, it will be obtained via a system call 
    radiohandleaddr = getCurrentRadioHandleAddr();
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
    _atomic_end(currentatomic);
    return;
}

//-------------------------------------------------------------------------
static uint8_t incomingNbrMsg[16];
static uint8_t incomingNbrLength;
static uint8_t incomingNbrPacketInfo[4];

//handles the incoming message from a neighbor. This usually scanns the neighborhood table and sets up stuff 
void wakeupMeOnNbrMsg()
{
    int i;
    uint8_t currentid;
    uint8_t neighbornodeid;
    uint8_t index;

    neighbornodeid = incomingNbrMsg[0];

    currentid = getnodeID();

    if (checkIfNeighbor(neighbornodeid, currentid) == 0)
    {
        return;
    }

    index = numOfNeighbors;

    for (i = 0; i < numOfNeighbors; i++)
    {
        if ((nbtTable[i].valid == 1) && (nbtTable[i].nodeid ==
                                         incomingNbrMsg[0]))
        {
            index = i;
            break;
        }
    }
    nbtTable[index].nodeid = incomingNbrMsg[0];
    nbtTable[index].coord[0] = incomingNbrMsg[1];
    nbtTable[index].coord[1] = incomingNbrMsg[2];
    nbtTable[index].coord[2] = incomingNbrMsg[3];
    nbtTable[index].coord[3] = incomingNbrMsg[4];
    nbtTable[index].valid = 1;
    
    if (index == numOfNeighbors)
    {
        numOfNeighbors++;
    }
    
    if (coordinates[0]>incomingNbrMsg[1]+1)
    	 coordinates[0] = incomingNbrMsg[1]+1; 
    	 
   
    if (coordinates[1]>incomingNbrMsg[2]+1)
    	 coordinates[1] = incomingNbrMsg[2]+1; 
    	 
    	 
    if (coordinates[2]>incomingNbrMsg[3]+1)
    	 coordinates[2] = incomingNbrMsg[3]+1; 
    	 
    if (coordinates[3]>incomingNbrMsg[4]+1)
    	 coordinates[3] = incomingNbrMsg[4]+1; 
    return;
}

//-------------------------------------------------------------------------
void registerNeighborhoodPacket()
{
    _atomic_t currentatomic;
    radiohandletype *radiohandleaddr;
    void (*getaddrfp) (void) = (void (*)(void))REGISTER_RADIO_RECEIVE_EVENT;

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
    _atomic_end(currentatomic);
    return;
}


//-------------------------------------------------------------------------
static uint8_t nbrUpdateMsg[16];
void setNeighborMsg()
{
    nbrUpdateMsg[0] = nodeid;
    nbrUpdateMsg[1] = coordinates[0];
    nbrUpdateMsg[2] = coordinates[1];
    nbrUpdateMsg[3] = coordinates[2];
    nbrUpdateMsg[4] = coordinates[3];
    return;
}


//-------------------------------------------------------------------------
static uint8_t landmarkMsg[16];
void setLandmarkMsg()
{
    landmarkMsg[0] = nodeid;
    landmarkMsg[1] = landmarknode;
    coordinates[landmarknode-1] = 0; 
    landmarkMsg[2] = 0;
    return;
}

//-------------------------------------------------------------------------
static uint8_t incomingLandmarkMsg[16];
static uint8_t incomingLandmarkLength;
static uint8_t incomingLandmarkPacketInfo[4];

//handles the incoming message from lcr landmark 

void wakeupMeOnLandmarkMsg()
{
    //(*thiscurrentthread)->state = STATE_ACTIVE;
    currentthread->state = STATE_ACTIVE;
    iswakenup = 2;
    syscall_postThreadTask();
}

//-------------------------------------------------------------------------
void registerLandmarkPacket()
{
    _atomic_t currentatomic;
    radiohandletype *radiohandleaddr;
    void (*getaddrfp) (void) = (void (*)(void))REGISTER_RADIO_RECEIVE_EVENT;

    radiohandleaddr = getCurrentRadioHandleAddr();
    incomingLandmarkLength = 0;
    //set up the radiohandleaddr data structures
    radiohandleaddr->port = 12;
    radiohandleaddr->maxLength = 16;
    radiohandleaddr->dataReady = &incomingLandmarkLength;
    radiohandleaddr->data = incomingLandmarkMsg;
    radiohandleaddr->packetinfo = incomingLandmarkPacketInfo;
    radiohandleaddr->handlefunc = wakeupMeOnLandmarkMsg;
    //close the interrupt     
    currentatomic = _atomic_start();
    //call the radio handle set to store the data structure into the handle vectors 
    getaddrfp();
    //open the interrupt 
    _atomic_end(currentatomic);
    return;
}





//This is the entry point. It broadcasts neighbor exchanges perodically, or handles incoming packets (non-update neighbor)
void main()
{
    uint8_t from, to;
    uint8_t i;
    uint16_t maxdist;
    uint8_t index;
    uint8_t dest1, dest2, dest3, dest0; 
    uint8_t portnum;
    
    uint8_t fromnode, landmarkindex, currenthop; 

    packetsremote = packetslocal = packetsdeliver = 0;
    
    index = 100;
    
    
    initTable();
    initLCRParameters(); 
    nodeid = getnodeID();
    
    
    //register both the negibhorhood update and the message incoming 
    thiscurrentthread = getCurrentThread();
    currentthread = *thiscurrentthread;
    registerDataPacket();
    registerNeighborhoodPacket();
    registerLandmarkPacket();
    
    while (1)
    {
        if ((iswakenup == 1)||(iswakenup == 2))
        {
            //If event occurs then skip the current sleep 
        }
        else
        {
            sleepThread(NBR_UPDATE_CYCLE);
        }
        //wakes up and check the new data message?
        if (iswakenup == 1)
        {
            //handles the data message by restructing it and broadcast it based on the neighborhood table information 
            //sends out the new modified message to the next hop and waits for its complete
            //This must be a data packet, whether remote or local!!!
            iswakenup = 0;
            dest0 = incomingMsg[0];
            dest1 = incomingMsg[1];
            dest2 = incomingMsg[2];
            dest3 = incomingMsg[3];
            
            from = incomingMsg[4];
            to = incomingMsg[5];
            portnum = incomingMsg[6];
            if (to == nodeid)
            {
                //StandardSocketSend( portnum, 0, incomingDataLength, incomingMsg );
                radioSend(portnum, 0, incomingDataLength, incomingMsg);
                packetsdeliver++;
            }
            else
            {
                index = 100;
                maxdist = 32000;
                for (i = 0; i < numOfNeighbors; i++)
                {
                    if (nbtTable[i].valid == 1)
                    {
                        uint16_t tempdist;


                        if (nbtTable[i].nodeid == to)
                        	{ index = i;
                        	  break;
                        	}
                        tempdist =
                            abs(nbtTable[i].coord[0],
                             dest0) * abs(nbtTable[i].coord[0], dest0) +
                             abs(nbtTable[i].coord[1],
                             dest1) * abs(nbtTable[i].coord[1] , dest1) +
                              abs(nbtTable[i].coord[2] ,
                             dest2) * abs(nbtTable[i].coord[2] , dest2) +
                              abs(nbtTable[i].coord[3] ,
                             dest3) * abs(nbtTable[i].coord[3] , dest3) ;
                             
                             
                        if (tempdist < maxdist)
                        {
                            index = i;
                            maxdist = tempdist;
                        }
                    }
                }
                
                incomingMsg[7] = index; 
                
                if (index != 100)
                {
                    radioSend(10, nbtTable[index].nodeid, incomingDataLength,
                              incomingMsg);
                    greenToggle();
                    incomingDataLength = 0;
                    if (from == nodeid)
                    {
                        packetslocal++;
                    }
                    else
                    {
                        packetsremote++;
                    }
                }
            }
        }
        
        else if (iswakenup == 2)
        {
        	  //now a landmark message has been received. Handle it. 
        	  
        	  iswakenup = 0;
            fromnode = incomingLandmarkMsg[0];
            landmarkindex = incomingLandmarkMsg[1];
            currenthop = incomingLandmarkMsg[2];
               
               
            if (checkIfNeighbor(nodeid, fromnode)==0)
            	 continue;             	         
            
            if (currenthop+1<coordinates[landmarkindex-1])
            {
            	  uint8_t jitter; 
            	  coordinates[landmarkindex-1] = currenthop+1; 
                jitter = rnd() % 20;
                sleepThread(jitter);
                incomingLandmarkMsg[0] = nodeid; 
                incomingLandmarkMsg[2]++; 
                radioSend(12, 0xffff, 16, incomingLandmarkMsg);
            }
        }
        else
        {
            //sends out hte neighborhood update message in case the previous action has used up the time slot anyway. The next
            //sleep will be another NBR_UPDATE_CYCLE 
            
            setNeighborMsg();
            radioSend(11, 0xffff, 16, nbrUpdateMsg);
            
            if (landmarknode > 0)
                if ((--landmarkupdatecount) == 0)
                {
                    landmarkupdatecount = 5;
                    setLandmarkMsg();
                    radioSend(12, 0xffff, 16, landmarkMsg);
                }
        }
    }
}
