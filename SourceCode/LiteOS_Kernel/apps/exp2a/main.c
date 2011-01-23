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
    
   (*current_thread)->ecbptr->share = 300; 
   (*current_thread)->ecbptr->totalenergy =  480000; 
   (*current_thread)->ecbptr->remainenergy = 480000; 
     
   //sbi(MCUCR, SM0);

   //sbi(MCUCR, SM1);
   //cbi(MCUCR, SM2); 
   //sbi(MCUCR, SE);
   
   //printfstring("before send\n");
   while(1)
   	{
	   
	   radioSend_string("hello, world!\n"); 
	   sleepThread(100);
	   //printfstring("sending\n");
	   
	   
    }
   
	return 0; 
}

