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

int main()
{
 uint16_t nodeid; 
 
 uint8_t destx, desty; 
 counter = 0; 
 
 fromnode = tonode = 100;
 
 
 __asm__ __volatile__("sei" ::); 
 
    nodeid = getnodeID();
	counter = 0; 
	
	

  while (1)
  	{
  	 
      if (nodeid == fromnode)
      {    

            destx = tonode%256;
            
			desty = tonode/256;
			counter++; 
			msg[0] = destx;
			msg[1] = desty; 
			msg[2] = fromnode; 
			msg[3] = tonode;
			msg[4] = 80;
			msg[5] = 0xff;
			msg[6] = 0xff;
			msg[7] = counter; 
		
			msg[8] = 'h';
			msg[9] = 'e';
			msg[10] = 'l';
			msg[11] = 'l';
			msg[12] = 'o';
			msg[13] = 0;
			
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
