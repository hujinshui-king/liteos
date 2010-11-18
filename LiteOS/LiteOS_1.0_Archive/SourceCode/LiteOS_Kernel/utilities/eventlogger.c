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
#include "eventlogger.h"
#include "../filesys/fsapi.h"
#include "../io/avr_serial/serialprint.h"
typedef struct tracingblock
{
    uint8_t internaleventid;
    uint8_t threadid;
    void (*externaladdtracefp) ();
    void (*externalapptracefp) ();
} tracingblock;
uint8_t enabletracing;
tracingblock internaltracingblock;
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

//-------------------------------------------------------------------------
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

//-------------------------------------------------------------------------
#else
void addTrace(uint8_t traceid, uint8_t threadid)
{
    internaltracingblock.internaleventid = traceid;
    internaltracingblock.threadid = threadid;
    if ((internaltracingblock.externaladdtracefp != NULL) && (enabletracing
                                                              == 1))
    {
        internaltracingblock.externaladdtracefp();
    }
}

//-------------------------------------------------------------------------
#endif
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
