/* The LiteOS Operating System Kernel */
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
#ifndef FSAPIH
#define FSAPIH
#include "../types/types.h"

/**\addtogroup filesystem */

/** @{ */
typedef struct
{
    uint8_t valid;
    uint8_t index;
    uint8_t addr;
    uint8_t mode;
    uint16_t size;
    int32_t fpos;
} fid, *fidptr, MYFILE;

/** This function opens a file according to a pathname and the mode. The file opened must be a file, not program, device driver and directory 
*/
MYFILE *fopen2(char *pathname, char *mode);

/**This function closes the file  */
void fclose2(MYFILE * fp);

/**This function changes teh current location of fpos parameter */
int fseek2(MYFILE * fp, int32_t offset, int position);

/** This function checks if the file specificed by pathname exists or not 
@param pathname The pathname of the file.
@return -1 if not exist, or the block id of the file if it does exist 
*/
int fexist2(char *pathname);

/** This function creates a directory as specified by the pathname parameter 
*/
int fcreatedir2(char *pathname);

/** Right now this is implemented in a simple way. Bsaically it checks the pathname and see if the file is there. If it is, 
then it deletes the block of the file.  
*/
int fdelete2(char *pathname);

/** Read file into buffer for nBytes */
int fread2(MYFILE * fp, void *buffer, int nBytes);

/**Write file into buffer for nBytes */
int fwrite2(MYFILE * fp, void *buffer, int nBytes);

/** locate the file and change its parent name info and should be ok 
to be implemented: only changes the parent information is necessary 
*/
int fmove(char *source, char *target);

/** to be implemented. Basically what this does is to generate a new file,copy the file information, and allocate new spaces and copy blocks   it uses an interface called copyflash(sourcepage, targetpage)
*    Implemented only to support files and the copy operation 
*/
int fcopy(char *source, char *target);

/**print all information for debugging */
void fsprintall();

/**change dir based on directory information */
void fchangedir(char *path);

/**get the current directory location */
void fcurrentdir(char *buffer, int size);

/**get the addr location file info */
void finfonode(char *buffer, int addr);

/**search for file information  */
void fsearch(uint8_t * addrlist, uint8_t * size, char *string);

/** Get node file information */
void fdirnode(char *buffer, int size, int addr);

/**Format the whole file system */
void formatSystem();

//-------------------------------------------------------------------------
int fcheckFSAllocation();

//-------------------------------------------------------------------------
int fcheckStorageAllocation();

/** @} */
#endif
