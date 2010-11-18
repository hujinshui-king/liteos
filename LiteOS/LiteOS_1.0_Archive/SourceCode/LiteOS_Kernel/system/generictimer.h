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
#ifndef GENERICTIMERH
#define GENERICTIMERH

/**\defgroup generictimer Generic timer service abstraction*/
#include "../types/types.h"

/** \ingroup generictimer
The timer fire types 
*/
enum
{
    TIMER_REPEAT = 0, TIMER_ONE_SHOT = 1,
};

/** \ingroup generictimer
Init the timer 
*/
inline result_t GenericTimerInit(void);

/** \ingroup generictimer
Start the timer. 
*/
inline result_t GenericTimerStart(uint8_t id, char type, uint32_t interval);

/** \ingroup generictimer
Timer fire event. 
*/
inline result_t GenericTimerFired(uint8_t id);
void setTimerCallBackFunction(uint8_t currentthreadindex, uint16_t period,
                              uint16_t type, void (*fp) ());
inline result_t GenericTimerStop(uint8_t id);
#endif
