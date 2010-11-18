#include "leds.h"
#include "thread.h"
#include "adc.h"
#include "radio.h"
#include "serial.h"
#include "file.h"
#include "system.h"


thread *mythread; 
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


#define PUSH_REG_STATUS()         \
  __asm__("push r31");        \
  __asm__("in r31,__SREG__"); \
  __asm__("push r31")


#define POP_PC()            \
  __asm__("pop r0")


#define POP_REG_STATUS()          \
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

uint8_t i,j,k; 

void test()
{ 
	PUSH_GPR();     
  i = 1; 
  

  
 
  j = i * 2; 
  k = i + j; 
  for (i=0;i<10;i++)
    greenToggle();
  k = j * 3; 
  
  
  j = i * 2; 
  k = i + j; 
  for (i=0;i<10;i++)
    greenToggle();
  k = j * 3; 
  
  
  j = i * 2; 
  k = i + j; 
  for (i=0;i<10;i++)
    greenToggle();
  k = j * 3; 
  
  
  
  j = i * 2; 
  k = i + j; 
  for (i=0;i<10;i++)
    greenToggle();
  k = j * 3; 
  
  
  j = i * 2; 
  k = i + j; 
  for (i=0;i<10;i++)
    greenToggle();
  k = j * 3; 
 
  POP_GPR(); 
  	  
}

int main()
{
 uint8_t index;  
 __asm__ __volatile__("sei" ::); 
 

 thread** current_thread;

   
 mythread = *current_thread;
   
   

 //setTimerFunction(10, 0, test);

while (1)
{ 
// hibernateThread();
 sleepThread(55); 
 radioSend_string("Hello, world!\n"); 
}


   
	return 0; 
}

