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
#include "eventtrace.h"
#include "trace.h"

 	
tracingblock *blockaddr; 
volatile uint8_t stacklevel; 
volatile uint8_t update = 0; 
uint8_t *eventid; 
MYFILE *fileptr; 
uint8_t trace[128]; 
uint16_t currentindex; 
uint8_t *threadid; 
volatile uint16_t regsource; 

thread **thiscurrentthread;
thread *thisthread; 
 
uint8_t writetask; 

uint16_t value; 

            
uint8_t traceshadow[32]; 

uint16_t originalsp; 

//PATCHDEFSTART$$$$$
volatile uint8_t tracepatch0[16];
volatile uint8_t tracepatch1[16];
//PATCHDEFEND$$$$$


volatile uint16_t **oldstackptraddr;
volatile uint16_t *currentthreadptr;

 uint16_t pagenum;
 uint8_t pageoffset;

#define SWAP_STACK_PTR(OLD, NEW) \
  __asm__("in %A0, __SP_L__\n\t in %B0, __SP_H__":"=r"(OLD):);\
  __asm__("out __SP_H__,%B0\n\t out __SP_L__,%A0"::"r"(NEW))

 
//APPTRACESTART$$$$$
#define APPTRACEPOINT0  0xb39b

#define APPTRACEPOINT0COMPARE 0xa0b3





//APPTRACEEND$$$$$

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
 
   if (*threadid  != 3)
   {
   	 trace[currentindex++] = 1;
   	 trace[currentindex++] = *eventid; 
     trace[currentindex++] = *threadid; 
   }
   
 
 if (currentindex >= 118)

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

//void tracingcontroluser(){}
void tracingcontroluser()  __attribute__(( naked ));
void tracingcontroluser() {
	
	
	  uint16_t SPvalue; 
	  uint16_t canaryaddr; 
	  uint16_t *sp;



    asm volatile (
    "in %A0, 0x3d" "\n\t"
    "in %B0, 0x3e" "\n\t"
    : "=r" (SPvalue)
    :
    );
   
     
    SPvalue = SPvalue+5+4; 
    sp = (uint16_t*)SPvalue; 
    regsource = (uint16_t)*(uint16_t*)sp;
    //reghigh = *(sp+1); 
    SPvalue = SPvalue-7; 
    
     asm volatile (
    "out 0x3d, %A0" "\n\t"
    "out 0x3e, %B0" "\n\t"
    :: "r" (SPvalue)
    
    );
    
    //APPJUMPSTART$$$$$
if (regsource == APPTRACEPOINT0COMPARE)

{

__asm__ __volatile__("pop r29" ::);
__asm__ __volatile__("pop r28" ::);
__asm__ __volatile__("pop r25" ::);
__asm__ __volatile__("pop r24" ::);

__asm__ __volatile__("pop r31" ::);
__asm__ __volatile__("pop r30" ::);
__asm__ __volatile__("pop r31" ::);
__asm__ __volatile__("pop r30" ::);

__asm__ __volatile__("pop r31" ::);
__asm__ __volatile__("pop r30" ::);
	
	
    



__asm__ __volatile__(" ldi	r24, 0x68"::);
    
__asm__ __volatile__("sts	0x0B2A, r24"::);
    
__asm__ __volatile__("ldi	r24, 0x65"::);
    
__asm__ __volatile__("sts	0x0B2B, r24"::);

__asm__ __volatile__("ldi	r24, 0x6C"::);


}



    //APPJUMPEND$$$$$
    
    
    {                     
                          
                          
    __asm__ __volatile__("push r31" ::); 
	  __asm__ __volatile__("push r30" ::); 
	  __asm__ __volatile__("push r25" ::); 
	  __asm__ __volatile__("push r24" ::); 
	  	                    
   // SWAP_STACK_PTR(currentthreadptr, *oldstackptraddr); 
                          
    PUSH_GPR();           
    PUSH_REG_STATUS();    
                          
    {                      
    _atomic_t _atomic = _atomic_start();
   
    
     //USERTRACEACTIONSTART$$$$$


if ( regsource == APPTRACEPOINT0COMPARE )

{

trace[currentindex++] = 2;

trace[currentindex++] = 12;

trace[currentindex++] = MEMORY_UINT8(0xb38);



}


    //USERTRACEACTIONEND$$$$$
    

 
 if (currentindex >= 118)

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
            
    
    POP_REG_STATUS();
    POP_GPR();
    
   // SWAP_STACK_PTR(*oldstackptraddr, currentthreadptr); 
    
    __asm__ __volatile__("pop r24" ::); 
	  __asm__ __volatile__("pop r25" ::); 
	  __asm__ __volatile__("pop r30" ::); 	
	  __asm__ __volatile__("pop r31" ::); 
    }
  
    
	  __asm__ __volatile__("push r30" ::); 	
	  __asm__ __volatile__("push r31" ::); 
	  	
    
   asm volatile(" mov r31, %A0" "\n\t"
	              " mov r30, %B0" "\n\t"
				 :
				 :"r" (regsource)
                );
    
  
   __asm__ __volatile__("ijmp" ::); 
    
   
}


 
//////////////////////////////////////////////////////////////////////////////////////
/// Clean up task for the thread
//////////////////////////////////////////////////////////////////////////////////////


void cleanUpTraceFunction()
{
	
	disableTracing();
}

//The trace arrays should be application specific here
void cleanUpTraceFunction2() {

//CLEANTRACESTART$$$$$                     


{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT0*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

//removeUserTracePoint( pagenum,  pageoffset , tracepatch0);

}




//CLEANTRACEEND$$$$$
	
}


int main()
{ 


  
  __asm__ __volatile__("sei" ::); 
 
 
 writetask = 0; 
 stacklevel = 0; 
 currentindex = 0; 
 
 fileptr = mfopen("/logtrace", "w");
 
 enableTracing();  

 blockaddr = (tracingblock *)getEventBlockAddress();
 eventid = &(blockaddr->internaleventid); 
 threadid = &(blockaddr->threadid); 
 
 blockaddr->externaladdtracefp = tracingcontrol; 
 blockaddr->externalapptracefp = tracingcontroluser; 
 
 thiscurrentthread = getCurrentThread(); 
 thisthread = *thiscurrentthread; 
 
  
  oldstackptraddr = (uint16_t**)getKernelStackPtr(); 
 
 setThreadTerminateHandler(cleanUpTraceFunction); 
 
 //setTimerFunction(50, 0, testUpdate);
 
//ADDTRACESTART$$$$$


{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT0*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}


//ADDTRACEEND$$$$$

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

