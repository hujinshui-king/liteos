 
 
#include "stdserial.h"
#include "../../types/types.h"
#include "../../config/nodeconfig.h"

#ifdef PLATFORM_AVR
#include "../avrserial/serialprint.h"
#endif


void printfstr(char *str)
{
	
	_atomic_t currentatomic;

    currentatomic = _atomic_start();
#ifdef PLATFORM_AVR
		#ifdef PRINT_SOURCE_ENABLED
	  printString(node_readnodestring());
	  printString(": ");
	  #endif
    printString(str);
#endif
  _atomic_end(currentatomic);
}

void printfstrln()
 {
 	_atomic_t currentatomic;

    currentatomic = _atomic_start();
  #ifdef PLATFORM_AVR
     printStringLn();
  #endif
  _atomic_end(currentatomic);
  
 }

//-------------------------------------------------------------------------
void printfstrlen(char *str, uint8_t len)
{
#ifdef PLATFORM_AVR
    #ifdef PRINT_SOURCE_ENABLED
	  printString(node_readnodestring());
	  printString(": ");
	  #endif
    printStringN(str, len);
#endif
}

void printfstrlen_base(char *str, uint8_t len)
{
#ifdef PLATFORM_AVR
    printStringN_base(str, len);
#endif
}


//-------------------------------------------------------------------------
void printfval(uint8_t val)
{
#ifdef PLATFORM_AVR
    usartPrint(val);
#endif
}

//-------------------------------------------------------------------------
void printfinteger32(int32_t val)
{
	_atomic_t currentatomic;

    currentatomic = _atomic_start();
#ifdef PLATFORM_AVR
	#ifdef PRINT_SOURCE_ENABLED
	  printString(node_readnodestring());
	  printString(": ");
	#endif
    printInteger32(val);
#endif
   _atomic_end(currentatomic); 
}

//-------------------------------------------------------------------------
void printfintegeru32(uint32_t val)
{
	_atomic_t currentatomic;

    currentatomic = _atomic_start();
    
#ifdef PLATFORM_AVR
  #ifdef PRINT_SOURCE_ENABLED
	  printString(node_readnodestring());
	  printString(": ");
	#endif
    printIntegerU32(val);
#endif
_atomic_end(currentatomic); 
}
