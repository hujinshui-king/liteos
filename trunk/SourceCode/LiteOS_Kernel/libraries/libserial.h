#ifndef SERIALPRINTH
#define SERIALPRINTH


#include "../types/types.h"  


//No need to be called from main 
//This function is called when the system is being initilized and prints system start 
void lib_init_usart();

//-------------------------------------------------------------------------
void lib_print_usart(uint8_t c);
 



void lib_print_usart_8bit(uint8_t c);

void lib_printf_string_with_id(char *p);
//-------------------------------------------------------------------------
void lib_print_string_lengthN(char *p, uint8_t n);
 
void lib_print_usart_string(char *p);


//-------------------------------------------------------------------------
void lib_usart_put_chip_hex(uint8_t cChip);
//-------------------------------------------------------------------------
void lib_usart_put_long(uint32_t l);
//-------------------------------------------------------------------------
void lib_usart_put_hex(uint8_t c);
//-------------------------------------------------------------------------
void lib_printf_integer32(int32_t a);
//-------------------------------------------------------------------------
void lib_printf_uinteger32(uint32_t l);
//-------------------------------------------------------------------------
void lib_usart_put_long(uint32_t l);
//-------------------------------------------------------------------------
void lib_print_integer(int32_t a, int32_t b, int32_t c, int32_t d);

void lib_print_current_time(); 

void lib_printf_ln();
//-------------------------------------------------------------------------
void lib_printf_integer32(int32_t val);

//-------------------------------------------------------------------------
void lib_printf_uinteger32(uint32_t val);



#endif
