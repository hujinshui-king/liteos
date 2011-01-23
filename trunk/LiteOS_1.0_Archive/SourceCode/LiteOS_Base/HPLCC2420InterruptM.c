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



uint8_t HPLCC2420InterruptM_FIFOWaitForState;
uint8_t HPLCC2420InterruptM_FIFOLastState;
 
uint8_t HPLCC2420InterruptM_CCAWaitForState;
uint8_t HPLCC2420InterruptM_CCALastState;
 
 
 

//This function is the start position of the receving pacekts


//void  __vector_7(void) __attribute__((signal, used,   externally_visible)); 
void __attribute((signal))  __vector_7(void)

{
  result_t val = SUCCESS;


  val = HPLCC2420InterruptM_FIFOP_fired();
  if (val == FAIL) {
      * (volatile unsigned char *)(unsigned int )& * (volatile unsigned char *)(0x39 + 0x20) &= ~(1 << 6);
      * (volatile unsigned char *)(unsigned int )& * (volatile unsigned char *)(0x38 + 0x20) |= 1 << 6;
    }
}





 inline   

result_t HPLCC2420InterruptM_FIFOP_startWait(bool low_to_high)

{
  { _atomic_t _atomic = _atomic_start();

    {
      CC2420_FIFOP_INT_MODE(low_to_high);
      * (volatile unsigned char *)(unsigned int )& * (volatile unsigned char *)(0x39 + 0x20) |= 1 << 6;
    }

    _atomic_end(_atomic); }
  return SUCCESS;
}





 inline   

result_t HPLCC2420InterruptM_FIFOP_disable(void)

{
  * (volatile unsigned char *)(unsigned int )& * (volatile unsigned char *)(0x39 + 0x20) &= ~(1 << 6);
  return SUCCESS;
}





inline    result_t HPLCC2420InterruptM_FIFOP_fired(void){

  unsigned char result;



  result = CC2420RadioM_FIFOP_fired();



  return result;

}






inline   result_t HPLCC2420InterruptM_FIFOTimer_start(char arg_0xa315138, uint32_t arg_0xa315290){

  unsigned char result;



  result = TimerM_Timer_start(10, arg_0xa315138, arg_0xa315290);



  return result;

}





 inline    

result_t HPLCC2420InterruptM_FIFO_default_fired(void)

{

  return FAIL;
}


inline    result_t HPLCC2420InterruptM_FIFO_fired(void){

  unsigned char result;



  result = HPLCC2420InterruptM_FIFO_default_fired();



  return result;

}




 inline  

result_t HPLCC2420InterruptM_FIFOTimer_fired(void)

{
  uint8_t FIFOState;
  result_t val = SUCCESS;

  FIFOState = TOSH_READ_CC_FIFO_PIN();
  if (HPLCC2420InterruptM_FIFOLastState != HPLCC2420InterruptM_FIFOWaitForState && FIFOState == HPLCC2420InterruptM_FIFOWaitForState) {

      val = HPLCC2420InterruptM_FIFO_fired();
      if (val == FAIL) {
        return SUCCESS;
        }
    }
  HPLCC2420InterruptM_FIFOLastState = FIFOState;
  return HPLCC2420InterruptM_FIFOTimer_start(TIMER_ONE_SHOT, 1);
}






inline    result_t HPLCC2420InterruptM_CCA_fired(void){

  unsigned char result;



  result = CC2420ControlM_CCA_fired();



  return result;

}

 inline  

result_t HPLCC2420InterruptM_CCATimer_fired(void)

{
  uint8_t CCAState;
  result_t val = SUCCESS;

  CCAState = TOSH_READ_CC_CCA_PIN();

  if (HPLCC2420InterruptM_CCALastState != HPLCC2420InterruptM_CCAWaitForState && CCAState == HPLCC2420InterruptM_CCAWaitForState) {
      val = HPLCC2420InterruptM_CCA_fired();
      if (val == FAIL) {
        return SUCCESS;
        }
    }
  HPLCC2420InterruptM_CCALastState = CCAState;
  return HPLCC2420InterruptM_CCATimer_start(TIMER_ONE_SHOT, 1);
}


 inline   

result_t HPLCC2420InterruptM_CCA_startWait(bool low_to_high)

{
  { _atomic_t _atomic = _atomic_start();

    HPLCC2420InterruptM_CCAWaitForState = low_to_high;

    _atomic_end(_atomic); }
  HPLCC2420InterruptM_CCALastState = TOSH_READ_CC_CCA_PIN();
  return HPLCC2420InterruptM_CCATimer_start(TIMER_ONE_SHOT, 1);
}






inline   result_t HPLCC2420InterruptM_CCATimer_start(char arg_0xa315138, uint32_t arg_0xa315290){

  unsigned char result;



  result = TimerM_Timer_start(11, arg_0xa315138, arg_0xa315290);



  return result;

}







inline    void HPLCC2420InterruptM_SFDCapture_enableEvents(void){

  HPLTimer1M_CaptureT1_enableEvents();

}




inline    void HPLCC2420InterruptM_SFDCapture_clearOverflow(void){

  HPLTimer1M_CaptureT1_clearOverflow();

}


inline    void HPLCC2420InterruptM_SFDCapture_setEdge(uint8_t arg_0xa4c5e80){

  HPLTimer1M_CaptureT1_setEdge(arg_0xa4c5e80);

}





inline    void HPLCC2420InterruptM_SFDCapture_disableEvents(void){

  HPLTimer1M_CaptureT1_disableEvents();

}




 inline   

result_t HPLCC2420InterruptM_SFD_enableCapture(bool low_to_high)

{
  { _atomic_t _atomic = _atomic_start();

    {

      HPLCC2420InterruptM_SFDCapture_disableEvents();
      HPLCC2420InterruptM_SFDCapture_setEdge(low_to_high);
      HPLCC2420InterruptM_SFDCapture_clearOverflow();
      HPLCC2420InterruptM_SFDCapture_enableEvents();
    }

    _atomic_end(_atomic); }
  return SUCCESS;
}






 inline   

result_t HPLCC2420InterruptM_SFD_disable(void)

{
  HPLCC2420InterruptM_SFDCapture_disableEvents();

  return SUCCESS;
}





inline    result_t HPLCC2420InterruptM_SFD_captured(uint16_t arg_0xa41e788){

  unsigned char result;



  result = CC2420RadioM_SFD_captured(arg_0xa41e788);



  return result;

}




inline    bool HPLCC2420InterruptM_SFDCapture_isOverflowPending(void){

  unsigned char result;



  result = HPLTimer1M_CaptureT1_isOverflowPending();



  return result;

}






   

void HPLCC2420InterruptM_SFDCapture_captured(uint16_t time)

{
  result_t val = SUCCESS;

  val = HPLCC2420InterruptM_SFD_captured(time);
  if (val == FAIL) {
      HPLCC2420InterruptM_SFDCapture_disableEvents();
    }
  else 
    {
      if (HPLCC2420InterruptM_SFDCapture_isOverflowPending()) {
        HPLCC2420InterruptM_SFDCapture_clearOverflow();
        }
    }
}

