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


#ifndef EVENTTRACEH
#define EVENTTRACEH

#include "liteoscommon.h"

typedef struct tracingblock {
	
 uint8_t internaleventid; 
 uint8_t threadid; 
 void (*externaladdtracefp)(); 
 void (*externalapptracefp)(); 
 }  tracingblock;
 
 
void *getEventBlockAddress();

void enableTracing();

void disableTracing();

void addUserTracePoint(uint16_t pagenum, uint16_t pageoffset); 

void addUserTracePoint_longinstruction(uint16_t pagenum, uint16_t pageoffset);



void removeUserTracePoint(uint16_t pagenum, uint16_t pageoffset, uint8_t *buffer);


void removeUserTracePoint_longinstruction(uint16_t pagenum, uint16_t pageoffset, uint8_t *buffer);


#endif 

