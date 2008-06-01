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
#include "globaltiming.h"
#if defined(PLATFORM_AVR) && defined(RADIO_CC2420)
#include "../platform/micaz/hplcc2420interruptm.h"
#include "../io/avr_serial/serialprint.h"
#endif
#ifdef PLATFORM_AVR
#include "../platform/avr/timerraw.h"
#include "../platform/avr/clockraw.h"
#include "../platform/micaz/leds.h"
#endif
static currentTimeUnit internaltime1, internaltime2;
static uint8_t timeselect;
currentTimeUnit *getCurrentTimeStamp()
{
#ifdef PLATFORM_AVR
    uint16_t counter1, counter2;
    uint32_t resolution;

    counter1 = HPLClock_readTimeCounterHigh();
    resolution = HPLClock_readTimeCounterLow();
    counter2 = HPLClock_readTimeCounterHigh();
    if (counter1 == counter2)
    {
    }
    else
    {
        counter1 = HPLClock_readTimeCounterHigh();
        resolution = HPLClock_readTimeCounterLow();
        counter2 = HPLClock_readTimeCounterHigh();
    }
    if (timeselect == 1)
    {
        internaltime1.counter = counter1;
        internaltime1.resolution = resolution;
        timeselect = 2;
        return &internaltime1;
    }
    if (timeselect == 2)
    {
        timeselect = 1;
        internaltime2.counter = counter1;
        internaltime2.resolution = resolution;
        return &internaltime2;
    }
#endif
    return NULL;
}

//-------------------------------------------------------------------------
uint16_t getCurrentCounterHigh()
{
#ifdef PLATFORM_AVR
    return HPLClock_readTimeCounterHigh();
#endif
}

//-------------------------------------------------------------------------
uint32_t getCurrentResolution()
{
#ifdef PLATFORM_AVR
    return HPLClock_readTimeCounterLow();
#endif
}

//-------------------------------------------------------------------------
void GenericTimingStart()
{
#ifdef PLATFORM_AVR
    timeselect = 1;
    HPLClock_Timer3_Start();
#endif
}

//-------------------------------------------------------------------------
void GenericTimingStop()
{
#ifdef PLATFORM_AVR
    HPLClock_Timer3_Stop();
#endif
}
