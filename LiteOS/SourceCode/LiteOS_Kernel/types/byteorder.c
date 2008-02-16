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
 
#include "byteorder.h"

//-------------------------------------------------------------------------
inline int is_host_lsb( void ) {
   const uint8_t n[ 2 ] =  {
      1, 0
   };
   return *( uint16_t* )n == 1;
}

//-------------------------------------------------------------------------
inline uint16_t toLSB16( uint16_t a ) {
   return is_host_lsb() ? a : ( a << 8 ) | ( a >> 8 );
}

//-------------------------------------------------------------------------
inline uint16_t fromLSB16( uint16_t a ) {
   return is_host_lsb() ? a : ( a << 8 ) | ( a >> 8 );
}
