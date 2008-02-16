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

 

#ifndef STDFSH
#define STDFSH

//This file serves as an middle layer between the file system API provided to the user and and low level operations

#include "../types/types.h"


//void foobar();


//This file provides basic interfaces for directory and file operations 

//change the directory based on filename. Filename is single-level


/**addtogroup filesystem
*/

/**@{ */


int changeDirectory(char *filename, int directory);

uint8_t fullBlock(int directory);


//create a directory
uint8_t createDir(char *filename, int directory);

//create a file
uint8_t createFileFromDirectory(char *filename, int directory);

//return current directory
int getPwd();

//set current directory 
void setPwd(int directory);

//check wehther a block exists
int existBlock(char *filename, int directory);

//check directory empty
int emptyDirectory(int directory);

//locate a file name
int locateFileName(char *pathname, int *state);

//open a file 
void openFile(int addr, int fid, int mode);

int isDirectory(int addr);

void freeBlocks(int addr);

void newSector(int addr);


void addChildNode(uint8_t addr, uint8_t child);

void removeChildNode(uint8_t addr, uint8_t child);

void getName(char *buffer, int addr);

int existBlockAddr(char *filename, int directory);

uint8_t getRealSector(uint8_t addr, uint8_t sectornum); 


/**@}*/
#endif 

