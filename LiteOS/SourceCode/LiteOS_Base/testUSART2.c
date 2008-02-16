#include <avr\io.h>
#include <avr/interrupt.h>
#define xtal 8000000L
#define baud 57600

#include "testUSART2.H"
#include "commandHandle.h"
#include "avrhardware.h"
#include "micazhardware.h"
#include "commandProcessor.h"
#include "Leds.h"

static char cFlag; 
static uint8_t receivebuffer[64]; 


static uint8_t previous; 
static uint8_t status; 

void initUSART()
{
      
	  UBRR0H = 0;
      UBRR0L = 15;

      UCSR0A |= _BV(U2X);  // double speed
      
	  UCSR0C |= _BV(UCSZ1)|_BV(UCSZ0);  // 8 data-bits, 1 stop-bit
      
	  UCSR0B |= _BV(RXCIE)|_BV(RXEN)|_BV(TXEN);  // Enable recieve/transmit/interrupts



   
      
      cFlag = 1; 

//	  currentindex = 0; 

	  previous = 0; 
	  status = 0; 

	      
}

 

void usartPrint(uint8_t c)
  
{
       if (cFlag == 0)
	      initUSART();


        // Loop until UDR register is empty
        while((UCSR0A& (1 <<UDRE0)) == 0);
           
      // Transmit letter a
         UDR0 = c;
}
 

void printString(char *p)
{
  
  //usartPrint(0);
   while ((*p)!='\0')
  {
      usartPrint(*p);
	  p++;
  }
}


void printStringN(char *p, uint8_t n)
{
  uint8_t i; 

  for (i=0;i<n;i++)
  {
      usartPrint(*p);
	  p++;
  }
}

 
//This function prints the value of uint8_t using '0','1',...'f'

void usartPutChipHex(uint8_t cChip)
{
  if (cChip > 9)
    {
      usartPrint('a'+cChip-10);
    }
  else
    {
      usartPrint('0'+cChip);
    }
}


//this function outputs the value of c

void usartPutHex(uint8_t c)
{
  usartPutChipHex(c>>4);
  usartPutChipHex(c&0xf);
}


//this function outputs the value of l . note avr is small-endian so the result is from the right-hand start 

void usartPutLong(uint32_t l)
{
  uint8_t *pcByte = ((char *)(&l)) + 3;
  usartPrint('0');
  usartPrint('x');
  usartPutHex(*pcByte);
  pcByte--;
  usartPutHex(*pcByte);
  pcByte--;
  usartPutHex(*pcByte);
  pcByte--;
  usartPutHex(*pcByte);
}




void printInteger(int32_t a, int32_t b, int32_t c, int32_t d)
{


  usartPrint(1);

  usartPutLong(*(uint32_t*)(&a));
  usartPutLong(*(uint32_t*)(&b));
  usartPutLong(*(uint32_t*)(&c));
  usartPutLong(*(uint32_t*)(&d));

}


/*
int main()
{

   printString("hello, world! Here ai amdfasdsafffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff\n");
     printInteger(-32, 32, 0,0);
   printInteger(-332, 2232, -232,0);
   printInteger(332, 1132, 340,0);
   printInteger(432, 111132, 3430,0);
   printInteger(532, 343432, 0,45);
 
   while(1);
   return 0; 

}

*/

/*
SIGNAL(SIG_UART0_RECV)
{

uint8_t dummy = UDR0; 
currentindex ++; 

if (currentindex ==3)
  {  printString("hello, reply\n\0"); 
     currentindex = 0;
  }

} 
*/


//Must get two sync to begin record. 
//Receive two sync to stop record. and then use the commandHandle to handle it. 

SIGNAL(SIG_UART0_RECV)
{



 uint8_t dummy = UDR0;
 uint8_t currentindex = 0; 
 uint8_t i; 


 if (dummy == 'a')
  {

   _atomic_t _atomic = _atomic_start();

    for (i=0;i<31;i++)
	{
    while (!(UCSR0A & (1<<RXC0)));
	receivebuffer[currentindex++] = UDR0; 
	}  

   _atomic_end(_atomic); 
		
		Leds_greenToggle(); 
		broadcastCommand(receivebuffer, currentindex); 
		//tryoutCommand(receivebuffer); 

  }
 else if (dummy == 'l')
  {
   _atomic_t _atomic = _atomic_start();
    for (i=0;i<63;i++)
	{
    while (!(UCSR0A & (1<<RXC0)));
	receivebuffer[currentindex++] = UDR0; 
	}  
    _atomic_end(_atomic); 
	    Leds_greenToggle(); 
		broadcastCommand(receivebuffer, currentindex); 
   }
 
   /* if (receivebuffer[1] == 122)
     {
	  int j;
	  for (j=0;j<100;j++){
	 printStringN("asdfsaasdssssssssssdddddddddddddddddsfasafsadfasdfsaf",32); 
	 }
	 }
	 */



  

 //receivebuffer[currentindex++] = dummy; 
/*
 if ((status == 1)&&(dummy=='b')&&(previous == 'b'))
 {  
    status = 2; 
	commandHandle(receivebuffer, currentindex-1); 
	status = 0; 
	dummy = 0;
	previous = 0;
	currentindex = 0; 
	return; 
 }


 if ((status == 0)&&(dummy =='a')&&(previous =='a'))
 {
   status = 1; 
   currentindex = 0; 
   previous = 0;  
   return; 
 
 }


 if (status == 0)
  {
    previous = dummy; return;   
  }

 if (status == 1)
 {
    previous = dummy; 
	receivebuffer[currentindex++] = dummy; 
 }
*/

} 



