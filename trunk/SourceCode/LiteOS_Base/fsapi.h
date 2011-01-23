//This interface should provide some basic functionalties including open, close, delete, rename, copy, move, seek, create, read, write 
//copy, move can be implemented using exisitng ones or could be optimized 

#ifndef STDFSH
#define STDFSH

#include "commondefines.h"

MYFILE* fopen2(char *pathname, char *mode);

void  fclose2(MYFILE *fp);
int   fseek2 (MYFILE *fp, int offset, int position);
int   fexist2(char *pathname);
int   fcreatedir2(char *pathname);
int   fdelete2(char *pathname);
int   fread2(MYFILE *fp, void *buffer, int nBytes);
int   fwrite2(MYFILE *fp, void *buffer, int nBytes);
int   fmove(char *source, char *target);
int   fcopy(char *source, char *target); 
int   fcheckEEPROM();
int   fcheckFlash();
void  fsprintall();


void fchangedir(char *path);
void fcurrentdir(char *buffer, int size);

void finfonode(char *buffer, int addr);
void fsearch(uint8_t *addrlist, uint8_t *size, char *string);
void fdirnode(char *buffer, int size, int addr);

void formatSystem();



#endif

