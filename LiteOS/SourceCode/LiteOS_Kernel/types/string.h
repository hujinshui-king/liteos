/* The LiteOS Operating System Kernel */
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
#include "types.h"

/** \defgroup string String Operations 
This module provides basic string operations 
*/

/**@{ */
void mystrcpy(char *dest, const char *src);
void mystrncpy(char *dest, const char *src, uint8_t n);
void strappend(char *base, char *string);
char *str_from_integer(int num);
int superstring(char *string1, char *string2);
int mystrncmp(char *s, uint8_t start1, char *t, uint8_t start2,
              uint8_t length);
int mystrlen(char *s);

/**@} */
#endif
