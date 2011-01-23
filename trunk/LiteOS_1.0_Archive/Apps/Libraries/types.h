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

#ifndef TYPESH
#define TYPESH

/** \defgroup types Type Defintitions

      This module defines types for the LiteOS kernel
*/

/** \ingroup types*/
/** @{ */
    
typedef signed char int8_t;
typedef unsigned char uint8_t;

typedef int int16_t;
typedef unsigned int uint16_t;

typedef long int32_t;
typedef unsigned long uint32_t;

typedef long long int64_t;
typedef unsigned long long uint64_t;

typedef unsigned int size_t;

typedef unsigned char boolean;

enum  {
  FALSE = 0, 
  TRUE = 1
};

enum  {
  FAIL = 0, 
  SUCCESS = 1
};

typedef uint8_t  result_t;

enum {
  NULL = 0x0
};

enum {
	 MILLISECOND = 1,
	 SECOND = 1000,
	 MINUTE = 60000
	};
	
	

/** @} */

#endif
