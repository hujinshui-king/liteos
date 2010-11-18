#include "eventlogger.h"
#include "../storage/filesys/fsapi.h"
#include "../timer/globaltiming.h"
#include "../io/avrserial/serialprint.h"


typedef struct tracingblock
{
    uint16_t internaleventid;
    uint8_t threadid;
    uint32_t timestamp; 
    void (*externaladdtracefp) ();
    void (*externalapptracefp) ();
} tracingblock;

uint8_t enabletracing;

tracingblock internaltracingblock;

uint32_t timestamp; 


void enabletracingfunction()
{
    enabletracing = 1;
}

//-------------------------------------------------------------------------
void disabletracingfunction()
{
    enabletracing = 0;
}

//-------------------------------------------------------------------------
void initTrace()
{
    internaltracingblock.externaladdtracefp = NULL;
    internaltracingblock.externalapptracefp = NULL;
    enabletracing = 0;
}


void addTrace(uint16_t traceid, uint8_t threadid)
{
	



    
	  if (threadid==1)
	   return; 
	  timestamp = getCurrentResolution();
    internaltracingblock.internaleventid = traceid;
    internaltracingblock.threadid = threadid;
    internaltracingblock.timestamp = timestamp; 
    if ((internaltracingblock.externaladdtracefp != NULL) && (enabletracing
                                                              == 1))
    {
    	
    	  	  
	  //lib_printf_uinteger32(traceid);
	  //lib_printf_uinteger32(threadid);
    internaltracingblock.externaladdtracefp();
    }
}

void apptracepointfunction() __attribute__ ((naked));
void apptracepointfunction()
{
    asm volatile ("push r24"::);
    asm volatile ("push r25"::);
    asm volatile ("push r28"::);
    asm volatile ("push r29"::);

    if ((internaltracingblock.externalapptracefp != NULL) && (enabletracing
                                                              == 1))
    {
        internaltracingblock.externalapptracefp();
    }
}

//-------------------------------------------------------------------------
void *getTracingBlockAddress()
{
    return (void *)&internaltracingblock;
}
