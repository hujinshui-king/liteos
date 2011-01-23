#include "leds.h"
#include "thread.h"
#include "radio.h"
#include "stringutil.h"
#include "serialprint.h"
#include "rollback.h"




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
  greenToggle();
  radioSend_string("Hello, world!\n"); 
  //i = thread_create_snapshot();
  
 // serialSend_string("hello\n");
 // serialSend_uint16(32323); 
  printfstring("hello, world\n"); 
  //thread_rollback_snapshot(i);
  sleepThread(1000);
	}
	return 0; 
}


