
#include "leds.h"
#include "system.h"
#include "liteoscommon.h"
#include "thread.h"
#include "radio.h"
#include "stringutil.h"
#include "file.h"
#include "rollback.h"
#include "adc.h"


/*
struct OscopeMsg
{
    uint16_t sourceMoteID;
    uint16_t lastSampleNumber;
    uint16_t channel;
    uint16_t data[BUFFER_SIZE];
};

*/
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


int main()
{ 
uint16_t i; 
   __asm__ __volatile__("sei" ::);
  
   
   i = thread_create_snapshot();
  
 
  thread_rollback_snapshot(i);
   while (1)
   {
   	  sleepThread(125);
      reading = get_light(); 
 		  radioSend(1, 0xffff, 2, &reading); 
    }

   	
return 0;    	
}

