#ifndef THREADSH
#define THREADSH


#include "thread_data.h"
#include "Types.h"
#include "threads.h"


void thread_init();

void __attribute__((noinline)) service_threads() ;

void __attribute__((noinline)) platform_switch_to_thread() ;


void yield() __attribute__((noinline));

void remove_thread();

 __attribute__((naked)) void getThreadAddress(); 

uint8_t is_thread();


void __attribute__((naked))  thread_wrapper();

void thread_task();

int create_thread(void (*fcn)(), uint16_t * stack_ptr, uint8_t priority);

void thread_sleep();


void thread_wakeup(uint8_t id);

 __attribute__((naked))  void mutexUnlockFunction();


void Mutex_unlock(mutex * m);


void Barrier_unblock(uint8_t type,uint8_t id);


void sleepThread(uint16_t milli);

void ServiceTimerFired(uint8_t id);

uint8_t is_thread();

__attribute__((naked)) void getRadioMutexAddress();

__attribute__((naked)) void getThreadIndexAddress();

#endif
