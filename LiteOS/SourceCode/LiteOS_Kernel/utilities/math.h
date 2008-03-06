/* LiteOS Version 0.3 */
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
                                                           



#ifndef MATHH
#define MATHH

#include "../types/types.h"

#if defined(PLATFORM_AVR)
#include <stdlib.h> 
#endif 

/**\defgroup math Mathematics*/

/** \ingroup math 
  
    The getRandomNumber() function returns a 16-bit random number gneerated by the system
*/
uint16_t getRandomNumber(); 

#endif 
