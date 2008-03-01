/* LiteOS Version 0.3 */
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


//Generic inclusions

#include "../avr/avrhardware.h"
#include "micazhardware.h"
#include "../../types/types.h"
#include "../../system/threads.h"
#include "../../system/scheduling.h"
#include "../../system/amradio.h"
#include "../../system/socket.h"
#include "../../filesys/inode.h"
#include "../../filesys/fsapi.h"
#include "../../system/commandhandle.h"
#include "../avr/bootloader.h"
#include "adcdriver.h"
#include "../../filesys/filesocket.h"
#include "realmain.h"
#include "../../system/nodeconfig.h"
#include "../../system/bytestorage.h"
#include "../../system/stdserial.h"
#include "../../filesys/vectorflash.h"
#include "../../filesys/vectornode.h"
#include "../../types/string.h"
#include "../../system/generictimer.h"
#include "../../utilities/eventlogger.h"


//MicaZ specific inclusions
#include <stdlib.h>
#include "../../system/packethandler.h"
#include "../../io/cc2420/cc2420controlm.h"
#include "leds.h"



#define MICAZCONFIGMESSAGERECEIVED 10
#define MICAZ_INIT_READY           11

static uint16_t nodeid; 

//This function receives init command in MicaZ installation task, and sets up the node status for reboot, so that the fiel system will
//be reformatted 


void initCommandReceived( uint8_t *receivebuffer ) {
  
   uint8_t i;
   
   genericwriteBytes ( NETWORKNAMEOFFSET, 16,  &receivebuffer[ 0 ] );
   genericwriteBytes ( NODEFILENAMEOFFSET,    15,  &receivebuffer[ 16 ] );
   node_setradiochannel( receivebuffer[31] ); 
   node_writenodeid ( receivebuffer[ 32 ] );
   node_setinitstatus (MICAZCONFIGMESSAGERECEIVED);
   
   for ( i = 0; i < 32; i ++ ) {
      printfstr ( '\0' );
   } 
  
   resetNode();
}


//The platform specific entry point. This is the place where the program gets executed. 
//Every time the system gets compiled, there should be at most one such entry point. 

int main() {

   //micaz specific initilizations
   
   LITE_SET_PIN_DIRECTIONS();
   adcdriver_init_adc();
   
   //system (hardware independent) init
   
   initScheduling();
   thread_init();

   //printf is expected to be connected to serial port in MicaZ. But this is 
   //going to be system independent, and may connect to radio on platforms where 
   //serial port is not available 
   
   printfstr( "System start!\n" );
   
   _avr_enable_interrupt(); 
   
   //for the following, read from else section, which defaults to the 0xff as the MicaZ is first initied by reprogramming
   
   
   if ( node_readinitstatus() == MICAZCONFIGMESSAGERECEIVED)
    {
    	//current_node_id is expected to be platform independent variable 
   
    	
      CURRENT_NODE_ID = node_readnodeid();
      srand( CURRENT_NODE_ID );
      
      genericreadBytes( NETWORKNAMEOFFSET, 16, networkid );
      genericreadBytes( NODEFILENAMEOFFSET, 16, filenameid );
      
    
      
      nodeid = CURRENT_NODE_ID;
      
      networkid[ 16 ] = '\0';
      filenameid[ 16 ] = '\0';
      
      Leds_redToggle();
      Leds_greenToggle();
      Leds_yellowToggle();
      
      formatSystem();
      
      buildRootNode();
      buildDeviceDirectory();
      
      Leds_redToggle();
      Leds_greenToggle();
      Leds_yellowToggle();
	  
	 // node_setinitstatus(MICAZ_INIT_READY); 
	 node_setinitstatus(MICAZCONFIGMESSAGERECEIVED); 
   }
   else if (node_readinitstatus() == MICAZ_INIT_READY)
   {
      
      CURRENT_NODE_ID = node_readnodeid();
      srand( CURRENT_NODE_ID );
      genericreadBytes( NETWORKNAMEOFFSET, 16, networkid );
      genericreadBytes( NODEFILENAMEOFFSET, 16, filenameid );
      nodeid = CURRENT_NODE_ID;
      
      networkid[ 16 ] = '\0';
      filenameid[ 16 ] = '\0';
      
      Leds_redToggle();
      Leds_greenToggle();
      Leds_yellowToggle();
      
      //formatSystem();
      //buildRootNode();
      //buildDeviceDirectory();

   	  readVectorFlashFromExternalStorage();
   	  readVectorNodeFromExternalStorage(); 
      
      Leds_redToggle();
      Leds_greenToggle();
      Leds_yellowToggle();
	  
	  //node_setinitstatus(MICAZ_INIT_READY);  
   
   
   }
   
   //entry point. Every time the system is booted, start from here. 
   else
    {   
      
      /* The following is for debugging the kernel , where no reprogramming overboard is used 
      */
	  /*
	    Leds_redToggle();
      Leds_greenToggle();
      Leds_yellowToggle();
      mystrncpy( networkid, "d01\0", 4 );
      mystrncpy( filenameid, "node1\0", 6 );
	    CURRENT_NODE_ID = 1;
      nodeid = CURRENT_NODE_ID; 
	    srand( nodeid );
	    formatSystem();
      buildRootNode();
      buildDeviceDirectory();
	    genericwriteBytes ( NETWORKNAMEOFFSET, 16, networkid );
      genericwriteBytes ( NODEFILENAMEOFFSET,    16,  filenameid );
      node_writenodeid ( nodeid );
	    node_setinitstatus(MICAZCONFIGMESSAGERECEIVED); 
	    node_setradiochannel(15); 
	    Leds_redToggle();
      Leds_greenToggle();
      Leds_yellowToggle();
      
	 */

    }
   
   
   //Init everything 
   
   GenericTimerInit();
   
   filesocketInit();
   
   AMStandard_Control_init();
   AMStandard_Control_start();
   
   SocketInit();


   initRadioHandle();

   InitShell();

   initTrace(); 

  {
   uint8_t currentchannel;  
   currentchannel = node_getradiochannel(); 
   cc2420controlm_CC2420Control_TuneChannel( currentchannel ); 
  }
   cc2420controlm_CC2420Control_TunePower( 31 ); 
   
   create_thread( ShellThread, ( uint16_t* )shellbuffer, STACK_TOP( shellbuffer ), 0, 15, "sysshell", 0, 0 );
   
   _avr_enable_interrupt();

   while ( 1 ) {
      runNextTask();
   }
   return 0;
}
////////////////

