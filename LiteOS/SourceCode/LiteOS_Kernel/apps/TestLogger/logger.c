#include "leds.h"
#include "system.h"
#include "liteoscommon.h"
#include "thread.h"
#include "radio.h"
#include "stringutil.h"
#include "serialprint.h"
#include "file.h"
#include "eeprom.h"
#include "adc.h"
#include "eventtrace.h"


 	
tracingblock *blockaddr; 
uint8_t *eventid; 
MYFILE *fileptr; 
uint32_t *localtimestamp; 
uint8_t trace[128]; 
uint8_t currentindex; 
uint8_t *threadid; 


thread **thiscurrentthread;
thread *thisthread; 
 
uint8_t writetask; 
            
uint8_t traceshadow[32]; 





void writeFileTask(){
	_atomic_t _atomic = _atomic_start();
		 mfwrite(fileptr, traceshadow, 32); 
		 mfseek(fileptr, 32, 1);
		 mfwrite(fileptr, &trace[32], 96); 
		 mfseek(fileptr, 96, 1);  
	_atomic_end(_atomic); 	 
}


//Shall use eventid to show the actual type of event here. 

void tracingcontrol(){
	 	
  _atomic_t _atomic = _atomic_start();
 
   if (*threadid > 1)
   {
   	uint32_t *temp;
   	 trace[currentindex++] = 1;
  	 trace[currentindex++] = *eventid; 
     trace[currentindex++] = *threadid; 
     temp = (uint32_t*)&trace[currentindex];
     currentindex+=4;
     *temp = *localtimestamp; 
     
   }
   
 
 if (currentindex >= 120)

  	{
	     //Now write the trace into the file and clean it up 
	   //This is critical becuase more events will happen when the write occurs. 
	   uint8_t i;
	   for (i=0;i<32;i++)
	      traceshadow[i] = trace[i]; 
	   currentindex = 0; 

     
     thisthread->state = STATE_ACTIVE; 
      syscall_postThreadTask_nologging();  
      writetask = 1;   

   }
         
_atomic_end(_atomic); 

}


void cleanUpTraceFunction()
{disableTracing();}

int main()
{   
  __asm__ __volatile__("sei" ::); 
 
 
 writetask = 0; 
 currentindex = 0; 
 fileptr = mfopen("/logtrace", "w");
 enableTracing();  

 blockaddr = (tracingblock *)getEventBlockAddress();
 eventid = &(blockaddr->internaleventid); 
 threadid = &(blockaddr->threadid); 
 localtimestamp = &(blockaddr->timestamp);
 blockaddr->externaladdtracefp = tracingcontrol; 

 thiscurrentthread = getCurrentThread(); 
 thisthread = *thiscurrentthread; 

 
  setThreadTerminateHandler(cleanUpTraceFunction); 

 while (1)
 {
 	hibernateThread();
 	if (writetask == 1)
   { writeFileTask();
     writetask = 0; 
   }
 }
 
 
 return 0; 
}

