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

     Get a random number
     \param NONE
     \return The generated random number
*/

int lib_random();


/** \ingroup system
     Get node ID
     \param NONE
     \return The current node ID
*/

uint16_t lib_get_node_id();

/** \ingroup system
     Set node ID
     \param nodeid The node id to be set to
     \return NONE
*/

void lib_set_node_id(uint16_t nodeid);


uint32_t get_current_timestamp();

 


#endif
