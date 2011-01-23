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
//This header provides encapsulation of the eeprom handling routines 
#ifndef IOEEPROMH
#define IOEEPROMH
#include "../../types/types.h"

/** \defgroup eeprom EEPROM Operations
This module defines EEPROM operations to read/write into EEPROM and basic abstractions 
The file system actually takes 32*97 = 3104. The 3105 starts the internal file network name stuff. 
The following code is used to test this module. 
\code
#include <ioeeprom.h>
int main(){
uint8_t a, a1;
int8_t b, b1;
uint16_t c, c1;
int16_t d, d1;
uint32_t r32, w32;
int32_t  a32, b32; 
int array1[12], array1a[12];
int array2[12], array2a[12];
int i;
for (i=0;i<12;i++)
{
array1[i] = i*2;
array2[i] = i*3;
}
a = 80;
b = -81;
c = 160;
d = -161;
w32 = 1234567;
a32 = -1234567;
write8uint(3, 6, a);
write8int(4,6, b);
write16uint(0, 1, c);
write16int(95, 3, d);
writeBytes(45, 1, sizeof(array1), array1);
writeBytes(46, 1, sizeof(array2), array2);
a1 = read8uint(3, 6);
b1 = read8int(4,6);
c1 = read16uint(0, 1);
d1 = read16int(95, 3);
readBytes(45, 1, sizeof(array1a), array1a);
readBytes(46, 1, sizeof(array2a), array2a);
write32uint(30, 1, w32);
r32 = read32uint(30, 1);
write32int(40, 1, a32);
b32 = read32int(40, 1);
return 0;
}
\endcode
*/

/**\ingroup eeprom
EEPROM informatino node 
*/

/**\addtogroup eeprom 
The methods are basically used for the file system only. 
this function reads a 8-bit uint from the inode-offset 
*/

/** @{ */
uint8_t eeprom_read8uint(uint16_t addr);
int8_t eeprom_read8int(uint16_t addr);
uint16_t eeprom_read16uint(uint16_t addr);
int16_t eeprom_read16int(uint16_t addr);
uint32_t eeprom_read32uint(uint16_t addr);
int32_t eeprom_read32int(uint16_t addr);
void eeprom_write8uint(uint16_t addr, uint8_t value);
void eeprom_write8int(uint16_t addr, int8_t value);
void eeprom_write16uint(uint16_t addr, uint16_t value);
void eeprom_write16int(uint16_t addr, int16_t value);
void eeprom_write32uint(uint16_t addr, uint32_t value);
void eeprom_write32int(uint16_t addr, int32_t value);
void eeprom_readBytes(uint16_t addr, int nBytes, void *buffer);
void eeprom_writeBytes(uint16_t addr, int nBytes, void *buffer);
void eeprom_initBytes(uint16_t addr, int nBytes, uint8_t value);

/**@}*/
#endif
