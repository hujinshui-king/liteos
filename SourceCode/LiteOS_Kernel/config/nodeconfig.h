#ifndef NODECONFIGH
#define NODECONFIGH
#include "../types/types.h"


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


/** \ingroup nodeconfig 
Write the node id into byte storage
*/
void node_writenodeid(uint16_t nodeid);

/** \ingroup nodeconfig 
Read the node id from byte storage
*/
uint16_t node_readnodeid();

char *node_readnodestring();

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


#endif
