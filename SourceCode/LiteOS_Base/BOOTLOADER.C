#include "bootloader.h"
#include "Types.h"
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/boot.h>


static uint16_t pageindex; 
static uint8_t pagebuffer[512];
static uint16_t currentpageindex; 
 

__attribute__((section (".bootloader"))) void ProgramSetPage(uint16_t pageno)
{
    int i; 
	for (i =0;i<512;i++)
	   pagebuffer[i] = 0;
	  
	pageindex = pageno;  	
	currentpageindex = 0; 
	
	
}

__attribute__((section (".bootloader"))) void ProgramString(char *buffer)
{
  int i;
  for (i=0;i<16;i++)
   {
     pagebuffer[currentpageindex+i] = buffer[i]; 
   }
   
  currentpageindex+=16; 
  if (currentpageindex==512)
  	{reprogramall(pagebuffer);  	
	 currentpageindex = 0;
    }
  	
	
}

__attribute__((section (".bootloader"))) void ProgramRemains()
{
  reprogramall(pagebuffer); 	
}


__attribute__((section (".bootloader"))) void ProgramLastString(char *buffer, uint8_t length)
{
 int i; 
 for (i=0;i<length;i++)
  pagebuffer[currentpageindex+i] = buffer[i]; 	

 reprogramall(pagebuffer); 	
 currentpageindex = 0; 
}


__attribute__((section (".bootloader"))) int reprogramall(char *program)
{

   unsigned char buffer[256];
   unsigned char a, b;
	   int j;
	   int high, low, all; 

	   for (j=0;j<256;j++)

  {       
          a = program[j*2];
          b = program[j*2+1];
		  if ((a>='0')&&(a<='9'))
		     high = a-'0';
          else
		     high = a-'A'+ 10; 
		     
	      if ((b>='0')&&(b<='9'))
		     low = b-'0';
          else
		     low = b-'A'+ 10; 
          all = high *16+low; 

		  buffer[j] = (unsigned char)all ;           
   }
     reprogram(buffer, pageindex);
    pageindex++;   
   return 0; 
}



 __attribute__((section (".bootloader"))) int reprogram(unsigned char *buffer, uint16_t codepage) {
    boot_program_page((uint32_t)SPM_PAGESIZE*((uint32_t)codepage), buffer);
    return 0; 
}






__attribute__((section (".bootloader"))) void boot_program_page (uint32_t page, uint8_t *buf)
{
uint16_t i;
uint8_t sreg;
// Disable interrupts.

sreg = SREG;
cli();
eeprom_busy_wait ();
boot_page_erase (page);
boot_spm_busy_wait (); // Wait until the memory is erased.

for (i=0; i<SPM_PAGESIZE; i+=2)
{
// Set up little-endian word.
uint16_t w = *buf++;
w += (*buf++) << 8;
boot_page_fill (page + i, w);
}

boot_spm_busy_wait (); 
boot_page_write (page); // Store buffer in flash page.
boot_spm_busy_wait(); // Wait until the memory is written.

// Reenable RWW-section again. We need this if we want to jump back
// to the application after bootloading.

boot_rww_enable ();
// Re-enable interrupts (if they were ever enabled).

SREG = sreg;
}




