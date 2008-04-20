#include "eventlogger.h"
#include "../filesys/fsapi.h"
#include "../io/avr_serial/serialprint.h"






typedef struct tracingblock {
	
 uint8_t internaleventid; 
 uint8_t threadid; 
 void (*externaladdtracefp)(); 
 void (*externalapptracefp)();
 
 }  tracingblock;
 
 
uint8_t enabletracing; 

tracingblock internaltracingblock;  


void enabletracingfunction() {
   enabletracing = 1; 
}

void disabletracingfunction(){
	 enabletracing = 0; 
}

void initTrace(){
	 internaltracingblock.externaladdtracefp = NULL; 
	 internaltracingblock.externalapptracefp = NULL; 
	 enabletracing = 0; 
}

#ifdef SERIAL_TRACE

void addTrace(uint8_t traceid, uint8_t threadid)
{
	usartPrint('K');
	usartPrint(' ');
	usartPrint(48 + (traceid / 100));
	usartPrint(48 + ((traceid / 10) % 10));
	usartPrint(48 + (traceid % 10));
	usartPrint('\n');
}

#else

void addTrace(uint8_t traceid, uint8_t threadid)
{
   internaltracingblock.internaleventid = traceid; 
   internaltracingblock.threadid = threadid; 
   
   if ((internaltracingblock.externaladdtracefp !=NULL)&&(enabletracing == 1))
   	internaltracingblock.externaladdtracefp(); 	
}

#endif 



void apptracepointfunction() __attribute__(( naked ));
void apptracepointfunction()
{
	
	 asm volatile( "push r24":: );
	 asm volatile( "push r25":: );
	 asm volatile( "push r28":: );
	 asm volatile( "push r29":: );
  if ((internaltracingblock.externalapptracefp !=NULL)&&(enabletracing == 1))
   	internaltracingblock.externalapptracefp(); 	
}





void *getTracingBlockAddress()
{
   return (void *)&internaltracingblock; 	
}


