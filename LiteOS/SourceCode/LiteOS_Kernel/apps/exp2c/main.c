#include "leds.h"
#include "thread.h"
#include "adc.h"
#include "radio.h"
#include "serial.h"
#include "file.h"
#include "liteoscommon.h"
#include "system.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#ifndef sbi

#define sbi(port, bit) ((port) |= _BV(bit))
#define cbi(port, bit) ((port) &= ~_BV(bit))
#define inp(port) (port)
#define inb(port) (port)
#define outp(value, port) ((port) = (value))
#define outb(port, value) ((port) = (value))
#define inw(port) (*(volatile uint16_t *)&(port))
#define outw(port, value) ((*(volatile uint16_t *)&(port)) = (value))
#define PRG_RDB(addr) pgm_read_byte(addr)
#endif


 MYFILE *fileptr;
 int i;
 uint8_t tempbuffer[100]; 
 

int main()
{ 

thread** current_thread;
 __asm__ __volatile__("sei" ::); 
 
  for (i=0;i<100;i++)
   tempbuffer[i] = i; 
	  
    current_thread = getCurrentThread();  
    
(*current_thread)->ecbptr->share = 333; 
   (*current_thread)->ecbptr->totalenergy =  800000; 
   (*current_thread)->ecbptr->remainenergy = 800000; 
     
     
   sbi(MCUCR, SM0);

   sbi(MCUCR, SM1);
   cbi(MCUCR, SM2); 
   sbi(MCUCR, SE);
   
   
 fileptr = mfopen("/file1", "w");
 

 while (1)
 {
    mfwrite(fileptr, tempbuffer, 10);
  	mfseek(fileptr, 0, 0); 
  	  	   
}


mfclose(fileptr);

return 0; 

}

