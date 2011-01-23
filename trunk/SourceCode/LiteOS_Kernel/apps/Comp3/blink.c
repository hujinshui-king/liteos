#include "leds.h"
#include "thread.h"
#include "adc.h"
#include "radio.h"
#include "serialprint.h"
#include "file.h"
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

int main()
{
 uint16_t index;
 uint8_t index2; 
  thread** current_thread;
 
 __asm__ __volatile__("sei" ::); 
 

	  
    current_thread = getCurrentThread();  
    
   (*current_thread)->ecbptr->share = 500; 
   (*current_thread)->ecbptr->totalenergy =  7500000; 
   (*current_thread)->ecbptr->remainenergy = 7500000; 
     
   sbi(MCUCR, SM0);

   sbi(MCUCR, SM1);
   cbi(MCUCR, SM2); 
   sbi(MCUCR, SE);
   
   
   while(1)
   	{
	   
	   for (index=0;index<1200;index++)
	    for (index2=0;index2<123;index2++)
	        {
	        	__asm__ __volatile__("nop" ::); 
	        }
	   sleepThread(120);
    }
   
	return 0; 
}

