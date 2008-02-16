#ifndef BYTESTORAGEH
#define BYTESTORAGEH



/**\defgroup bytestorage Byte storage hardware independent abstraction
*/



#include "../types/types.h"


/** \ingroup bytestorage 
   
    The generic byte storage task structure 
*/

typedef struct {
  uint16_t addr;
  uint16_t nBytes;
  uint8_t *buffer; 
} genericByteStorageTaskNode; 


/** \ingroup bytestorage 
   
    Read bytes from the byte storage. 
*/

void genericreadBytes (uint16_t addr, int nBytes, void *buffer);

/** \ingroup bytestorage 
   
    Write bytes to the byte storage. 
*/
void genericwriteBytes(uint16_t addr, int nBytes, void *buffer);

/** \ingroup bytestorage 
   
    Init part of the byte storage. 
*/
void initBytes(uint16_t addr, int nBytes, uint8_t value);

/** \ingroup bytestorage 
   
    Read an unsigned 8-bit integer from the byte storage.
*/
uint8_t  read8uint (uint16_t addr);

/** \ingroup bytestorage 
   
    Read a signed 8-bit integer from the byte storage.
*/
int8_t   read8int  (uint16_t addr);

/** \ingroup bytestorage 
   
    Read an unsigned 16-bit integer from the byte storage.
*/
uint16_t read16uint(uint16_t addr);

/** \ingroup bytestorage 
   
    Read a signed 16-bit integer from the byte storage.
*/
int16_t  read16int (uint16_t addr);

/** \ingroup bytestorage 
   
    Read an unsigned 32-bit integer from the byte storage.
*/
uint32_t read32uint(uint16_t addr);

/** \ingroup bytestorage 
   
    Read a signed 32-bit integer from the byte storage.
*/
int32_t  read32int (uint16_t addr);

/** \ingroup bytestorage 
   
    Write an unsigned 8-bit integer from the byte storage.
*/
void  write8uint (uint16_t addr, uint8_t value);

/** \ingroup bytestorage 
   
    Write a signed 8-bit integer to the byte storage.
*/
void  write8int  (uint16_t addr, int8_t value);

/** \ingroup bytestorage 
   
    Write an unsigned 16-bit integer to the byte storage.
*/
void  write16uint(uint16_t addr, uint16_t value);

/** \ingroup bytestorage 
   
    Write a signed 16-bit integer to the byte storage.
*/
void  write16int (uint16_t addr, int16_t value);

/** \ingroup bytestorage 
   
    Write an unsigned 32-bit integer to the byte storage.
*/
void  write32uint(uint16_t addr, uint32_t value);

/** \ingroup bytestorage 
   
    Write a signed 16-bit integer to the byte storage.
*/
void  write32int (uint16_t addr, int32_t value);


/** \ingroup bytestorage 
   
    Get the byte storage task structure address. 
*/
void* getGenericStorageNodeAddress(); 

/** \ingroup bytestorage 
   
    Generic read task for the byte storage.  
*/
void genericReadTask();


/** \ingroup bytestorage 
   
    Generic write task for the byte storage.  
*/
void genericWriteTask(); 



#endif

