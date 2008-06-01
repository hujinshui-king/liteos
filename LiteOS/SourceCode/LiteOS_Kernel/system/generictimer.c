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
#include "threads.h"
#include "generictimer.h"
#include "globaltiming.h"
#include "stdserial.h"
#include "scheduling.h"
#if defined(PLATFORM_AVR) && defined(RADIO_CC2420)
#include "../platform/micaz/hplcc2420interruptm.h"
#include "../io/avr_serial/serialprint.h"
#endif
#ifdef PLATFORM_AVR
#include "../platform/avr/timerraw.h"
#include "../platform/avr/clockraw.h"
#include "../platform/micaz/leds.h"
#endif

//Implementing platform related modules 
//This part assumes that LITE_MAX_THREAD is 8
void (*timercallback[8]) ();
inline result_t GenericTimerInit(void)
{
    uint8_t i;

    for (i = 0; i < 8; i++)
    {
        timercallback[i] = NULL;
    }
#ifdef PLATFORM_AVR
    return TimerM_StdControl_init();
#endif
}

//-------------------------------------------------------------------------
inline result_t GenericTimerStart(uint8_t id, char type, uint32_t interval)
{
#ifdef PLATFORM_AVR
    return TimerM_Timer_start(id, type, interval);
#endif
}

//-------------------------------------------------------------------------
inline result_t GenericTimerStop(uint8_t id)
{
#ifdef PLATFORM_AVR
    return TimerM_Timer_stop(id);
#endif
}

//-------------------------------------------------------------------------
void setTimerCallBackFunction(uint8_t currentthreadindex, uint16_t period,
                              uint16_t type, void (*fp) ())
{
    timercallback[currentthreadindex] = fp;
    GenericTimerStart(currentthreadindex + 18, type, period);
}

//-------------------------------------------------------------------------
void timercallbackinvoke(uint8_t id)
{
    uint8_t index = id - 18;

    if (timercallback[index] != NULL)
    {
        (*timercallback[index]) ();
    }
}

//This function is called from the particular implementation!
//This function also contains platform related defintions 
//0, 1, 2, AND 10, 11 are reserved. User is suggested to start with id 20. 
inline result_t GenericTimerFired(uint8_t id)
{
    unsigned char result;

    //0, 1, 2 for threads
    switch (id)
    {
    case 0:
        ServiceTimerFired(0);
        break;
    case 1:
        ServiceTimerFired(1);
        break;
    case 2:
        ServiceTimerFired(2);
        break;
    case 3:
        ServiceTimerFired(3);
        break;
    case 4:
        ServiceTimerFired(4);
        break;
    case 5:
        ServiceTimerFired(5);
        break;
    case 6:
        ServiceTimerFired(6);
        break;
    case 7:
        ServiceTimerFired(7);
        break;
    case 9:
#ifdef PLATFORM_CPU_MEASURE
        {
            _atomic_t currentatomic;

            currentatomic = _atomic_start();
            usartPutLong(cpucounter);
            _atomic_end(currentatomic);
        }
#endif
        break;
#if defined(PLATFORM_AVR) && defined(RADIO_CC2420)
    case 10:
        hplcc2420interruptm_FIFOTimer_fired();
        break;
    case 11:
        hplcc2420interruptm_CCATimer_fired();
        break;
#endif
    case 12:
        //Leds_redToggle();
        break;
    case 15:
        //Leds_redToggle(); 
        break;
    default:
        timercallbackinvoke(id);
        result = SUCCESS;
    }
    return SUCCESS;
}
