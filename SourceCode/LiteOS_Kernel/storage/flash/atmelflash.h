
#ifndef _ATMEL_FLASH_H_
#define _ATMEL_FLASH_H_
/* \defgroup flash Flash related hardware drivers  
   These definitons collectively form flash related definitons 
 */
/* @{ */
///Initilizes the atmel flash
void atmel_flash_init();
void readFlash(int pagenum, uint8_t offset, void *buffer, int NumOfBytes);
void writeFlash(int pagenum, uint8_t offset, void *buffer, int NumOfBytes);
inline uint16_t atmel_flash_pagesize();
inline uint16_t atmel_flash_pagenumber();
void copyFlash(int sourcepage, int targetpage);

//Compares flash content 
uint8_t atmel_flash_compare(uint8_t * p, uint16_t count);
uint16_t atmel_flash_crc(uint32_t count);

/* @} */
#endif
