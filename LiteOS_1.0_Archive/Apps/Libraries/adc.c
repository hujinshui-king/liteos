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


#include "adc.h"
#include "liteoscommon.h"
#include "thread.h"
#include "system.h"

int get_light()
{
   thread** current_thread; 
   void (*fp)() = (void (*)(void))ADC_READ_LIGHT;
   current_thread = getCurrentThread();    
   fp(); 
   return (*current_thread)->data.adcstate.adcreading;  
  
}


int get_temp()
{
   thread** current_thread; 
   void (*fp)() = (void (*)(void))ADC_READ_TEMP;
   current_thread = getCurrentThread();    
   fp(); 
   return (*current_thread)->data.adcstate.adcreading;  

}


int get_magx()
{
   thread** current_thread; 
   
   {_atomic_t test = _atomic_start();
   
   void (*fp)() = (void (*)(void))ADC_READ_MAGX;
   current_thread = getCurrentThread();    
   fp(); 
   
   _atomic_end(test);
  }
   return (*current_thread)->data.adcstate.adcreading;  

}


int get_magy()
{
   thread** current_thread; 
  {_atomic_t test = _atomic_start();
   void (*fp)() = (void (*)(void))ADC_READ_MAGY;
   current_thread = getCurrentThread();    
   fp(); 
   
      
   _atomic_end(test);
  }
   return (*current_thread)->data.adcstate.adcreading;  
}


int get_accx()
{
   thread** current_thread; 
   void (*fp)() = (void (*)(void))ADC_READ_ACCX;
   current_thread = getCurrentThread();    
   fp(); 
   return (*current_thread)->data.adcstate.adcreading;  
}


int get_accy()
{
   thread** current_thread; 
   void (*fp)() = (void (*)(void))ADC_READ_ACCY;
   current_thread = getCurrentThread();    
   fp(); 
   return (*current_thread)->data.adcstate.adcreading;  
}
