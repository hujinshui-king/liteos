#include "leds.h"
#include "thread.h"
#include "adc.h"
#include "radio.h"
#include "serial.h"
#include "file.h"
#include "system.h"



uint8_t msg[20];
uint8_t fromnode;
uint8_t tonode; 

uint16_t nodeid; 
uint16_t sig; 
uint16_t incomingnode; 

int main()
{


 fromnode = tonode = 100;
 fromnode = 2; 
 
 
 __asm__ __volatile__("sei" ::); 
 
    nodeid = getnodeID();
	
	

  while (1)
  	{
  	 
      if (nodeid == fromnode)
      {    

      sig = rnd(); 
      msg[0] = nodeid%256;
			msg[1] = nodeid/256;
			msg[2] = sig%256;
			msg[3] = sig/256;
			msg[4] = 90; 
			msg[5] = 0xff;
			msg[6] = 0xff;
			msg[7] = 'h';
			msg[8] = 'e';
			msg[9] = 'l';
			msg[10] = 'l';
			msg[11] = 'o';
			msg[12] = 0;
	    radioSend(20, 0, 16, msg);
//			redToggle(); 
			 
           
      }
    else 
 	  {
			    radioReceive(90, 16, msg); 
		      incomingnode = (uint16_t)msg[0]+ (((uint16_t)msg[1]) *256); 
			  
			    if (incomingnode%3==0)						  
 				yellowToggle();
				else if (incomingnode%3==1)		
					redToggle();
				else
					greenToggle(); 
				
				
	  }
	sleepThread(2000);
	  
  	}	 
	return 0; 
}

