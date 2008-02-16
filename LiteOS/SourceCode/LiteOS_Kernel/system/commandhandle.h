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




#ifndef COMMANDHANDLEH
#define COMMANDHANDLEH


#include "../types/types.h"
#include "../filesys/fsapi.h"


/**\defgroup commands Interactive Shell API 
     This module implements various command handlers. 
*/

/**@{ */

/** \brief This function defines the command handler for an incoming message. 

      This is the key function that handles the incoming messages. Basically the messages themselves contain the info on how should be delivered and scheduled. 
      \param receivebuffer The pointer to the start location of the incoming messages
      \param total The total number of bytes of the incoming messages
      \return VOID 
 */
 
void  commandHandle(uint8_t *receivebuffer, uint8_t total); 

/** @} */

void InitShell();

extern uint8_t shellbuffer[ 256 ];
void ShellThread();

//uint8_t createThreadFromFile(uint8_t *filename); 


//test the bootloader
//void create_thread_task();


#endif 
