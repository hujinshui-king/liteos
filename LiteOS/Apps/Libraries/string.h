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

#ifndef STRINGH
#define STRINGH


/** \defgroup string String Operations

      This module implements common string operations on the mote
*/
    

#include "types.h"

/** \ingroup string
 
    Get the length of the string
*/

int String_length(char* s);

/** \ingroup string
    
    Copy a string with a length parameter
*/
void mystrncpy(char *dest, const char *src, uint16_t n);

/** \ingroup string
  
    Copy a string without a length parameter
*/
void mystrcpy(char *dest, const char *src); 


/** \ingroup string
  
    Append a string to another string
    
*/
void String_append(char *base, char *string);

/** \ingroup string

     Convert an integer to a string
*/
char *String_intToString(int num);

/** \ingroup string
 
     Convert a hexidecimal value to an integer. For example, A is converted to 10 
 
*/
uint16_t hex2value(uint8_t hex);

#endif
