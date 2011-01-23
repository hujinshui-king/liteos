#include "leds.h"
#include "thread.h"
#include "adc.h"
#include "radio.h"
#include "serial.h"
#include "file.h"
#include "system.h"




 
int main()
{
   uint16_t nodeid; 
   uint8_t *content = "hello, can you hear me?";
   uint8_t receivemsg[30]; 
   
   __asm__ __volatile__("sei" ::); 

   nodeid =  getnodeID();
   if (nodeid == 3)
   	{
       while (1)
       	{
         radioSend(10, 0xFFFF, 10, content); 
     		 sleepThread(1000); 

	    }

    }
   
   else
   	{
       while (1)
       	{
		radioReceive(10, 30, receivemsg); 
		serialSend(30, receivemsg); 
       	}
		


   	}
   	

	


 
	return 0; 
}

