#ifndef THREADKERNELH
#define THREADKERNELH
#include "threaddata.h"
 
//Our thread table
extern thread thread_table[LITE_MAX_THREADS];


//point to the current thread
extern volatile thread *current_thread;


void thread_init();

uint8_t is_thread();


/**\ingroup  thread */
///create thread
int create_thread(void (*fcn) (), uint16_t * ram_start, uint16_t * stack_ptr,
                  uint16_t staticdatasize, uint8_t priority, char *threadName,
                  uint16_t romstart, uint16_t romsize);

void setThreadTerminateFunction(uint8_t currentindex, void(*fp)());

void destroy_user_thread();

void __attribute__((noinline))lite_switch_to_user_thread();

void __attribute__((noinline))thread_yield();

void sleepThread(uint16_t milli);

void threads_handle_service();

void thread_func_dispatcher()__attribute__((naked));

inline int thread_get_next();

void thread_task();

void thread_wakeup(uint8_t id);

void postNewThreadTask();

void break_point_function();

#endif
