#ifndef STDSERIALH
#define STDSERIALH

#include "../types/types.h"

/**\ingroup serial
    Print a string via the serial port. 
*/
void printfstr(char *str);

/**\ingroup serial
    Print a string with a certain length through the serial port. 
*/
void printfstrlen(char *str, uint8_t len);
 
 /**\ingroup serial
     Print a value through the serial port. 
*/
void printfval(uint8_t val); 

#endif 

