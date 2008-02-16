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


#ifndef BOOTLOADERH
#define BOOTLOADERH


#include "../../types/types.h"


/** \defgroup bootloader Bootloader operations 

     \par Overview

      Bootloader operations handle the rewriting of flash, and is used to implement dynamic loading, breakpoints, among other program flash related tasks. 
      This part describes its basic interface and how it is implemented 

      \par Implementation and Usage

      The bootloader API of LiteOS is designed for use with the file system. Normally, we expect that the kernel will read strings out of a file, usually with limited
      number of bytes, then write such bytes into flash. This is basic mechanism that we consider. 

      To use, there are four functions that are related. First, the user should set which page to write by invoking ProgramSetPage(). This function sets the correct page
      number for the application. Then, the user repeatedly invokes ProgramString() to write new strings into this flash. The string is expected to be strings like A0B0, etc. 
      When the last string is written, the user invokes ProgramLastString() to write the last string into the internal buffer. Or the user may invoke ProgramRemains to write the 
      remaining part into the flash as well. 

      \note The functions boot_program_page(), reprogram(), reprogramall() are expected to be internal and not to be used. 
*/



/* Program a page accoridng to the page and buffer information. 
*    \param page number, represented by the byte address of the first byte in this page. 
*    \param buf the buffer that contains the data to be written into the flash. 
*    \return NONE 
*/

void boot_program_page (uint32_t page, uint8_t *buf) __attribute__((section (".bootloader"))) ;

/* Reprogram a page accoridng to the page and buffer information 
*    \param buffer the buffer
*    \param codepage codepage number
*    \return NONE 
*/

int reprogram(unsigned char *buffer, unsigned int codepage) __attribute__((section (".bootloader"))) ; 

/* Reprogram using program information from a varialbe \c program, which is for debugging purposes 

*    \param program the content is the program hex file 
*    \return  0
*/
int reprogramall(unsigned char *program) __attribute__((section (".bootloader"))) ;


/** \ingroup bootloader
      Reprogram using the pageno  
      \param the page number
      \return VOID 
*/

void ProgramSetPage(uint16_t pageno) __attribute__((section (".bootloader")));

/**\ingroup bootloader
     Reprogram a string into the buffer. Only handles 16 bytes of string each time 
*/

void ProgramString(char *buffer) __attribute__((section(".bootloader")));

/** \ingroup bootloader
      Reprogram a string into the buffer , which is the last string. Therefore, this function calls reprogramall directly 
*/

void ProgramLastString(char *buffer, uint8_t length) __attribute__((section (".bootloader"))); 

/**\ingroup bootloader 
     Reprogram the remain part of a buffer
*/

void ProgramRemains() __attribute__((section (".bootloader")));

/** Insert a breakpoint using instructions received by radio 
*/

void boot_insertBreakPoint(uint16_t pagenum, uint8_t pageoffset, uint8_t *instructions) __attribute__((section (".bootloader")));


/** Remove a breakpoint using instructions received by radio 
*/

void boot_removeBreakPoint(uint16_t pagenum, uint8_t pageoffset, uint8_t *instructions) __attribute__((section (".bootloader")));

/**@}*/

#endif 

