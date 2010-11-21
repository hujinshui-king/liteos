#include "../../libraries/libleds.h"
#include "../../libraries/libthread.h"
#include "../../libraries/libsystem.h"
#include "../../libraries/libserial.h"
#include "../../types/types.h"
#include "../../libraries/libradio.h"
#include "../../libraries/libfile.h"

 
uint8_t blinkbuffer[250]; 
int i; 
LIB_MYFILE *filefp; 
uint16_t counter; 

uint16_t addr; 

void blink()


{
    int i;
	uint8_t *addrptr; 
	//or (i=0;i<100;i++)

 /*	
   sbi(MCUCR, SM0);
   sbi(MCUCR, SM1);
   cbi(MCUCR, SM2); 
   sbi(MCUCR, SE);
   
 */ addrptr = 0; 
    lib_sleep_thread(30000);  
	while (1)
	{
  lib_green_toggle();
  
 // serialSend_string("hello\n");
 // serialSend_uint16(32323); 
  //lib_radio_send_string("hello, world\n"); 
   lib_radio_send_msg(12, 0xFFFF, 80, (uint8_t*)addrptr);
  //thread_rollback_snapshot(i);
  lib_sleep_thread(50);
	}
	return 0; 
}



void blink2()
{

   char *str = "hello,world"; 
   uint8_t *addrptr; 
   
   addr = 256; 
/* 
   filefp =  lib_mfopen("/hello", "w");
   for (i=0;i<100;i++)
{   lib_mfwrite(filefp, str, 10);
   lib_mfseek(filefp, 10, 1);
}
   lib_mfclose(filefp); 
*/
	 
   for (counter=0; counter<3000; counter++)
   	{
   	 if (counter<1000)
	   lib_red_toggle();   
     else if (counter <2000)
	    lib_yellow_toggle();
		else
		 lib_green_toggle();

     addrptr = (uint8_t*)addr; 
     
   	 if (addr<4000)
   	  lib_radio_send_msg(12, 0xFFFF, 80, (uint8_t*)addrptr);

	  
   	 addr+=80; 
     
     if (addr >4000)
     	addr = 0;
     lib_sleep_thread(1000);
     if (counter == 2999)
     	counter = 0; 
	   }
  	return; 
}

