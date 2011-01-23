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
