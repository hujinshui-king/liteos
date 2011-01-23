#include "ClockRaw.h"
#include "TimerRaw.h"

extern volatile uint16_t * old_stack_ptr ;

extern volatile uint16_t *stackinterrupt_ptr; 

inline   
uint8_t HPLClock_Clock_readCounter(void)
{
	return TCNT0;
  //return * (volatile unsigned char *)(unsigned int )& * (volatile unsigned char *)(0x32 + 0x20);
}
inline   
uint8_t HPLClock_Clock_getInterval(void)
{
   return OCR0;
  //return * (volatile unsigned char *)(unsigned int )& * (volatile unsigned char *)(0x31 + 0x20);
}


inline  result_t HPLClock_Clock_fire(void){
  unsigned char result;
  result = TimerM_Clock_fire();
  return result;
}


inline   
void HPLClock_Clock_setInterval(uint8_t value)
{
	OCR0 = value;
  //* (volatile unsigned char *)(unsigned int )& * (volatile unsigned char *)(0x31 + 0x20) = value;
}

inline
result_t HPLClock_Clock_setRate(char interval, char scale)
{
	
  scale &= 0x7;
  scale |= 0x8;
  //shoudl be 1011 here in scale 
  { _atomic_t _atomic = _atomic_start();
    {
    	
    	
      cbi(TIMSK, TOIE0);
      cbi(TIMSK, OCIE0);     //Disable TC0 interrupt
      sbi(ASSR, AS0);        //set Timer/Counter0 to be asynchronous
      //from the CPU clock with a second external
      //clock(32,768kHz)driving it.
      outp(scale, TCCR0);    //prescale the timer to be clock/128 to make it
      outp(0, TCNT0);
      outp(interval, OCR0);
      sbi(TIMSK, OCIE0);
      
      
     /* * (volatile unsigned char *)(unsigned int )& * (volatile unsigned char *)(0x37 + 0x20) &= ~(1 << 0);
      * (volatile unsigned char *)(unsigned int )& * (volatile unsigned char *)(0x37 + 0x20) &= ~(1 << 1);
      * (volatile unsigned char *)(unsigned int )& * (volatile unsigned char *)(0x30 + 0x20) |= 1 << 3;


      * (volatile unsigned char *)(unsigned int )& * (volatile unsigned char *)(0x33 + 0x20) = scale;
      * (volatile unsigned char *)(unsigned int )& * (volatile unsigned char *)(0x32 + 0x20) = 0;
      * (volatile unsigned char *)(unsigned int )& * (volatile unsigned char *)(0x31 + 0x20) = interval;
      * (volatile unsigned char *)(unsigned int )& * (volatile unsigned char *)(0x37 + 0x20) |= 1 << 1;
      */
    }
    _atomic_end(_atomic); }
  return SUCCESS;
}




_INTERRUPT(SIG_OUTPUT_COMPARE0)
{

  
  if (is_thread())
   {SWAP_STACK_PTR(stackinterrupt_ptr, old_stack_ptr);  }

  { _atomic_t _atomic = _atomic_start();
   

    {
      if (HPLClock_set_flag) {
          HPLClock_mscale = HPLClock_nextScale;
          HPLClock_nextScale |= 0x8;
          
          outp(HPLClock_nextScale, TCCR0);
	
	        outp(HPLClock_minterval, OCR0);
	
          HPLClock_set_flag = 0;
        }
    }
    _atomic_end(_atomic); }
  HPLClock_Clock_fire();

 if (is_thread())
 {
  SWAP_STACK_PTR(old_stack_ptr, stackinterrupt_ptr);  
  yield();
 }
}

//_INTERRUPT(SIG_OUTPUT_COMPARE0)
//void __vector_15() __attribute__ ((interrupt, spontaneous, C))
//void __attribute((interrupt))   __vector_15(void)




