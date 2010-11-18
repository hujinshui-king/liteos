#ifndef THREADTOOLSH
#define THREADTOOLSH

#include "threaddata.h"

uint8_t memory_conflict_detect(uint16_t createflashromstart, uint16_t
  createflashromsize, uint16_t ramstackstart, uint16_t ramstackend);
  
int check_for_memory_corrupt(int i);

void ServiceTimerFired(uint8_t id);

void **getKernelStackAddress();

int getThreadIndexAddress();

void barrier_unblock(uint8_t type, uint8_t id); 

 
#endif
