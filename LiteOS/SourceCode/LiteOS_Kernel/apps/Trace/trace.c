#include "../../libraries/libleds.h"
#include "../../libraries/libthread.h"
#include "../../libraries/libsystem.h"
#include "../../libraries/libserial.h"
#include "../../libraries/liteoscommon.h"
#include "../../libraries/libradio.h"
#include "../../libraries/libstring.h"
#include "../../libraries/libfile.h"
#include "../../libraries/libeeprom.h"
#include "../../libraries/libadc.h"
#include "../../libraries/libtrace.h"
#include "../../types/types.h"
 	
libtracingblock *blockaddr; 
uint16_t *eventid; 
LIB_MYFILE *fileptr; 
uint32_t *localtimestamp; 
uint8_t trace[128]; 
uint8_t currentindex; 
uint8_t *threadid; 

uint8_t tracebuffer[300]; 


lib_thread **thiscurrentthread;
lib_thread *libthisthread; 
 
uint8_t writetask; 
            
uint8_t traceshadow[32]; 





void tracewriteFileTask(){
	_atomic_t _atomic = _atomic_start();
		 lib_mfwrite(fileptr, traceshadow, 32); 
		 lib_mfseek(fileptr, 32, 1);
		 lib_mfwrite(fileptr, &trace[32], 96); 
		 lib_mfseek(fileptr, 96, 1);  
	_atomic_end(_atomic); 	 
}


//Shall use eventid to show the actual type of event here. 

void tracingcontrol(){
	 	
  _atomic_t _atomic = _atomic_start();
   
   //lib_printf_string_with_id("tracing control function\n");
   
   if (*threadid > 1)
   {
   	uint32_t *temp;
   	 trace[currentindex++] = 1;
  	 trace[currentindex++] = *eventid/256;
  	 trace[currentindex++] = (*eventid)%256; 
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

     
      libthisthread->state = STATE_ACTIVE; 
      syscall_post_thread_task_no_logging();  
      writetask = 1;   

   }
         
_atomic_end(_atomic); 

}


void cleanUpTraceFunction()
{lib_disable_tracing();}

void tracemain()
{   
 
 
 
 writetask = 0; 
 currentindex = 0; 
 thiscurrentthread = lib_get_current_thread(); 
 libthisthread = *thiscurrentthread; 
 
 fileptr = lib_mfopen("/logtrace", "w");
 lib_enable_tracing();  

 blockaddr = (libtracingblock *)lib_get_event_trace_block_address();
 eventid = &(blockaddr->internaleventid); 
 threadid = &(blockaddr->threadid); 
 localtimestamp = &(blockaddr->timestamp);
 blockaddr->externaladdtracefp = tracingcontrol; 

lib_print_usart_string("terminate handler set\n");

 
  lib_set_thread_termination_handler(cleanUpTraceFunction); 

	lib_print_usart_string("enter loop\n");
 while (1)
 {
 	lib_print_usart_string("hibernate now\n");
 	lib_hibernate_thread();
 	
 	lib_print_usart_string("tracing wakeup\n");
 	if (writetask == 1)
   { tracewriteFileTask();
     writetask = 0; 
   }
 }
 
 
 return;
}

