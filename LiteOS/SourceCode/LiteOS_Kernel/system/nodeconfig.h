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
extern char networkid[ 17 ];


/** \ingroup nodeconfig 
    The file name that this node belongs to 
*/
extern char filenameid[ 17 ];



    
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

