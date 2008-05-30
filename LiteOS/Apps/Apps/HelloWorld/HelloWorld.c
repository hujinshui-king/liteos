#include "leds.h"
#include "thread.h"
#include "radio.h"
#include "stringutil.h"
#include "serial.h"




int main()
{
    int i;
	//or (i=0;i<100;i++)

 /*	
   sbi(MCUCR, SM0);
   sbi(MCUCR, SM1);
   cbi(MCUCR, SM2); 
   sbi(MCUCR, SE);
   
 */ 
      
	while (1)
	{
  radioSend_string("Hello, world!\n"); 
  sleepThread(100);
	}
	return 0; 
}


