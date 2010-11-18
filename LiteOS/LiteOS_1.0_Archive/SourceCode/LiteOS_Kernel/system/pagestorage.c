#include "pagestorage.h"
//Note this file connects implementations to the particular hardware.
//In case other platform is used, new platform impelemtnations should be provided in this file 
#ifdef PLATFORM_AVR
#include "../platform/micaz/atmelflash.h"
//Get the size of each page
uint16_t getpagesize()
{
    return atmel_flash_pagesize();
}

//Get the total number of pages
uint16_t getpagenumber()
{
    return atmel_flash_pagenumber();
}

//Init the page storage
void pagestorageinit()
{
    atmel_flash_init();
}

//Read from a page. Intra-page only.
void readpagestorage(int pagenum, uint8_t offset, void *buffer, int NumOfBytes)
{
    readFlash(pagenum, offset, buffer, NumOfBytes);
}

//Write to a page.  Intra-page only. 
void writepagestorage(int pagenum, uint8_t offset, void *buffer, int
                      NumOfBytes)
{
    writeFlash(pagenum, offset, buffer, NumOfBytes);
}

//-------------------------------------------------------------------------
#endif
