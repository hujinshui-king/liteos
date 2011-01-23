/* The LiteOS Operating System Kernel */
/*
   The following is the license of LiteOS.
   This file is part of LiteOS.
   Copyright Qing Cao, 2007-2008, University of Illinois , qcao2@uiuc.edu
   LiteOS is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   LiteOS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with LiteOS.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef GLOBALTIMINGH
#define GLOBALTIMINGH
#include "../types/types.h"

//The data structure used to store the 48 bit cycle accurate timer 
typedef struct
{
    uint16_t counter;
    uint32_t resolution;
} currentTimeUnit;

void GenericTimingStart();
void GenericTimingStop();

//Read the current time stamp including both the high level counter and the low level counter
//A total of 48 bits are read out. The 48-bit value will increase by 1 for every CPU cycle. 

currentTimeUnit *getCurrentTimeStamp();

//Read the low counter which is 32 bits
uint32_t getCurrentResolution();

//Read the high level counter which is 16 bits. 
uint16_t getCurrentCounterHigh();

#define SHOWME(X)  {    _atomic_t _atomic;\
uint32_t counter;\
uint16_t hcounter;\
_atomic = _atomic_start();\
counter = getCurrentResolution();\
hcounter = getCurrentCounterHigh();\
printfstr(#X);\
printfintegeru32(hcounter); \
printfintegeru32(counter);\
printfstr("\n");  \
_atomic_end(_atomic); \
}
#endif
