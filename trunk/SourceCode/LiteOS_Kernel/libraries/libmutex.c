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

#include "libsystem.h"
#include "libthread.h"
#include "liteoscommon.h"
#include "../types/types.h"







void lib_file_barrier_block(uint8_t type, uint8_t id){
   
    _atomic_t currentatomic;
	lib_thread **current_thread; 

     
	current_thread = lib_get_current_thread(); 

    currentatomic = _atomic_start();
    
      (*current_thread)->state = STATE_IO;
      (*current_thread)->data.iostate.type = type;
      (*current_thread)->data.iostate.id = id;
      lib_yield();
    
    _atomic_end(currentatomic);
}



