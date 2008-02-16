#include "leds.h"
#include "thread.h"
#include "adc.h"
#include "radio.h"
#include "Serial.h"
#include "File.h"
#include "System.h"



int main()
{
 uint8_t index;  
 __asm__ __volatile__("sei" ::); 
 
   for (index = 0;index <100;index++)
   	{
	   greenToggle();      
	   sleepThread(100);
    }
   
	return 0; 
}

