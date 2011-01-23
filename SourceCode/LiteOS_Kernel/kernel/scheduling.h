#ifndef SCHEDULING
#define SCHEDULING
#include "../types/types.h"

/**\defgroup scheduling Thread sheduling and task scheduling */

/** @{ */

/**Init the scheduler */
void initScheduling(void);

/**post a new task with a priority */
bool postTask(void (*tp) (void), uint8_t priority);

/**run the next task in the queue */
bool runNextTask(void);

/** @} */
#endif
