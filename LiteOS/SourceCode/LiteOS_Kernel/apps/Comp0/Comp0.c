#include "../../libraries/libleds.h"
#include "../../libraries/libthread.h"
#include "../../libraries/libsystem.h"
#include "../../libraries/libserial.h"
#include "../../libraries/commonapp.h"

uint8_t compbuffer[200];

void comp0()
{
  uint16_t index;
  uint8_t index2; 
  lib_thread **current_thread;
 
  __asm__ __volatile__("sei" ::); 
 

	  
  current_thread = lib_get_current_thread();  
    
   (*current_thread)->ecbptr->share = 500; 
   (*current_thread)->ecbptr->totalenergy =  7500000; 
   (*current_thread)->ecbptr->remainenergy = 7500000; 
     
  /* sbi(MCUCR, SM0);

   sbi(MCUCR, SM1);
   cbi(MCUCR, SM2); 
   sbi(MCUCR, SE);
   
*/
   
   while(1)
   	{
	   
	   for (index=0;index<1200;index++)
	    for (index2=0;index2<123;index2++)
	        {
	        	__asm__ __volatile__("nop" ::); 
	        }
	   lib_yield(); 
    }
   
	return; 
}

