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
#include "../types/types.h"
#ifdef PLATFORM_AVR
#include "../platform/avr/avrhardware.h"
#endif
#include "nodeconfig.h"
#include "bytestorage.h"
//Global variable!!! 
//This is the id of the current node. Every time it is modified, has to be written back into the 
//bytestorage for synchronization 
//Assuming that bytestorage is non-volatile, then this id could be kept across system reboots. 
//It is not hardcoded into the program so that it can be modifed using user commands 
uint16_t CURRENT_NODE_ID;

//The network name that this node belongs to 
char networkid[17];

//The name of this particular node 
char filenameid[17];
void node_writenodeid(uint16_t nodeid)
{
    write16uint(NODEIDOFFSET, nodeid);
}

//-------------------------------------------------------------------------
uint16_t node_readnodeid()
{
    return read16uint(NODEIDOFFSET);
}

//-------------------------------------------------------------------------
void node_setinitstatus(uint8_t status)
{
    write8uint(NODESTATUSOFFSET, status);
}

//-------------------------------------------------------------------------
uint8_t node_readinitstatus()
{
    return read8uint(NODESTATUSOFFSET);
}

//-------------------------------------------------------------------------
void node_setradiochannel(uint8_t channel)
{
    write8uint(NODECHANNELOFFSET, channel);
}

//-------------------------------------------------------------------------
uint8_t node_getradiochannel()
{
    return read8uint(NODECHANNELOFFSET);
}

//-------------------------------------------------------------------------
void resetNode()
{
#ifdef PLATFORM_AVR
    avr_resetNode();
#endif
}

//-------------------------------------------------------------------------
inline void nodeSleep()
{
#ifdef PLATFORM_AVR
    _avr_sleep();
#endif
}

//-------------------------------------------------------------------------
inline void _enable_interrupt()
{
#ifdef PLATFORM_AVR
    _avr_enable_interrupt();
#endif
}

//-------------------------------------------------------------------------
inline void _disable_interrupt()
{
#ifdef PLATFORM_AVR
    _avr_disable_interrupt();
#endif
}
