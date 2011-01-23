#include "avrhardware.h"


void TOSH_wait()
{
  asm volatile("nop");
  asm volatile("nop");
}

inline void _atomic_sleep()
{
  /* Atomically enable interrupts and sleep */
  sei();  // Make sure interrupts are on, so we can wake up!
  asm volatile ("sleep");
  TOSH_wait();
}

inline void __nesc_enable_interrupt() {
  sei();
}

inline void __nesc_disable_interrupt() {
  cli();
}

_atomic_t _atomic_start(void) 
{
  _atomic_t result = inp(SREG);
  cli();
  return result;
}

void _atomic_end(_atomic_t oldSreg)
{
  outp(oldSreg, SREG);
}

