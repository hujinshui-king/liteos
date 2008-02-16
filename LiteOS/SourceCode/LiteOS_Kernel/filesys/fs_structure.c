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

#include "fs_structure.h"
#include "inode.h"
#include "fsconfig.h"
#include "fsapi.h"

fid fidtable[ 4 ];


int checkFsValid() {
   if ( checkNodeValid( FSROOTNODE ) == DIRNODE ) {
      return 1;
   } else {
      return 0;
   }
}
//-------------------------------------------------------------------------
void initFidTable() {
   int i;
   for ( i = 0; i < 8; i ++ ) {
      fidtable[ i ].valid = 0;
   }
}
//-------------------------------------------------------------------------
int getFreeFid() {
   int i;
   for ( i = 0; i < 8; i ++ ) {
      if ( fidtable[ i ].valid == 0 ) {
         break;
      }
   }
   fidtable[ i ].valid = 1;
   fidtable[ i ].index = i;
   fidtable[ i ].addr = fidtable[ i ].mode = 0;
   fidtable[ i ].fpos =  - 1;
   return i;
}
//-------------------------------------------------------------------------
void releaseFid( int fid ) {
   fidtable[ fid ].valid = 0;
}
