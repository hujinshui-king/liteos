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

#include "bootloader.h"
#include "../../types/types.h"

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/boot.h>
#include <avr/io.h>

#include "../../types/byteorder.h"

static uint16_t pageindex;

static uint8_t pagebuffer[ 512 ];

static uint16_t currentpageindex;

static uint8_t breakpointstring[ 8 ] =  {
   0xEC, 0xE8, 0xFA, 0xEE, 0x09, 0x95, 0xFC, 0xCF
};


void  __attribute__(( section( ".bootloader" ))) ProgramSetPage( uint16_t pageno ) {
   int i;
   for ( i = 0; i < 512; i ++ ) {
      pagebuffer[ i ] = 0;
   } 
   pageindex = pageno;
   currentpageindex = 0;
}

//-------------------------------------------------------------------------
void   __attribute__(( section( ".bootloader" ))) ProgramString( char *buffer ){
   int i;
   for ( i = 0; i < 16; i ++ ) {
      pagebuffer[ currentpageindex + i ] = buffer[ i ];
   }
   currentpageindex += 16;
   if ( currentpageindex == 512 ) {
      reprogramall( pagebuffer );
      currentpageindex = 0;
   }
}

//-------------------------------------------------------------------------
void  __attribute__(( section( ".bootloader" ))) ProgramRemains() {

    if (currentpageindex !=0)
		reprogramall( pagebuffer );
}

//-------------------------------------------------------------------------
void  __attribute__(( section( ".bootloader" ))) ProgramLastString( char *buffer, uint8_t length ) {
   int i;
   for ( i = 0; i < length; i ++ ) {
      pagebuffer[ currentpageindex + i ] = buffer[ i ];
   } 
   reprogramall( pagebuffer );
   currentpageindex = 0;
}

//-------------------------------------------------------------------------
int __attribute__(( section( ".bootloader" ))) reprogramall( unsigned char *program )  {
   //unsigned char buffer[256];
   unsigned char a, b;
   int j;
   int high, low, all;
   for ( j = 0; j < 256; j ++ )
    {
      a = program[ j *2 ];
      b = program[ j *2+1 ];
      if (( a >= '0' ) && ( a <= '9' )) {
         high = a - '0';
      } else {
         high = a - 'A' + 10;
      } 
      if (( b >= '0' ) && ( b <= '9' )) {
         low = b - '0';
      } else {
         low = b - 'A' + 10;
      } all = high * 16+low;
      //This might introduce bugs.
      program[ j ] = ( unsigned char )all;
   }
   reprogram( program, pageindex );
   pageindex ++;
   return 0;
}

//-------------------------------------------------------------------------
int  __attribute__(( section( ".bootloader" ))) reprogram( unsigned char *buffer, uint16_t codepage ) {
   boot_program_page(( uint32_t )SPM_PAGESIZE *(( uint32_t )codepage ), buffer );
   return 0;
}
//insert a breakpoint by removing 8 bytes at the location specified by the pagenum and offset 
void  __attribute__(( section( ".bootloader" )))  boot_insertBreakPoint( uint16_t pagenum, uint8_t pageoffset, uint8_t *instructions ){
   //first read the page, then change the string, then write back the page
   int i;
   uint32_t addr;
   uint8_t index;
   uint16_t data;
   addr = ( uint32_t )pagenum *( uint32_t )SPM_PAGESIZE;
   for ( i = 0; i < 128; i ++ ) {
      data = pgm_read_word_far( addr + (( uint32_t )i ) *2 );
      if ( is_host_lsb()) {
         pagebuffer[ i *2 ] = data % 256;
         pagebuffer[ i *2+1 ] = data / 256;
      }
       else {
         pagebuffer[ i *2+1 ] = data % 256;
         pagebuffer[ i *2 ] = data / 256;
      }
   }
   index = pageoffset;
   for ( i = 0; i < 6; i ++ )
    {
      instructions[ i ] = pagebuffer[ index + i ];
      pagebuffer[ index + i ] = breakpointstring[ i ];
   }
   reprogram( pagebuffer, pagenum );
}


//-------------------------------------------------------------------------

//remove a breakpoint by patching the 8 bytes at the location specified at the pagenum and offset
void  __attribute__(( section( ".bootloader" ))) boot_removeBreakPoint( uint16_t pagenum, uint8_t pageoffset, uint8_t *instructions ) {
   //first read the page, then change the string, then write back the page. 
   //first read the page, then change the string, then write back the page
   int i;
   uint32_t addr;
   uint8_t index;
   uint16_t data;
   addr = ( uint32_t )pagenum *( uint32_t )SPM_PAGESIZE;
   for ( i = 0; i < 128; i ++ ) {
      data = pgm_read_word_far( addr + (( uint32_t )i ) *2 );
      if ( is_host_lsb()) {
         pagebuffer[ i *2 ] = data % 256;
         pagebuffer[ i *2+1 ] = data / 256;
      }
       else {
         pagebuffer[ i *2+1 ] = data % 256;
         pagebuffer[ i *2 ] = data / 256;
      }
   }
   index = pageoffset;
   for ( i = 0; i < 6; i ++ )
    {
      pagebuffer[ index + i ] = instructions[ i ];
   }
   reprogram( pagebuffer, pagenum );
}



//-------------------------------------------------------------------------
void __attribute__(( section( ".bootloader" ))) boot_program_page( uint32_t page, uint8_t *buf )  {
   uint16_t i;
   uint8_t sreg;
   // Disable interrupts.
   sreg = SREG;
   cli();
   eeprom_busy_wait();
   boot_page_erase( page );
   boot_spm_busy_wait(); // Wait until the memory is erased.
   for ( i = 0; i < SPM_PAGESIZE; i += 2 ) {
      // Set up little-endian word.
      uint16_t w =  *buf ++;
      w += ( *buf ++ ) << 8;
      boot_page_fill( page + i, w );
   }
   boot_spm_busy_wait();
   boot_page_write( page ); // Store buffer in flash page.
   boot_spm_busy_wait(); // Wait until the memory is written.
   // Reenable RWW-section again. We need this if we want to jump back
   // to the application after bootloading.
   boot_rww_enable();
   // Re-enable interrupts (if they were ever enabled).
   SREG = sreg;
}
