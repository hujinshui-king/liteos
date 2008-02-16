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
char networkid[ 17 ];

//The name of this particular node 
char filenameid[ 17 ];


void node_writenodeid(uint16_t nodeid) {
	
	   write16uint(NODEIDOFFSET, nodeid);              

}


uint16_t node_readnodeid(){
	
		     return read16uint(NODEIDOFFSET); 	

}


void node_setinitstatus(uint8_t status)
{
	write8uint(NODESTATUSOFFSET, status);
	
}


uint8_t node_readinitstatus() 
{
	return read8uint(NODESTATUSOFFSET); 
}

void node_setradiochannel(uint8_t channel)
{
	write8uint(NODECHANNELOFFSET, channel);
	
}


uint8_t node_getradiochannel() 
{
	return read8uint(NODECHANNELOFFSET); 
}



void resetNode() {
	
#ifdef PLATFORM_AVR
   avr_resetNode(); 
#endif

}

inline void nodeSleep(){
#ifdef PLATFORM_AVR
	 _avr_sleep(); 
	 #endif
}



inline void _enable_interrupt() {
	 #ifdef PLATFORM_AVR
	  _avr_enable_interrupt(); 
	 #endif
	}	


inline void _disable_interrupt() {
	 #ifdef PLATFORM_AVR
	   _avr_disable_interrupt();
	 #endif
	}



