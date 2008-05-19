#ifndef GLOBALTIMINGH
#define GLOBALTIMINGH


#include "../types/types.h"

typedef struct 
{
	uint16_t counter; 
	uint32_t resolution;
}
currentTimeUnit;



void GenericTimingStart();

void GenericTimingStop();


currentTimeUnit *getCurrentTimeStamp();

uint32_t getCurrentResolution(); 

uint16_t getCurrentCounterHigh(); 


#define SHOWME(X)  {    _atomic_t _atomic;\
                        uint32_t counter;\
	                   	  uint16_t hcounter;\
	                   	  _atomic = _atomic_start();\
	                   	  counter = getCurrentResolution();\
	                   	  hcounter = getCurrentCounterHigh();\
	                   	  printfstr(#X);\
	                   	  printfintegeru32(counter);\
	                   	  printfintegeru32(hcounter); \
	                   	  printfstr("\n");  \
	                   	  _atomic_end(_atomic); \
	                   	}
#endif


