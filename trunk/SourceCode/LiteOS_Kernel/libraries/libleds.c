/*
The following is the license of LiteOS.

This file is part of LiteOS.
Copyright Qing Cao, 2007-2008, University of Illinois , qcao2@uiuc.edu

LiteOS is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

LiteOS is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with LiteOS.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "libleds.h"
#include "liteoscommon.h"
#include "../types/types.h"
void lib_green_toggle()
{
 void (*greenfp)() = (void (*)(void))GREEN_LED_TOGGLE_FUNCTION; 
 greenfp();                              



}

void lib_red_toggle()
{
                          
  void (*redfp)() = (void (*)(void))RED_LED_TOGGLE_FUNCTION; 
  redfp();    
}


void lib_yellow_toggle()
{
                          
  void (*yellowfp)() = (void (*)(void))YELLOW_TOGGLE_FUNCTION; 
  yellowfp();    
}



void lib_red_on()
{
                          
  void (*redfp)() = (void (*)(void))RED_ON_FUNCTION; 
  redfp();    
}



void lib_red_off()
{
                          
  void (*redfp)() = (void (*)(void))RED_OFF_FUNCTION; 
  redfp();    
}



void lib_green_on()
{
                          
  void (*greenfp)() = (void (*)(void))GREEN_ON_FUNCTION; 
  greenfp();    
}


void lib_green_off()
{
                          
  void (*greenfp)() = (void (*)(void))GREEN_OFF_FUNCTION; 
  greenfp();    
}

void lib_yellow_on()
{
                          
  void (*yellowfp)() = (void (*)(void))YELLOW_ON_FUNCTION; 
  yellowfp();    
}

void lib_yellow_off()
{
                          
  void (*yellowfp)() = (void (*)(void))YELLOW_OFF_FUNCTION; 
  yellowfp();    
}
