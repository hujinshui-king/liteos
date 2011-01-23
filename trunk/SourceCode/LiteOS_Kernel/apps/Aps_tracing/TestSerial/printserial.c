#include "leds.h"
#include "thread.h"
#include "adc.h"
#include "radio.h"
#include "serial.h"
#include "file.h"
#include "system.h"

uint8_t i; 

int main()
{
 uint16_t reading; 

 
 __asm__ __volatile__("sei" ::); 
 
   for (i =0; i< 100; i++)
   	{
	   redToggle(); 
     serialSend_string("This is a message");
     serialSend_uint16(124);

     serialSend(12, "This is another message");
	   sleepThread(100);
    }
   
	return 0; 
}

