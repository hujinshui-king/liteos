#ifndef CLOCKH
#define CLOCKH


#include "avrhardware.h"
#include "Types.h"

uint8_t HPLClock_set_flag;
uint8_t HPLClock_mscale;
uint8_t HPLClock_nextScale;
uint8_t HPLClock_minterval;


inline   
uint8_t HPLClock_Clock_readCounter(void);

inline   
uint8_t HPLClock_Clock_getInterval(void);


inline  result_t HPLClock_Clock_fire(void);


inline   
void HPLClock_Clock_setInterval(uint8_t value);

inline
result_t HPLClock_Clock_setRate(char interval, char scale);




#endif
