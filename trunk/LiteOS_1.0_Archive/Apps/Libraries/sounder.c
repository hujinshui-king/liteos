/* added by Qi Mi (qm8e@virginia.edu) */

#include "sounder.h"
#include "liteoscommon.h"

void sounderOn()
{
                          
  void (*sounderfp)() = (void (*)(void))SOUNDER_ON_FUNCTION; 
  sounderfp();    
}



void sounderOff()
{
                          
  void (*sounderfp)() = (void (*)(void))SOUNDER_OFF_FUNCTION; 
  sounderfp();    
}

