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




#ifndef BYTEORDERH
#define BYTEORDERH

#include "types.h"



/** \defgroup type Type definitions and data structures  
      These definitons collectively define how to do some bytes and type information operations 
*/


/**@{*/

/** Find whether the host machine is LSB 
*    \param NONE
*    \return  if it is LSB, return 1, otherwise 0 
*/

inline int is_host_lsb(void);


/** Change a variable into LSB 16 bit
*    \param a the parameter to be changed
*    \return the value in LSB 
*/

inline uint16_t toLSB16(uint16_t a);


/** Change a variable FROM LSB 16 bit
*    \param a the parameter to be changed
*    \return the value in MSB 
*/


inline uint16_t fromLSB16(uint16_t a);

/**@}*/
#endif

