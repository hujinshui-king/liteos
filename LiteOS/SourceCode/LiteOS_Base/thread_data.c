#include "thread_data.h"





void call_fcn_ptr(void (*tp)()){
  (*tp)();
}

/* is_thread
 * This returns whether the currently executing process is either a thread (1) or a task(0)
 */




/* stack_top
 * Since tinyos doesn't always allow defines from its includes...
 * This routine calculates the top of the stack
 */
//uint16_t * stack_top(uint16_t * stack, uint16_t size){
  //return(&(stack[(size / sizeof(uint16_t)) - 1]));



