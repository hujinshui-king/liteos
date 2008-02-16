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

#ifndef SYSTEMH
#define SYSTEMH

#include "liteoscommon.h"



/** \defgroup system System Utilities

      This module implements LiteOS system utilities 
*/


/** \ingroup system 

     Definition of _atomic_t type variable. 
*/

typedef uint8_t _atomic_t;


/** \ingroup system

     Start of an atomic code section. 
     \param NONE
     \return The register status
*/

inline _atomic_t _atomic_start(void);


/** \ingroup system

     End of an atomic code section. 
     \param oldSreg The old register status
     \return NONE
*/

inline void _atomic_end(_atomic_t oldSreg);

/** \ingroup system

     Get a random number
     \param NONE
     \return The generated random number
*/

int random();


/** \ingroup system
     Get node ID
     \param NONE
     \return The current node ID
*/

uint16_t getnodeID();

/** \ingroup system
     Set node ID
     \param nodeid The node id to be set to 
     \return NONE 
*/

void setnodeID(uint16_t nodeid);




#endif
