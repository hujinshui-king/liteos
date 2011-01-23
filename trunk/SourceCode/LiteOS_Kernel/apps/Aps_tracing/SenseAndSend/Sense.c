
#include "leds.h"
#include "system.h"
#include "liteoscommon.h"
#include "thread.h"
#include "radio.h"
#include "stringutil.h"
#include "file.h"
#include "adc.h"


enum {
  BUFFER_SIZE = 10
};

struct OscopeMsg
{
    uint16_t sourceMoteID;
    uint16_t lastSampleNumber;
    uint16_t channel;
    uint16_t data[BUFFER_SIZE];
};


/*
struct OscopeResetMsg
{
     Empty payload! 
*/



enum {
  AM_OSCOPEMSG = 10,
  AM_OSCOPERESETMSG = 32
};


uint16_t reading;
uint8_t packetReadingNumber;
uint16_t readingNumber;

struct OscopeMsg DataMsg; 

int main()
{ 

   struct OscopeMsg *pack;
   
   __asm__ __volatile__("sei" ::);
  
  
   packetReadingNumber = 0;
   readingNumber = 0;
   
   
   
   while (1)
   {
   	  sleepThread(200);
      reading = get_light(); 
      
		  
	    pack = &DataMsg;
      pack->data[packetReadingNumber++] = reading ;
      readingNumber++;

      if (packetReadingNumber == BUFFER_SIZE) {
        	 pack->lastSampleNumber = readingNumber;
        	 packetReadingNumber = 0;
        	 pack->channel = 1;
           pack->sourceMoteID = 1;
        	 radioSend(0x7d0a, 0x007e, sizeof(struct OscopeMsg), &DataMsg);
      }
    }
 
    if (reading > 0x0300)
       redOn();
    else
       redOff();
 
 
   	
return 0;    	
}

