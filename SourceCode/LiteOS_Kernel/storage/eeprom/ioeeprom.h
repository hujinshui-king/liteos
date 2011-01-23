//This header provides encapsulation of the eeprom handling routines 
#ifndef IOEEPROMH
#define IOEEPROMH
#include "../../types/types.h"


/**\ingroup eeprom EEPROM Opertions 
The methods are basically used for the file system only. 
this function reads a 8-bit uint from the inode-offset 
*/

/** @{ */

uint8_t eeprom_read8uint(uint16_t addr);
int8_t eeprom_read8int(uint16_t addr);
uint16_t eeprom_read16uint(uint16_t addr);
int16_t eeprom_read16int(uint16_t addr);
uint32_t eeprom_read32uint(uint16_t addr);
int32_t eeprom_read32int(uint16_t addr);
void eeprom_write8uint(uint16_t addr, uint8_t value);
void eeprom_write8int(uint16_t addr, int8_t value);
void eeprom_write16uint(uint16_t addr, uint16_t value);
void eeprom_write16int(uint16_t addr, int16_t value);
void eeprom_write32uint(uint16_t addr, uint32_t value);
void eeprom_write32int(uint16_t addr, int32_t value);
void eeprom_readBytes(uint16_t addr, int nBytes, void *buffer);
void eeprom_writeBytes(uint16_t addr, int nBytes, void *buffer);
void eeprom_initBytes(uint16_t addr, int nBytes, uint8_t value);

/**@}*/
#endif
