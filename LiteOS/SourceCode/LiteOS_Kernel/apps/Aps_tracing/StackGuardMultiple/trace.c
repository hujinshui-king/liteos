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

uint16_t currentindex; 
uint8_t *threadid; 
volatile uint16_t regsource; 

thread **thiscurrentthread;
thread *thisthread; 
 
uint8_t writetask; 

uint16_t value; 

uint16_t popcanary;
uint8_t temploop; 
          

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
#define APPTRACEPOINT0  0xb465

#define APPTRACEPOINT0COMPARE 0x6ab4



#define APPTRACEPOINT1  0xb470

#define APPTRACEPOINT1COMPARE 0x75b4






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
	
__asm__ __volatile__("push r30" ::);
__asm__ __volatile__("push r31" ::);	
__asm__ __volatile__("push r30" ::);
__asm__ __volatile__("push r31" ::);
__asm__ __volatile__("push r25" ::);
__asm__ __volatile__("push r24" ::);
__asm__ __volatile__("push r29" ::);
__asm__ __volatile__("push r28" ::);

 asm volatile (
    "in %A0, 0x3d" "\n\t"
    "in %B0, 0x3e" "\n\t"
    : "=r" (canaryaddr)
    :
    );
   
   
     
  canaryaddr = canaryaddr + 7; 
  (*(uint16_t*)canaryaddr) = 0x1234;
  
__asm__ __volatile__("pop r28" ::);
__asm__ __volatile__("pop r29" ::);
__asm__ __volatile__("pop r24" ::);
__asm__ __volatile__("pop r25" ::); 
__asm__ __volatile__("pop r31" ::);
__asm__ __volatile__("pop r30" ::);
    
__asm__ __volatile__("push r28"::);

__asm__ __volatile__("push r29"::);

__asm__ __volatile__("in r28, 0x3d"::);

__asm__ __volatile__("in r29, 0x3e"::);

__asm__ __volatile__("sbiw r28, 0x0a"::);

__asm__ __volatile__("in r0, 0x3f"::);

__asm__ __volatile__("cli"::);

}

else if (regsource == APPTRACEPOINT1COMPARE)

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

__asm__ __volatile__("in r0, 0x3f"::);

__asm__ __volatile__("cli"::);

__asm__ __volatile__("out 0x3e, r29"::);

__asm__ __volatile__("out 0x3f, r0"::);

__asm__ __volatile__("out 0x3d, r28"::);

__asm__ __volatile__("pop r29"::);

__asm__ __volatile__("pop r28"::);


__asm__ __volatile__("push r31" ::);
__asm__ __volatile__("push r30" ::);	
__asm__ __volatile__("push r29" ::);
__asm__ __volatile__("push r28" ::);
__asm__ __volatile__("push r25" ::);
__asm__ __volatile__("push r24" ::);
__asm__ __volatile__("push r29" ::);
__asm__ __volatile__("push r28" ::);

 asm volatile (
    "in %A0, 0x3d" "\n\t"
    "in %B0, 0x3e" "\n\t"
    : "=r" (canaryaddr)
    :
    );
   
   
     
  canaryaddr = canaryaddr + 9; 
  popcanary = *(uint16_t*)canaryaddr;
  if (popcanary != 0x1234)
  	 {
        	
__asm__ __volatile__("push r29" ::);
__asm__ __volatile__("push r28" ::);
__asm__ __volatile__("push r25" ::);
__asm__ __volatile__("push r24" ::);
__asm__ __volatile__("push r29" ::);
__asm__ __volatile__("push r28" ::); 	
  	 }
  
  canaryaddr = canaryaddr+1;

  *(uint8_t*)canaryaddr =*(uint8_t*)(canaryaddr-2) ;
  canaryaddr = canaryaddr -1; 
  *(uint8_t*)canaryaddr =*(uint8_t*)(canaryaddr-2) ;
  canaryaddr = canaryaddr -1; 
  *(uint8_t*)canaryaddr =*(uint8_t*)(canaryaddr-2) ;
    canaryaddr = canaryaddr -1; 
  *(uint8_t*)canaryaddr =*(uint8_t*)(canaryaddr-2) ;
    canaryaddr = canaryaddr -1; 
  *(uint8_t*)canaryaddr =*(uint8_t*)(canaryaddr-2) ;
    canaryaddr = canaryaddr -1; 
  *(uint8_t*)canaryaddr =*(uint8_t*)(canaryaddr-2) ;
    canaryaddr = canaryaddr -1; 
  *(uint8_t*)canaryaddr =*(uint8_t*)(canaryaddr-2) ;  
    canaryaddr = canaryaddr -1; 
  *(uint8_t*)canaryaddr =*(uint8_t*)(canaryaddr-2) ;
    	 
  __asm__ __volatile__("pop r28" ::);
__asm__ __volatile__("pop r29" ::);
__asm__ __volatile__("pop r24" ::);
__asm__ __volatile__("pop r25" ::);
__asm__ __volatile__("pop r28" ::);
__asm__ __volatile__("pop r29" ::);
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


//The trace arrays should be application specific here
void cleanUpTraceFunction() {

//CLEANTRACESTART$$$$$                     


{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT0*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

removeUserTracePoint( pagenum,  pageoffset , tracepatch0);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT1*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

removeUserTracePoint( pagenum,  pageoffset , tracepatch1);

}


//CLEANTRACEEND$$$$$
	
}




int main()
{ 


  
  __asm__ __volatile__("sei" ::); 
 
 
 writetask = 0; 
 stacklevel = 0; 
 currentindex = 0; 
 //fileptr = mfopen("/logtrace", "w");
 enableTracing();  

 blockaddr = (tracingblock *)getEventBlockAddress();
 eventid = &(blockaddr->internaleventid); 
 threadid = &(blockaddr->threadid); 
 //blockaddr->externaladdtracefp = tracingcontrol; 
 blockaddr->externalapptracefp = tracingcontroluser; 
 
 thiscurrentthread = getCurrentThread(); 
 thisthread = *thiscurrentthread; 
 
 //setTimerFunction(500, 0, traceMemory);
 
 
 oldstackptraddr = (uint16_t**)getKernelStackPtr(); 
 
 //setThreadTerminateHandler(cleanUpTraceFunction); 
 
 // setTimerFunction(50, 0, testUpdate);
 
//CLEANDEFSTART$$$$$
tracepatch0[0] = 0xcf;

tracepatch0[1] = 0x93;

tracepatch0[2] = 0xdf;

tracepatch0[3] = 0x93;

tracepatch0[4] = 0xcd;

tracepatch0[5] = 0xb7;

tracepatch0[6] = 0xde;

tracepatch0[7] = 0xb7;

tracepatch0[8] = 0x2a;

tracepatch0[9] = 0x97;

tracepatch0[10] = 0x0f;

tracepatch0[11] = 0xb6;

tracepatch0[12] = 0xf8;

tracepatch0[13] = 0x94;

tracepatch0[ 14 ] = 0xff;

tracepatch0[ 15 ] = 0xff;

tracepatch1[0] = 0x0f;

tracepatch1[1] = 0xb6;

tracepatch1[2] = 0xf8;

tracepatch1[3] = 0x94;

tracepatch1[4] = 0xde;

tracepatch1[5] = 0xbf;

tracepatch1[6] = 0x0f;

tracepatch1[7] = 0xbe;

tracepatch1[8] = 0xcd;

tracepatch1[9] = 0xbf;

tracepatch1[10] = 0xdf;

tracepatch1[11] = 0x91;

tracepatch1[12] = 0xcf;

tracepatch1[13] = 0x91;

tracepatch1[ 14 ] = 0xff;

tracepatch1[ 15 ] = 0xff;

//CLEANDEFEND$$$$$
 
//ADDTRACESTART$$$$$


{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT0*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}




{
uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT1*2);

pagenum = tracepointlocation/2/128;

pageoffset = tracepointlocation%256;

addUserTracePoint( pagenum,  pageoffset);

}


//ADDTRACEEND$$$$$

 while (1)
 {
 	hibernateThread();
 	
 }
 return 0; 
}

