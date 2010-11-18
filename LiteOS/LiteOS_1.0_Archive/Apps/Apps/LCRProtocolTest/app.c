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
volatile uint8_t counter; 
volatile uint8_t coord0, coord1, coord2, coord3; 
int main()
{
 uint16_t nodeid; 
 

 
 counter = 0; 
 
 fromnode = tonode = 100;
 
 
 __asm__ __volatile__("sei" ::); 
 
  nodeid = getnodeID();
	counter = 0; 
	
	

  while (1)
  	{
  	 
      if (nodeid == fromnode)
      {    

  
			counter++; 
			msg[0] = coord0; 
			msg[1] = coord1;
			msg[2] = coord2;
			msg[3] = coord3; 
			 
			msg[4] = fromnode; 
			msg[5] = tonode;
			msg[6] = 80;
			msg[7] = 0xff;
			msg[8] = 0xff;
			msg[9] = counter; 
		
			msg[10] = 'h';
			msg[11] = 'e';
			msg[12] = 'l';
			msg[13] = 'l';
			msg[14] = 'o';
			msg[15] = 0;
			
	    radioSend(10, 0, 16, msg);
			redToggle(); 
			 
           
      }
	else if (nodeid == tonode)
	  {
			    radioReceive(80, 16, msg); 
		        serialSend(16, msg); 
				yellowToggle();
				
	  }
	  sleepThread(1000);
  	}	 
	return 0; 
}
