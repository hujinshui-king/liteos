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
 
/*This file is part of mos, the MANTIS Operating System.
 Copyright (C) 2002, 2003, 2004, 2005 University of Colorado, Boulder

 mos is free software; you can redistribute it and/or modify it under
 the terms of the GNU General Public License as published by the Free
 Software Foundation; either version 2 or (at your option) any later version.

 mos is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 for more details.

 You should have received a copy of the GNU General Public License along
 with mos (file GPL-2); if not, write to the Free Software Foundation, Inc.,
 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.

 As a special exception, if other files instantiate templates or use macros
 or inline functions from this file, or you compile this file and link it
 with other works to produce a work based on this file, this file does not
 by itself cause the resulting work to be covered by the GNU General Public
 License. However the source code for this file must still be made available
 in accordance with section (3) of the GNU General Public License.

 This exception does not invalidate any other reasons why a work based on
 this file might be covered by the GNU General Public License.

 This license is adapted from the eCos license, available from
 http://ecos.sourceware.org/license-overview.html

*/



#ifndef _ATMEL_FLASH_H_
#define _ATMEL_FLASH_H_


/* \defgroup flash Flash related hardware drivers  
      These definitons collectively form flash related definitons 
*/


/* @{*/

///Initilizes the atmel flash

void atmel_flash_init();

void readFlash(int pagenum, uint8_t offset, void *buffer, int NumOfBytes);

void writeFlash(int pagenum, uint8_t offset, void *buffer,  int NumOfBytes);

inline uint16_t atmel_flash_pagesize();

inline uint16_t atmel_flash_pagenumber();

void copyFlash( int sourcepage, int targetpage ) ;

//Compares flash content 
uint8_t atmel_flash_compare (uint8_t *p, uint16_t count);


uint16_t atmel_flash_crc(uint32_t count);

/* @} */

#endif
