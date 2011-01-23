#ifndef STDFSH
#define STDFSH

//This file serves as an middle layer between the file system API provided to the user and and low level operations

#include "../../types/types.h"

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
