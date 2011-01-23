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
/*                                                                      tab:4
 * "Copyright (c) 2000-2003 The Regents of the University  of California.  
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice, the following
 * two paragraphs and the author appear in all copies of this software.
 * 
 * IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF
 * CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
 *
 * Copyright (c) 2002-2003 Intel Corporation
 * All rights reserved.
 *
 * This file is distributed under the terms in the attached INTEL-LICENSE     
 * file. If you do not find these files, copies can be found by writing to
 * Intel Research Berkeley, 2150 Shattuck Avenue, Suite 1300, Berkeley, CA, 
 * 94704.  Attention:  Intel License Inquiry.
 */
/*
 *
 * Authors:             Jason Hill, David Gay, Philip Levis
 * Date last modified:  6/2/03
 *
 */

/**
* @author Jason Hill
* @author David Gay
* @author Philip Levis
*/
#include "../avr/avrhardware.h"
#include "micazhardware.h"
#include "leds.h"
#include "../../utilities/eventlogger.h"
#include "adcdriver.h"
uint8_t *adc_parameter;
uint8_t Leds_ledsOn;
enum
{
    Leds_RED_BIT = 1, Leds_GREEN_BIT = 2, Leds_YELLOW_BIT = 4
};
void Leds_Leds()
{
    {
        _atomic_t _atomic = _atomic_start();

        Leds_ledsOn = 0;
        LITE_MAKE_RED_LED_OUTPUT();
        LITE_MAKE_YELLOW_LED_OUTPUT();
        LITE_MAKE_GREEN_LED_OUTPUT();
        LITE_SET_RED_LED_PIN();
        LITE_SET_YELLOW_LED_PIN();
        LITE_SET_GREEN_LED_PIN();
        _atomic_end(_atomic);
    }
}

//-------------------------------------------------------------------------
void Leds_redOn()
{
    {
        _atomic_t _atomic = _atomic_start();

        LITE_CLR_RED_LED_PIN();
        Leds_ledsOn |= Leds_RED_BIT;
        _atomic_end(_atomic);
    }
    return;
}

//-------------------------------------------------------------------------
void Leds_redOff()
{
    {
        _atomic_t _atomic = _atomic_start();

        LITE_SET_RED_LED_PIN();
        Leds_ledsOn &= ~Leds_RED_BIT;
        _atomic_end(_atomic);
    }
    return;
}

//-------------------------------------------------------------------------
void Leds_redToggle()
{
    {
        _atomic_t _atomic = _atomic_start();

        if (Leds_ledsOn & Leds_RED_BIT)
        {
            Leds_redOff();
        }
        else
        {
            Leds_redOn();
        }
        _atomic_end(_atomic);
    }
#ifdef TRACE_ENABLE
#ifdef TRACE_ENABLE_LEDEVENT
    addTrace(TRACE_LEDEVENT_REDTOGGLE);
#endif
#endif
    return;
}

//-------------------------------------------------------------------------
void Leds_greenOn()
{
    {
        _atomic_t _atomic = _atomic_start();

        LITE_CLR_GREEN_LED_PIN();
        Leds_ledsOn |= Leds_GREEN_BIT;
        _atomic_end(_atomic);
    }
    return;
}

//-------------------------------------------------------------------------
void Leds_greenOff()
{
    {
        _atomic_t _atomic = _atomic_start();

        LITE_SET_GREEN_LED_PIN();
        Leds_ledsOn &= ~Leds_GREEN_BIT;
        _atomic_end(_atomic);
    }
    return;
}

//-------------------------------------------------------------------------
void Leds_greenToggle()
{
    {
        _atomic_t _atomic = _atomic_start();

        if (Leds_ledsOn & Leds_GREEN_BIT)
        {
            Leds_greenOff();
        }
        else
        {
            Leds_greenOn();
        }
        _atomic_end(_atomic);
    }
    return;
}

//-------------------------------------------------------------------------
void Leds_yellowOn()
{
    {
        _atomic_t _atomic = _atomic_start();

        LITE_CLR_YELLOW_LED_PIN();
        Leds_ledsOn |= Leds_YELLOW_BIT;
        _atomic_end(_atomic);
    }
    return;
}

//-------------------------------------------------------------------------
void Leds_yellowOff()
{
    {
        _atomic_t _atomic = _atomic_start();

        LITE_SET_YELLOW_LED_PIN();
        Leds_ledsOn &= ~Leds_YELLOW_BIT;
        _atomic_end(_atomic);
    }
    return;
}

//-------------------------------------------------------------------------
void Leds_yellowToggle()
{
    {
        _atomic_t _atomic = _atomic_start();

        if (Leds_ledsOn & Leds_YELLOW_BIT)
        {
            Leds_yellowOff();
        }
        else
        {
            Leds_yellowOn();
        }
        _atomic_end(_atomic);
    }
    return;
}

//-------------------------------------------------------------------------
void set_led_task()
{
    adc_parameter = getAdcParameterAddr();
    if (adc_parameter[0] == 1)
    {
        Leds_redOn();
    }

    if (adc_parameter[0] == 0)
    {
        Leds_redOff();
    }

    if (adc_parameter[0] == 2)
    {
        Leds_redToggle();
    }
    if (adc_parameter[1] == 1)
    {
        Leds_greenOn();
    }

    if (adc_parameter[1] == 0)
    {
        Leds_greenOff();
    }

    if (adc_parameter[1] == 2)
    {
        Leds_greenToggle();
    }
    if (adc_parameter[2] == 1)
    {
        Leds_yellowOn();
    }

    if (adc_parameter[2] == 0)
    {
        Leds_yellowOff();
    }

    if (adc_parameter[2] == 2)
    {
        Leds_yellowToggle();
    }
}
