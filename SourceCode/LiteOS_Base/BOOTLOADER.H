#ifndef BOOTLOADERH
#define BOOTLOADERH
#include "Types.h"

__attribute__((section (".bootloader"))) void boot_program_page (uint32_t page, uint8_t *buf);
__attribute__((section (".bootloader"))) int reprogram(unsigned char *buffer, unsigned int codepage); 
__attribute__((section (".bootloader"))) int reprogramall(char *program);
__attribute__((section (".bootloader"))) void ProgramSetPage(uint16_t pageno);
__attribute__((section (".bootloader"))) void ProgramString(char *buffer);
__attribute__((section (".bootloader"))) void ProgramLastString(char *buffer, uint8_t length); 
__attribute__((section (".bootloader"))) void ProgramRemains();

#endif 

