#include "leds.h"
#include "system.h"
#include "liteoscommon.h"
#include "thread.h"
#include "radio.h"
#include "stringutil.h"
#include "serial.h"
#include "file.h"
#include "eeprom.h"
#include "adc.h"



uint8_t ramstack[60]; 

void loop()
{
  __asm__ __volatile__("sei" ::);
  while (1)
  { sleepThread(100);}
  		
}


int main()
{
    __asm__ __volatile__("sei" ::);
    	    
    createThread(loop, ramstack, ramstack+60, 1, "loopme");
    
    while (1)
    {
    	 sleepThread(100);
    }
    
    return 0; 
}



    