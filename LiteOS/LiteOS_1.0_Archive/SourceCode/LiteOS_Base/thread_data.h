#ifndef THREADDATAH
#define THREADDATAH


#ifndef TINYTHREAD_THREADCOUNT
#define TINYTHREAD_THREADCOUNT 8
#endif

#include "Types.h"


//Our mutexes

typedef struct mutex{
  volatile uint8_t lock;
  volatile uint8_t waiting;
}mutex;


typedef struct thread{
  volatile uint16_t * sp;
  volatile uint8_t state;

  uint8_t priority;
  volatile uint8_t remaincredits; 
  
  volatile union{
    void (*tp) ();
   
    mutex *m;
    
    struct {	 
	  uint16_t sleeptime;
	  uint8_t sleeptype;
	  }sleepstate;

    struct {
	  uint8_t id; 
	  uint16_t address;
	  uint8_t length;
	  uint8_t *msg;
	 }radiostate; 
	     
    struct {
      uint8_t type, id;
	  uint8_t *msg; 
    }iostate;

    struct {
	 int adcreading;
	}adcstate; 

   }data;

  volatile union{
   	struct {
	  uint8_t *fileptr;
	  uint8_t *bufferptr; 
	  uint16_t bytes;
	}filestate;
	 
	struct {
	  uint8_t *fileptr;
	  int offset; 
	  int position; 
	}fileseekstate;

	}filedata; 

}thread;



//This must be a power of 2

#define TOSH_MAX_THREADS      TINYTHREAD_THREADCOUNT
#define TOSH_MAX_THREADS_MASK (TOSH_MAX_THREADS - 1)

enum {
  STATE_NULL = 0,     //There is no thread here
  STATE_READY = 1,    //This thread has been created but never executed before
  STATE_ACTIVE = 2,   //There is a thread here, not blocked or sleeping
  STATE_BLOCKED = 3,  //This thread is blocked on a mutex
  STATE_PRESLEEP = 4,    //This thread is about to sleep
  STATE_SLEEP = 5,
  STATE_IO = 6,       //This thread is blocked until I/O Completion
  STATE_FILE = 7
};


#define STACK_TOP(stack)    \
  (&(((uint16_t *)stack)[(sizeof(stack) / sizeof(uint16_t)) - 1]))


void yield();



void call_fcn_ptr(void (*tp)());


#define PUSH_GPR()        \
  __asm__("push r0");     \
  __asm__("push r1");     \
  __asm__("push r2");     \
  __asm__("push r3");     \
  __asm__("push r4");     \
  __asm__("push r5");     \
  __asm__("push r6");     \
  __asm__("push r7");     \
  __asm__("push r8");     \
  __asm__("push r9");     \
  __asm__("push r10");    \
  __asm__("push r11");    \
  __asm__("push r12");    \
  __asm__("push r13");    \
  __asm__("push r14");    \
  __asm__("push r15");    \
  __asm__("push r16");    \
  __asm__("push r17");    \
  __asm__("push r18");    \
  __asm__("push r19");    \
  __asm__("push r20");    \
  __asm__("push r21");    \
  __asm__("push r22");    \
  __asm__("push r23");    \
  __asm__("push r24");    \
  __asm__("push r25");    \
  __asm__("push r26");    \
  __asm__("push r27");    \
  __asm__("push r28");    \
  __asm__("push r29");    \
  __asm__("push r30");    \
  __asm__("push r31")


#define PUSH_PC()          \
  __asm__("push r0")


#define PUSH_STATUS()         \
  __asm__("push r31");        \
  __asm__("in r31,__SREG__"); \
  __asm__("push r31")


#define POP_PC()            \
  __asm__("pop r0")


#define POP_STATUS()          \
  __asm__("pop r31");         \
  __asm__("out __SREG__,r31");\
  __asm__("pop r31")


//Pop the general purpose registers
#define POP_GPR()         \
  __asm__("pop r31");     \
  __asm__("pop r30");     \
  __asm__("pop r29");     \
  __asm__("pop r28");     \
  __asm__("pop r27");     \
  __asm__("pop r26");     \
  __asm__("pop r25");     \
  __asm__("pop r24");     \
  __asm__("pop r23");     \
  __asm__("pop r22");     \
  __asm__("pop r21");     \
  __asm__("pop r20");     \
  __asm__("pop r19");     \
  __asm__("pop r18");     \
  __asm__("pop r17");     \
  __asm__("pop r16");     \
  __asm__("pop r15");     \
  __asm__("pop r14");     \
  __asm__("pop r13");     \
  __asm__("pop r12");     \
  __asm__("pop r11");     \
  __asm__("pop r10");     \
  __asm__("pop r9");      \
  __asm__("pop r8");      \
  __asm__("pop r7");      \
  __asm__("pop r6");      \
  __asm__("pop r5");      \
  __asm__("pop r4");      \
  __asm__("pop r3");      \
  __asm__("pop r2");      \
  __asm__("pop r1");      \
  __asm__("pop r0")



#define SWAP_STACK_PTR(OLD, NEW) \
  __asm__("in %A0, __SP_L__\n\t in %B0, __SP_H__":"=r"(OLD):);\
  __asm__("out __SP_H__,%B0\n\t out __SP_L__,%A0"::"r"(NEW))
 

#define PREPARE_STACK()                               \
  SWAP_STACK_PTR(old_stack_ptr, current_thread->sp);  \
  __asm__("push %A0\n push %B0"::"r"(thread_wrapper));\
  for(i=0;i<36;i++)                                   \
    __asm__("push __zero_reg__");                     \
  SWAP_STACK_PTR(current_thread->sp, old_stack_ptr)



#endif
