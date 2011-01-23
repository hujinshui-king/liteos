#include "leds.h"
#include "thread.h"
#include "adc.h"
#include "radio.h"
#include "serial.h"
#include "file.h"
#include "system.h"





void foo_stack(){
	
	uint8_t local[10];
 
	 
	return; 

}

int main()
{
 uint8_t index;  
 __asm__ __volatile__("sei" ::); 
 
 
 

while (1)
{ 
 //hibernateThread();
 //counter2++;
 //radioSend_string("Hello, world!\n"); 
 
 sleepThread(1000);
 redToggle();
 foo_stack();
  
}
return 0; 
}

