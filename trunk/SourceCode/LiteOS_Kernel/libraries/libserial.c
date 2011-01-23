#include "libsystem.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#define xtal 8000000L
#define baud 57600
#include "libserial.h"
#include "libstring.h"
#include "libthread.h"
#include "../types/types.h"


static char cFlag;
static uint8_t previous;
static uint8_t status;
 
_atomic_t flag; 
 

static uint8_t localbuffer[16];


uint8_t itoa(uint16_t value, char*  str, int radix)
{
	  
    int  rem = 0;
    int  pos = 0;
    char ch  = '!' ;
    int i; 
    do
    {
        rem    = value % radix ;
        value /= radix;
        if ( 16 == radix )
        {
            if( rem >= 10 && rem <= 15 )
            {
                switch( rem )
                {
                    case 10:
                        ch = 'a' ;
                        break;
                    case 11:
                        ch ='b' ;
                        break;
                    case 12:
                        ch = 'c' ;
                        break;
                    case 13:
                        ch ='d' ;
                        break;
                    case 14:
                        ch = 'e' ;
                        break;
                    case 15:
                        ch ='f' ;
                        break;
                }
            }
        }
        if( '!' == ch )
        {
            str[pos++] = (char) ( rem + 0x30 );
        }
        else
        {
            str[pos++] = ch ;
        }
    }while( value != 0 );
   // str[pos] = '\0' ;
    
    i=0; 
    
    while (i<pos/2)
    {
        *(str+pos) = *(str+i);       //uses the null character as the temporary storage.
        *(str+i) = *(str + pos - i -1);
        *(str+pos-i-1) = *(str+pos);
        i++;
    }
   
    *(str+pos) = '\0';
    return pos;
}

//No need to be called from main 
//This function is called when the system is being initilized and prints system start 
void lib_init_usart()
{
    UBRR0H = 0;
    UBRR0L = 15;
    UCSR0A |= _BV(U2X0);        // double speed
    UCSR0C |= _BV(UCSZ01) | _BV(UCSZ00);        // 8 data-bits, 1 stop-bit
    UCSR0B |= _BV(RXCIE0) | _BV(RXEN0) | _BV(TXEN0);
    // Enable recieve/transmit/interrupts
    cFlag = 1;
    previous = 0;
    status = 0;
}

//-------------------------------------------------------------------------
void lib_print_usart(uint8_t c)
{

   flag = _atomic_start();
     

    if (cFlag == 0)
    {
        lib_init_usart();
    }
    while ((UCSR0A & (1 << UDRE0)) == 0)
        ;
    UDR0 = c;
	
    _atomic_end(flag);
}

 



void lib_print_usart_8bit(uint8_t c)
{
  
   flag = _atomic_start();
   
	
   lib_print_usart(0xFA);
   lib_print_usart(c);
   lib_print_usart(0xFA);
   
    _atomic_end(flag);
}


//-------------------------------------------------------------------------
void lib_print_usart_string(char *p)
{
   
   flag = _atomic_start();
   
    lib_print_usart(0xFC);
    while ((*p) != '\0')
    {
        lib_print_usart(*p);
        p++;
    }
    lib_print_usart(0xFC);
  
    _atomic_end(flag);
	
}


void lib_printf_string_with_id(char *p)
{     
      
	  itoa(lib_get_node_id(), (char *)localbuffer, 10);
    lib_print_usart_string((char *)localbuffer);
	  lib_print_usart_string(": ");	
	  lib_print_usart_string(p); 
	
}



void lib_printf_string_energy_wrapper(char *p)
{
     lib_thread** current_thread;
	   uint8_t length;
	   
	   length = lib_string_length(p);
	   
     current_thread = lib_get_current_thread();      

  	 //(*current_thread)->ecbptr->remainenergy -= (uint16_t)SERIAL_SEND * (uint16_t)length;           
      
         		 
     lib_printf_string_with_id(p);
     
}
      

//-------------------------------------------------------------------------
void lib_print_string_lengthN(char *p, uint8_t n)
{
    uint8_t i;
    
   flag = _atomic_start();
    
    lib_print_usart(0xFA);
    lib_print_usart(n); 
    for (i = 0; i < n; i++)
    {
        lib_print_usart(*p);
        p++;
    }
  //  usartPrint(0xFA);
	  lib_print_usart(0xFC);
    lib_print_usart_string("\n"); 
    lib_print_usart(0xFC);
    
	_atomic_end(flag);
}


 
 

//-------------------------------------------------------------------------
void lib_usart_put_chip_hex(uint8_t cChip)
{
  
   flag = _atomic_start();
   
    if (cChip > 9)
    {
        lib_print_usart('a' + cChip - 10);
    }
    else
    {
        lib_print_usart('0' + cChip);
    }
	
	 _atomic_end(flag);
}

 

//-------------------------------------------------------------------------
void lib_usart_put_hex(uint8_t c)
{ 
   
   flag = _atomic_start();
   
    lib_usart_put_chip_hex(c >> 4);
    lib_usart_put_chip_hex(c & 0xf);
	
	 _atomic_end(flag);
}

//-------------------------------------------------------------------------
void lib_printf_integer32(int32_t a)
{
   
   flag = _atomic_start();
  
	   
    itoa(lib_get_node_id(), (char *)localbuffer, 10);
    lib_print_usart_string((char *)localbuffer);
    lib_print_usart_string(": ");	
    lib_print_usart(0xFD);
    lib_usart_put_long(*(uint32_t *) (&a));
    lib_print_usart(0xFD);
	
	_atomic_end(flag);
}

//-------------------------------------------------------------------------
void lib_printf_uinteger32(uint32_t l)
{
   flag = _atomic_start();
   
    itoa(lib_get_node_id(), (char *)localbuffer, 10);
    lib_print_usart_string((char *)localbuffer);
    lib_print_usart_string(": ");		
    
    lib_print_usart(0xFE);
    lib_usart_put_long(l);
    lib_print_usart(0xFE);
	
	_atomic_end(flag);
}

//-------------------------------------------------------------------------
void lib_usart_put_long(uint32_t l)
{
    uint8_t *pcByte = ((uint8_t *) (&l)) + 3;

   flag = _atomic_start();
    
    lib_print_usart(*pcByte);
    pcByte--;
    lib_print_usart(*pcByte);
    pcByte--;
    lib_print_usart(*pcByte);
    pcByte--;
    lib_print_usart(*pcByte);
   _atomic_end(flag);
	
} 

//-------------------------------------------------------------------------
void lib_print_integer(int32_t a, int32_t b, int32_t c, int32_t d)
{
 
   flag = _atomic_start();
  
    lib_print_usart(1);
    lib_usart_put_long(*(uint32_t *) (&a));
    lib_usart_put_long(*(uint32_t *) (&b));
    lib_usart_put_long(*(uint32_t *) (&c));
    lib_usart_put_long(*(uint32_t *) (&d));

  _atomic_end(flag);	
}



//-------------------------------------------------------------------------
void lib_printf_ln()
{
#ifdef PLATFORM_AVR
    _atomic_t currentatomic;
    currentatomic = _atomic_start();
    lib_print_usart(0xFC);
    lib_print_usart(0x6D);
    lib_print_usart(0x6D);
    lib_print_usart(0x6D);
    lib_print_usart(0xFC);
	_atomic_end(currentatomic);
    
#endif
}

//-------------------------------------------------------------------------
void lib_printf_integer_u32ln(uint32_t val)
{
#ifdef PLATFORM_AVR
    _atomic_t currentatomic;
    currentatomic = _atomic_start();
     lib_printf_uinteger32(val);
     lib_print_usart_string("\n"); 
	_atomic_end(currentatomic);
   
#endif
}


