#include "leds.h"
#include "system.h"
#include "liteoscommon.h"
#include "thread.h"
#include "radio.h"
#include "stringutil.h"
#include "file.h"
#include "adc.h"




int main()
{ 

 uint8_t channel;
 
 for (channel = 11;channel <25;channel++)
 { setRadioChannel(channel); 
  
 radioSend_string("Hello, world!\n"); 
 greenToggle();
 sleepThread(1000);

 }
 
 channel = 15; 
 setRadioChannel(channel); 
 return 0; 

}

