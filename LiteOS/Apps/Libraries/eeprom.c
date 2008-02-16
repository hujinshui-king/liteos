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

#include "radio.h"
#include "system.h"
#include "liteoscommon.h"
#include "mutex.h"
#include "string.h"
#include "thread.h"
#include "eeprom.h"

//Assume that the file system take 0-3199. 

//The remaining 3200-4000 , 25 32-byte locations are provided to store hash data

//And the EEPROM library also allows user to access, read, and write to individual EEPROM locations 




//static uint8_t iocache[8]; 

//#define GLOBALINFOBLOCK 25 



genericByteStorageTaskNode *getCurrentEEPROMInfo()
{
   genericByteStorageTaskNode *currenteeprominfoaddr; 

   void (*getaddrfp)(void) = (void (*)(void))GETCURRENTEEPROMINFOHANDLE; 

   asm volatile("push r20" "\n\t"
                "push r21" "\n\t"
                ::);
   getaddrfp();     
   asm volatile(" mov %A0, r20" "\n\t"
	             "mov %B0, r21" "\n\t"
				 :"=r" (currenteeprominfoaddr)
				 :
                );
    asm volatile("pop r21" "\n\t"
	             "pop r20" "\n\t"
	              ::);
   return currenteeprominfoaddr; 
}




//Turn off the interrupt, access the location, and use system call to implement poll based imlementation provided by avr libc 

void readFromEEPROM(uint16_t addr, uint16_t nBytes, uint8_t *buffer)
{

	
	_atomic_t currentatomic;

	currentatomic = _atomic_start();


    genericByteStorageTaskNode *eeprominfoaddr; 
    eeprominfoaddr = getCurrentEEPROMInfo();
	
	void (*getaddrfp)(void) = (void (*)(void))READFROMEEPROM; 
        
    eeprominfoaddr-> addr = addr;
   	eeprominfoaddr-> nBytes = nBytes;
   	eeprominfoaddr-> buffer = buffer; 

	  
    getaddrfp();     

	_atomic_end(currentatomic); 
}




//Turn off the interrupt, access the location, and use system call to implement poll based implementation provided by avr libc 

void writeToEEPROM(uint16_t addr, uint16_t nBytes, uint8_t *buffer)
{

	_atomic_t currentatomic;

	currentatomic = _atomic_start();

    genericByteStorageTaskNode *eeprominfoaddr; 
    eeprominfoaddr = getCurrentEEPROMInfo();
	
	void (*getaddrfp)(void) = (void (*)(void))WRITETOEEPROM; 
        
    eeprominfoaddr-> addr = addr;
	  eeprominfoaddr-> nBytes = nBytes;
	  eeprominfoaddr-> buffer = buffer; 

	  
    getaddrfp();     
	
	_atomic_end(currentatomic); 


}



