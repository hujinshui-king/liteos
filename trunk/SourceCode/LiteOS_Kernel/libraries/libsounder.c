/* added by Qi Mi (qm8e@virginia.edu) */

#include "libsounder.h"
#include "liteoscommon.h"
#include "../types/types.h"
void lib_sounder_on()
{
                          
  void (*sounderfp)() = (void (*)(void))SOUNDER_ON_FUNCTION; 
  sounderfp();    
}



void lib_sounder_off()
{
                          
  void (*sounderfp)() = (void (*)(void))SOUNDER_OFF_FUNCTION; 
  sounderfp();    
}

