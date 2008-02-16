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
 
#include "ioeeprom.h"
#include "../avr/avrhardware.h"

#include <avr/eeprom.h> 

//-------------------------------------------------------------------------
void eeprom_readBytes( uint16_t addr, int nBytes, void *buffer ) {
   eeprom_busy_wait();
   eeprom_read_block( buffer, ( void* )addr, nBytes );
   return ;
}

//-------------------------------------------------------------------------
void eeprom_writeBytes( uint16_t addr, int nBytes, void *buffer ) {
   eeprom_busy_wait();
   eeprom_write_block( buffer, ( void* )addr, nBytes );
   return ;
}

//-------------------------------------------------------------------------
uint8_t eeprom_read8uint( uint16_t addr) {
  
   uint8_t ret;
   _atomic_t _atomic = _atomic_start();
   eeprom_busy_wait();
   ret = eeprom_read_byte(( uint8_t* )addr );
   _atomic_end( _atomic );
   return ret;
}

//-------------------------------------------------------------------------
int8_t eeprom_read8int( uint16_t addr ) {
   uint8_t value;
   _atomic_t _atomic = _atomic_start();
   eeprom_busy_wait();
   value = eeprom_read_byte(( uint8_t* )addr );
   _atomic_end( _atomic );
   return *( int8_t* )& value;
}

//-------------------------------------------------------------------------
uint16_t eeprom_read16uint(uint16_t addr ) {
   uint16_t value;
   _atomic_t _atomic = _atomic_start();
   eeprom_busy_wait();
   value = eeprom_read_word(( uint16_t* )addr );
   _atomic_end( _atomic );
   return value;
}

//-------------------------------------------------------------------------
int16_t eeprom_read16int( uint16_t addr) {

   uint16_t value;
   _atomic_t _atomic = _atomic_start();
   eeprom_busy_wait();
   value = eeprom_read_word(( uint16_t* )addr );
   _atomic_end( _atomic );
   return *( int16_t* )& value;
}

//-------------------------------------------------------------------------
void eeprom_write8uint(  uint16_t addr, uint8_t value ) {
   _atomic_t _atomic = _atomic_start();
   eeprom_busy_wait();
   eeprom_write_byte(( uint8_t* )addr, value );
   _atomic_end( _atomic );
   return ;
}

//-------------------------------------------------------------------------
void eeprom_write8int(  uint16_t addr, int8_t value ) {
   
   uint8_t value2;
   _atomic_t _atomic = _atomic_start();
   eeprom_busy_wait();
   value2 = *( uint8_t* )& value;
   eeprom_write_byte(( uint8_t* )addr, value2 );
   _atomic_end( _atomic );
   return ;
}

//-------------------------------------------------------------------------
void eeprom_write16uint(  uint16_t addr, uint16_t value ) {
   _atomic_t _atomic = _atomic_start();
   eeprom_busy_wait();
   eeprom_write_word(( uint16_t* )addr, value );
   _atomic_end( _atomic );
   return ;
}

//-------------------------------------------------------------------------
void eeprom_write16int(  uint16_t addr, int16_t value ) {
   uint16_t value2;
   _atomic_t _atomic = _atomic_start();
   eeprom_busy_wait();
   value2 = *( uint16_t* )& value;
   eeprom_write_word(( uint16_t* )addr, value2 );
   _atomic_end( _atomic );
   return ;
}

//-------------------------------------------------------------------------
uint32_t eeprom_read32uint(  uint16_t addr ) {
   uint16_t value1;
   uint16_t value2;
   _atomic_t _atomic = _atomic_start();
   eeprom_busy_wait();
   value1 = eeprom_read_word(( uint16_t* )addr );
   eeprom_busy_wait();
   value2 = eeprom_read_word(( uint16_t* )( addr + 2 ));
   _atomic_end( _atomic );
   return ((( uint32_t )value2 ) << 16 ) + ( uint32_t )value1;
}

//-------------------------------------------------------------------------
int32_t eeprom_read32int(  uint16_t addr ) {
   uint16_t value1;
   uint16_t value2;
   uint32_t returnvalue;
   _atomic_t _atomic = _atomic_start();
   eeprom_busy_wait();
   value1 = eeprom_read_word(( uint16_t* )addr );
   eeprom_busy_wait();
   value2 = eeprom_read_word(( uint16_t* )( addr + 2 ));
   returnvalue = (((( uint32_t )value2 ) << 16 ) + ( uint32_t )value1 );
   _atomic_end( _atomic );
   return *( int32_t* )& returnvalue;
}

//-------------------------------------------------------------------------
void eeprom_write32uint(  uint16_t addr, uint32_t value ) {
   uint16_t value1;
   uint16_t value2;
   _atomic_t _atomic = _atomic_start();
   value1 = ( uint16_t )( value >> 16 );
   value2 = ( uint16_t )( value& 0x0000ffff );
   eeprom_busy_wait();
   //value2 = *(uint16_t *)&value;
   eeprom_write_word(( uint16_t* )addr, value2 );
   addr = addr +2; 
   eeprom_busy_wait();
   //value2 = *(uint16_t *)&value;
   eeprom_write_word(( uint16_t* )addr, value1 );
   _atomic_end( _atomic );
   return ;
}

//-------------------------------------------------------------------------
void eeprom_write32int(  uint16_t addr, int32_t value ) {
   uint16_t value1;
   uint32_t value2;
   _atomic_t _atomic = _atomic_start();
   value1 = ( uint16_t )(( *( uint32_t* )& value ) >> 16 );
   value2 = ( uint16_t )(( *( uint32_t* )& value )& 0x0000ffff );
   eeprom_busy_wait();
   //value2 = *(uint16_t *)&value;
   eeprom_write_word(( uint16_t* )addr, value2 );
   addr = addr + 2;
   eeprom_busy_wait();
   //value2 = *(uint16_t *)&value;
   eeprom_write_word(( uint16_t* )addr, value1 );
   _atomic_end( _atomic );
   return ;
}


void eeprom_initBytes(uint16_t addr, int nBytes,  uint8_t value)
{
   int i;
   _atomic_t _atomic = _atomic_start();
   for ( i = 0; i < nBytes; i ++ ) {
      eeprom_write8uint( addr + i, value );
   } 
  _atomic_end( _atomic );	
}
