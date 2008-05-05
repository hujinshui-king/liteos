/* added by Qi Mi (qm8e@virginia.edu) */

#include "../avr/avrhardware.h"
#include "micazhardware.h"
#include "sounder.h"
#include "../../utilities/eventlogger.h"

//--------------------------------------
void micaz_sounder() { //activate the sounder
  {
      _atomic_t _atomic = _atomic_start();
	  
      LITE_MAKE_PW2_OUTPUT();

      _atomic_end( _atomic );
   }

}

//--------------------------------------
void micaz_sounderOn() {
    {
      _atomic_t _atomic = _atomic_start();

      micaz_sounder();
      LITE_SET_PW2_PIN();

      _atomic_end( _atomic );
   }
	return;
}

//--------------------------------------
void micaz_sounderOff() {
    {
      _atomic_t _atomic = _atomic_start();

      micaz_sounder();
      LITE_CLR_PW2_PIN();

      _atomic_end( _atomic );
   }
	return;
}
