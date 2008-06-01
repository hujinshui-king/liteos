#ifndef PAGESTORAGEH
#define PAGESTORAGEH

/**\defgroup pagestorage Hardware-independent page storage abstraction*/
#include "../types/types.h"

/** \ingroup pagestorage 
Get the size of a storage page 
*/
uint16_t getpagesize();

/** \ingroup pagestorage 
Get the total number of pages
*/
uint16_t getpagenumber();

/** \ingroup pagestorage 
Init the page storage
*/
void pagestorageinit();

/** \ingroup pagestorage 
Read from a page
*/
void readpagestorage(int pagenum, uint8_t offset, void *buffer,
                     int NumOfBytes);

/** \ingroup pagestorage 
Write to a page. 
*/
void writepagestorage(int pagenum, uint8_t offset, void *buffer, int
                      NumOfBytes);
#endif
