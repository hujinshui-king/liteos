#include "avrhardware.h"
#include "micazhardware.h"
#include "scheduling.h"
#include "Leds.h"
#include "TimerRaw.h"
#include "amcommon.h"
#include "amradio.h"
#include "byteorder.h"
#include "cc2420Const.h"
#include "CC2420ControlM.h"
#include "CC2420RadioM.h"
#include "HPLCC2420FIFOM.h"
#include "HPLCC2420InterruptM.h"
#include "HPLCC2420M.h"
#include "HPLTimer1.h"
#include "Types.h"

//THis file is the organized HPLTIMER1 
//The fire is not connected becuase no one is using it
//only the capture interface is used and has a reverse link 
//To do  is to add the headers and ge tthe function calls outside 




uint8_t HPLTimer1M_set_flag;
uint8_t HPLTimer1M_mscale;

uint8_t HPLTimer1M_nextScale;
uint16_t HPLTimer1M_minterval;




inline  
result_t HPLTimer1M_StdControl_init(void)
{
  { _atomic_t _atomic = _atomic_start();
    {
      HPLTimer1M_mscale = TCLK_CPU_DIV256;
      HPLTimer1M_minterval = TIMER1_DEFAULT_INTERVAL;
    }
    _atomic_end(_atomic); }
  return SUCCESS;
}





inline  

result_t HPLTimer1M_StdControl_start(void)

{
  uint16_t mi;
  uint8_t ms;


  { _atomic_t _atomic = _atomic_start();

    {
      mi = HPLTimer1M_minterval;
      ms = HPLTimer1M_mscale;
    }

    _atomic_end(_atomic); }
  HPLTimer1M_Timer1_setRate(mi, ms);
  return SUCCESS;
}



inline  

result_t HPLTimer1M_StdControl_stop(void){
      uint16_t mi;
   { _atomic_t _atomic = _atomic_start();
   {
		mi = HPLTimer1M_minterval;
   }
   _atomic_end(_atomic); }
    HPLTimer1M_Timer1_setRate(mi, 0);	   //default scale=0=OFF
    return SUCCESS;
}





 inline   

result_t HPLTimer1M_Timer1_setRate(uint16_t interval, char scale)

{


  scale &= 0x7;
  scale |= 0x8;
  { _atomic_t _atomic = _atomic_start();

    {
     outp(0, TCCR1A);	  //stop the timer's clock
		 cbi(TIMSK, OCIE1A);	 //disable output compare
		 cbi(TIMSK, TOIE1);  //disable Overflow interrupts
		 cbi(TIMSK, TICIE1);	 //clear input capture
		 outw(TCNT1L,0);		 //clear the 16bit count 
		 outw(OCR1AL, interval);//set the compare value
		 sbi(TIFR,OCF1A);	//clear Timer1A OCF flag by writing 1
//		sbi(TIMSK, OCIE1A);	  //enable OCIE1A interrupt
		 outp(scale, TCCR1B);	 //starts the timer with sc
    }

    _atomic_end(_atomic); }
  return SUCCESS;
  
  
  
}



 inline   

void HPLTimer1M_CaptureT1_enableEvents(void)

{

   cbi(TCCR1B,WGM13);
   cbi(TCCR1B,WGM12);
   sbi(TIMSK, TICIE1);
}




 inline   

void HPLTimer1M_CaptureT1_clearOverflow(void)

{
    sbi(TIFR,TOV1);
    return;
}




 inline   

void HPLTimer1M_CaptureT1_disableEvents(void)

{
   cbi(TIMSK, TICIE1); //disable
   sbi(TIFR, ICF1);	//clear any pending interrupt}
}




 inline    

result_t HPLTimer1M_Timer1_default_fire(void)

{

  return SUCCESS;
}


inline    result_t HPLTimer1M_Timer1_fire(void){

  unsigned char result;



  result = HPLTimer1M_Timer1_default_fire();



  return result;

}

 inline   

uint16_t HPLTimer1M_CaptureT1_getEvent(void)

{
  uint16_t i;


  { _atomic_t _atomic = _atomic_start();

   i = inw(ICR1L);

    _atomic_end(_atomic); }
  return i;
}



inline    void HPLTimer1M_CaptureT1_captured(uint16_t arg_0xa4d7ac0){

  HPLCC2420InterruptM_SFDCapture_captured(arg_0xa4d7ac0);

}



//void   __vector_11(void) __attribute__((signal, used,   externally_visible)); 
void __attribute((signal))   __vector_11(void) 
{
  HPLTimer1M_CaptureT1_captured(HPLTimer1M_CaptureT1_getEvent());
}




 inline   

bool HPLTimer1M_CaptureT1_isOverflowPending(void)

{
  return( inp(TIFR) & TOV1 );
}



inline 

void HPLTimer1M_CaptureT1_setEdge(uint8_t LowToHigh)

{


   if( LowToHigh )
   sbi(TCCR1B,ICES1);	//rising edge
  else
   cbi(TCCR1B,ICES1);	//falling edge

//Set InputCapture pin PortD pin4 as INPUT
//  TOSH_MAKE_CC_SFD_INPUT();
  
  sbi(TIFR, ICF1);	//clear any pending interrupt
  return;
}





//void  __vector_12(void)  __attribute__((signal, used,   externally_visible)); 
void __attribute((interrupt))  __vector_12(void)

{
  { _atomic_t _atomic = _atomic_start();

    {
      if (HPLTimer1M_set_flag) {
          HPLTimer1M_mscale = HPLTimer1M_nextScale;
          HPLTimer1M_nextScale |= 0x8;
          outp(HPLTimer1M_nextScale, TCCR1B);  //update the clock scale
		      outw(OCR1AL, HPLTimer1M_minterval); //update the compare value
          HPLTimer1M_set_flag = 0;
        }
    }

    _atomic_end(_atomic); }
  HPLTimer1M_Timer1_fire();
}

