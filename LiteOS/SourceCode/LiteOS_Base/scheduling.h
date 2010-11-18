#ifndef SCHEDULING
#define SCHEDULING

#include "Types.h"


void initScheduling(void);

bool postTask(void (*tp) (void), uint8_t priority);

bool runNextTask(void);



#endif 

