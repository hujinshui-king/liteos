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
#ifndef NODECONFIGH
#define NODECONFIGH

/**\defgroup nodeconfig  Hardware-independent node abstraction*/
#include "../platform/avr/avrhardware.h"

/** \ingroup nodeconfig 
Node id, assumed to be 16-bit
*/
extern uint16_t CURRENT_NODE_ID;

/** \ingroup nodeconfig 
The network name that this node belongs to 
*/
extern char networkid[17];

/** \ingroup nodeconfig 
The file name that this node belongs to 
*/
extern char filenameid[17];

#define NETWORKNAMEOFFSET 3104
#define NODEFILENAMEOFFSET    3125
#define NODEIDOFFSET 3150
#define NODESTATUSOFFSET 3154
#define NODECHANNELOFFSET  3156
//EEPROM vector contains 12 bytes
#define EEPROMVECTORSTART 3138
//flash vector contains 32 bytes 
#define FLASHVECTORSTART  3150

/** \ingroup nodeconfig 
Write the node id into byte storage
*/
void node_writenodeid(uint16_t nodeid);

/** \ingroup nodeconfig 
Read the node id from byte storage
*/
uint16_t node_readnodeid();

/** \ingroup nodeconfig 
Set the current status of this node 
*/
void node_setinitstatus(uint8_t status);

/** \ingroup nodeconfig 
Read the node status. 
*/
uint8_t node_readinitstatus();

/** \ingroup nodeconfig 
Write the channel of the current node into byte storage
*/
void node_setradiochannel(uint8_t channel);

/** \ingroup nodeconfig 
Get the node channel from byte storage 
*/
uint8_t node_getradiochannel();

/** \ingroup nodeconfig
Sleep the current node
*/
inline void nodeSleep();

/** \ingroup nodeconfig
Enable interrupt of current node
*/
inline void _enable_interrupt();

/** \ingroup nodeconfig
Disable interrupt of the current node
*/
inline void _disable_interrupt();

/** \ingroup nodeconfig
Reset the current node. 
*/
void resetNode();
#endif
