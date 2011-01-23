//This header provides encapsulation of the eeprom handling routines 

#ifndef IOEEPROMH
#define IOEEPROMH

#include <avr/eeprom.h> 
#include "Types.h"

#include "commondefines.h"

uint8_t  read8uint (int inode, int offset);
int8_t   read8int  (int inode, int offset);
uint16_t read16uint(int inode, int offset);
int16_t  read16int (int inode, int offset);
uint32_t read32uint(int inode, int offset);
int32_t  read32int (int inode, int offset);


void  write8uint (int inode, int offset, uint8_t value);
void  write8int  (int inode, int offset, int8_t value);
void  write16uint(int inode, int offset, uint16_t value);
void  write16int (int inode, int offset, int16_t value);
void  write32uint(int inode, int offset, uint32_t value);
void  write32int (int inode, int offset, int32_t value);



void readBytes (int inode, int offset, int nBytes, void *buffer);
void writeBytes(int inode, int offset, int nBytes, void *buffer);

void initBytes(int inode, int offset, int nBytes, uint8_t value);
#endif

/*the test program for this function calls 

#include "io_eeprom.h"

int main(){

 
   uint8_t a, a1;
   int8_t b, b1;
   uint16_t c, c1;
   int16_t d, d1;

   uint32_t r32, w32;

   int32_t  a32, b32; 

   int array1[12], array1a[12];
   int array2[12], array2a[12];

   int i;
   for (i=0;i<12;i++)
   {
     array1[i] = i*2;
	array2[i] = i*3;
   }

   a = 80;
   b = -81;
   c = 160;
   d = -161;

   w32 = 1234567;
   
   a32 = -1234567;



   write8uint(3, 6, a);
   write8int(4,6, b);
   write16uint(0, 1, c);
   write16int(95, 3, d);
   
   writeBytes(45, 1, sizeof(array1), array1);
   writeBytes(46, 1, sizeof(array2), array2);

   a1 = read8uint(3, 6);
   b1 = read8int(4,6);
   c1 = read16uint(0, 1);
   d1 = read16int(95, 3);

   readBytes(45, 1, sizeof(array1a), array1a);
   readBytes(46, 1, sizeof(array2a), array2a);

   write32uint(30, 1, w32);

   r32 = read32uint(30, 1);

   write32int(40, 1, a32);

   b32 = read32int(40, 1);

   return 0;
    
}


*/
