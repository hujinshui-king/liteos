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
 
 
#include "stdserial.h"
#include "../types/types.h"
#ifdef PLATFORM_AVR
#include "../io/avr_serial/serialprint.h"
#endif
void printfstr(char *str)
{
#ifdef PLATFORM_AVR
    printString(str);
#endif
}

//-------------------------------------------------------------------------
void printfstrlen(char *str, uint8_t len)
{
#ifdef PLATFORM_AVR
    printStringN(str, len);
#endif
}

//-------------------------------------------------------------------------
void printfval(uint8_t val)
{
#ifdef PLATFORM_AVR
    usartPrint(val);
#endif
}

//-------------------------------------------------------------------------
void printfinteger32(int32_t val)
{
#ifdef PLATFORM_AVR
    printInteger32(val);
#endif
}

//-------------------------------------------------------------------------
void printfintegeru32(uint32_t val)
{
#ifdef PLATFORM_AVR
    printIntegerU32(val);
#endif
}
