#ifndef GLOBALTIMINGH
#define GLOBALTIMINGH


#include "../types/types.h"


//The data structure used to store the 48 bit cycle accurate timer 

typedef struct 
{
	uint16_t counter; 
	uint32_t resolution;
}
currentTimeUnit;



void GenericTimingStart();

void GenericTimingStop();

//Read the current time stamp including both the high level counter and the low level counter
//A total of 48 bits are read out. The 48-bit value will increase by 1 for every CPU cycle. 

currentTimeUnit *getCurrentTimeStamp();

//Read the low counter which is 32 bits

uint32_t getCurrentResolution(); 


//Read the high level counter which is 16 bits. 

uint16_t getCurrentCounterHigh(); 


#endif


