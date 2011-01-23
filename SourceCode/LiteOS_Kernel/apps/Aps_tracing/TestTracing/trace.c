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

uint8_t *eventid; 
uint8_t *threadid; 

MYFILE *fileptr; 
uint8_t trace[128]; 
uint8_t traceshadow[32]; 
uint8_t currentindex; 
uint8_t writetask; 

volatile uint16_t regsource; 

thread **thiscurrentthread;
thread *thisthread; 
 
          



#define APPTRACEPOINT1 0x6C7C

#define APPTRACEPOINT2 0x2875

#define APPTRACEPOINT1COMPARE 0x856C

#define APPTRACEPOINT2COMPARE 0x7E28


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
   	 trace[currentindex++] = 1;
   	 trace[currentindex++] = *eventid; 
     trace[currentindex++] = *threadid; 
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


void tracingcontroluser()  __attribute__(( naked ));
void tracingcontroluser() {
	
	
	  uint16_t SPvalue; 
	  uint16_t *sp;



    asm volatile (
    "in %A0, 0x3d" "\n\t"
    "in %B0, 0x3e" "\n\t"
    : "=r" (SPvalue)
    :
    );
    
    SPvalue = SPvalue+5; 
    sp = (uint16_t*)SPvalue; 
    regsource = (uint16_t)*(uint16_t*)sp;
    //reghigh = *(sp+1); 
    SPvalue = SPvalue+1; 
    
     asm volatile (
    "out 0x3d, %A0" "\n\t"
    "out 0x3e, %B0" "\n\t"
    :: "r" (SPvalue)
    
    );
    
    
    if (regsource == APPTRACEPOINT1COMPARE)
    	{
    	
	  __asm__ __volatile__("pop r31" ::); 
	  __asm__ __volatile__("pop r30" ::); 
	  __asm__ __volatile__("pop r29" ::); 	
	  __asm__ __volatile__("pop r28" ::); 
	  __asm__ __volatile__("pop r25" ::); 
	  __asm__ __volatile__("pop r24" ::); 
	  	
	  	
	  	/*
    __asm__ __volatile__("ldi	r30, 0x0C" ::); 
    __asm__ __volatile__("ldi	r31, 0xEA" ::);
    __asm__ __volatile__("icall" ::);
    __asm__ __volatile__("mov	r26, r20"::);
    __asm__ __volatile__("mov	r27, r21"::);
    __asm__ __volatile__("pop	r21"::);
    __asm__ __volatile__("pop	r20"::);
    __asm__ __volatile__("ld	r30, X+"::);
    __asm__ __volatile__("ld	r31, X"::); 
    __asm__ __volatile__("sbiw	r26, 0x01"::);
    __asm__ __volatile__("ldi	r24, 0x04"::); 
    __asm__ __volatile__("std	Z+2, r24"::);
    __asm__ __volatile__("ld	r30, X+"::); 
    */
    
     __asm__ __volatile__("push	r28" ::);
		 __asm__ __volatile__("push	r29" ::);                     
		 __asm__ __volatile__("in	r28, 0x3d" ::);       
		 __asm__ __volatile__("in	r29, 0x3e" ::);      
		 __asm__ __volatile__("sbiw	r28, 0x08" ::);       
		 __asm__ __volatile__("in	r0, 0x3f" ::);     
		 __asm__ __volatile__("cli" ::);                     
		 __asm__ __volatile__("out	0x3e, r29" ::);
		 __asm__ __volatile__("out	0x3f, r0" ::);     
		 __asm__ __volatile__("out	0x3d, r28" ::);     
		 __asm__ __volatile__("std	Y+8, r25" ::);   
		 __asm__ __volatile__("std	Y+7, r24" ::);
		 __asm__ __volatile__("lds	r24, 0x078a" ::);
	  
	  }             
		                      
		else if (regsource == APPTRACEPOINT2COMPARE)
			{
			
		__asm__ __volatile__("pop r31" ::); 
	  __asm__ __volatile__("pop r30" ::); 
	  __asm__ __volatile__("pop r29" ::); 	
	  __asm__ __volatile__("pop r28" ::); 
	  __asm__ __volatile__("pop r25" ::); 
	  __asm__ __volatile__("pop r24" ::); 
	  	
	  	
	  	
    __asm__ __volatile__("push	r28" ::);     
    __asm__ __volatile__("push	r29" ::);      
    __asm__ __volatile__("in	r28, 0x3d" ::);
    __asm__ __volatile__("in	r29, 0x3e" ::);
    __asm__ __volatile__("sbiw	r28, 0x07" ::);
    __asm__ __volatile__("in	r0, 0x3f" ::);
    __asm__ __volatile__("cli" ::);            
    __asm__ __volatile__("out	0x3e, r29" ::);
    __asm__ __volatile__("out	0x3f, r0" ::);
    __asm__ __volatile__("out	0x3d, r28" ::);
    __asm__ __volatile__("std	Y+5, r25" ::);
    __asm__ __volatile__("std	Y+4, r24" ::);
    __asm__ __volatile__("call	0x134c0" ::);
    
  
     }
    {
    
    
    PUSH_GPR();
    PUSH_REG_STATUS();
    //sourceaddr = 	(uint16_t)reglow * 16 + reghigh - 9;  
    _atomic_t _atomic = _atomic_start();
  
   if (regsource == APPTRACEPOINT1COMPARE)
    {
     trace[currentindex++] = 1;
   	 trace[currentindex++] = 11; 
     trace[currentindex++] = 0;     		
    }
       
       
  if (regsource == APPTRACEPOINT2COMPARE)
    {
     trace[currentindex++] = 2;
   	 trace[currentindex++] = 12; 
     trace[currentindex++] = 0;     		
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
    
    
       
       
    
    POP_REG_STATUS();
    POP_GPR();
    }
  
    __asm__ __volatile__("push r24" ::); 
	  __asm__ __volatile__("push r25" ::); 
	  __asm__ __volatile__("push r30" ::); 	
	  __asm__ __volatile__("push r31" ::); 
	  	
    asm volatile (
    "in %A0, 0x3d" "\n\t"
    "in %B0, 0x3e" "\n\t"
    : "=r" (SPvalue)
    :
    );
    
    SPvalue = SPvalue-1;
    sp = (uint16_t*)SPvalue; 
    *sp = regsource; 
    SPvalue --; 
     asm volatile (
    "out 0x3d, %A0" "\n\t"
    "out 0x3e, %B0" "\n\t"
    :: "r" (SPvalue)
    
    );
     
   __asm__ __volatile__("ret" ::); 
    
   
}


void traceMemory()
{
   uint16_t value = MEMORY_UINT16(0x5cc);

     _atomic_t _atomic = _atomic_start();
  
   {
        
   trace[currentindex++] = 3;
   trace[currentindex++] = 201; 
   trace[currentindex++] = 2;
   trace[currentindex++] = value%256;
   trace[currentindex++] = value/256; 	
    
       
       
 
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
   
  }
         
_atomic_end(_atomic); 
  
  
    value = MEMORY_UINT16(0x5ce);

     _atomic = _atomic_start();
  
   {
        
   trace[currentindex++] = 3;
   trace[currentindex++] = 202; 
   trace[currentindex++] = 2;
   trace[currentindex++] = value%256;
   trace[currentindex++] = value/256; 	
    
       
       
 
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
   
  }
         
_atomic_end(_atomic); 

  
 }

int main()
{ 

 __asm__ __volatile__("sei" ::); 
 	
 uint16_t pagenum;
 uint8_t pageoffset;
  
 
 writetask = 0; 
 currentindex = 0; 
 
 
 
 fileptr = mfopen("/logtrace", "w");
  
 thiscurrentthread = getCurrentThread(); 
 thisthread = *thiscurrentthread; 
 
 enableTracing();  
 
 

 blockaddr = (tracingblock *)getEventBlockAddress();
 eventid = &(blockaddr->internaleventid); 
 threadid = &(blockaddr->threadid); 
 blockaddr->externaladdtracefp = tracingcontrol; 
 blockaddr->externalapptracefp = tracingcontroluser; 
 
 

 setTimerFunction(500, 0, traceMemory);
      
 {
 uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT1*2); 
 pagenum = tracepointlocation/2/128;
 pageoffset = tracepointlocation%256; 
 addUserTracePoint_longinstruction( pagenum,  pageoffset );
}


{
 uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT2*2); 
 pagenum = tracepointlocation/2/128;
 pageoffset = tracepointlocation%256;  
 addUserTracePoint_longinstruction( pagenum,  pageoffset ); 
}


 while (1)
 {
 	sleepThread(100);
 	if (writetask == 1)
   { writeFileTask();
     writetask = 0; 
   }
 }
 return 0; 
}

