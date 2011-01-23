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


uint8_t datainternal[32];
uint8_t datainternal2[32]; 


int main()
{
    int i;
    __asm__ __volatile__("sei" ::);
    for (i=0;i<32;i++)
     datainternal[i] = i * 3; 
    
    writeToEEPROM(3500, 32, datainternal);
    
    readFromEEPROM(3500, 32, datainternal2);
    
    serialSend(32, datainternal2); 
    
      for (i=0;i<32;i++)
     datainternal[i] = i * 5; 
    
    writeToEEPROM(3700, 32, datainternal);
    
    readFromEEPROM(3700, 32, datainternal2);
    
    serialSend(32, datainternal2); 
    
      for (i=0;i<32;i++)
     datainternal[i] = i * 8; 
    
    writeToEEPROM(4000, 32, datainternal);
    
    readFromEEPROM(4000, 32, datainternal2);
    
    serialSend(32, datainternal2); 
    
    
        
  
	  return 0; 
}


