#ifndef COMMONDEFINESH
#define COMMONDEFINESH


#include "Types.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
//#include <string.h>
#include <avr/eeprom.h> 
#include "testUSART2.h"





typedef uint8_t boolean;



#ifndef TRUE
#define TRUE  1
#endif
#ifndef true
#define true  1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef false
#define false 0
#endif

#ifndef NULL
#define NULL 0
#endif




//I decide to put the fid table definition here so that every module that includes this file will be able to manipulate fid 
typedef struct 
{
  uint8_t valid;
  uint8_t index;
  uint8_t addr;
  uint8_t mode;
  uint16_t size; 
  int fpos;
} fid, *fidptr, MYFILE;
	

int mystrncmp(char *s, uint8_t start1,  char *t, uint8_t start2, uint8_t length);

 
int strlen(char* s);

int fileMode(char* s);

int superstring(char *string1, char *string2);

#define isLetter(c) (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'))

#define isDigit(c) (('0' <= c && c <= '9'))


void mystrncpy(char *dest, const char *src, size_t n);

uint16_t hex2value(uint8_t hex); 

#endif
