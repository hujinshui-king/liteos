#include "avrhardware.h"
#include "micazhardware.h"
#include "Leds.h"


//public section:
//public section starts


void Leds_Leds();


void Leds_redOn();


void Leds_redOff();


void Leds_redToggle();


void Leds_greenOn();


void Leds_greenOff();


void Leds_greenToggle();


void Leds_yellowOn();


void Leds_yellowOff();


void Leds_yellowToggle();
//private section:
//private section starts


uint8_t Leds_ledsOn;


enum{Leds_RED_BIT=1,Leds_GREEN_BIT=2,Leds_YELLOW_BIT=4};


void Leds_Leds(){

 //line 54

   { _atomic_t _atomic = _atomic_start();
Leds_ledsOn=0;
TOSH_MAKE_RED_LED_OUTPUT();
TOSH_MAKE_YELLOW_LED_OUTPUT();
TOSH_MAKE_GREEN_LED_OUTPUT();
TOSH_SET_RED_LED_PIN();
TOSH_SET_YELLOW_LED_PIN();
TOSH_SET_GREEN_LED_PIN();
return;

_atomic_end(_atomic); }
}

void Leds_redOn(){

 //line 73

   { _atomic_t _atomic = _atomic_start();
TOSH_CLR_RED_LED_PIN();
Leds_ledsOn|=Leds_RED_BIT;

_atomic_end(_atomic); }

 //line 78
return;
}

void Leds_redOff(){

 //line 87

   { _atomic_t _atomic = _atomic_start();
TOSH_SET_RED_LED_PIN();
Leds_ledsOn&=~Leds_RED_BIT;

_atomic_end(_atomic); }

 //line 92
return;
}

void Leds_redToggle(){

 //line 101

   { _atomic_t _atomic = _atomic_start();
if(Leds_ledsOn&Leds_RED_BIT)Leds_redOff();
else Leds_redOn();

_atomic_end(_atomic); }

 //line 108
return;
}

void Leds_greenOn(){

 //line 118

   { _atomic_t _atomic = _atomic_start();
TOSH_CLR_GREEN_LED_PIN();
Leds_ledsOn|=Leds_GREEN_BIT;

_atomic_end(_atomic); }

 //line 123
return;
}

void Leds_greenOff(){

 //line 132

   { _atomic_t _atomic = _atomic_start();
TOSH_SET_GREEN_LED_PIN();
Leds_ledsOn&=~Leds_GREEN_BIT;

_atomic_end(_atomic); }

 //line 137
return;
}

void Leds_greenToggle(){

 //line 146

   { _atomic_t _atomic = _atomic_start();
if(Leds_ledsOn&Leds_GREEN_BIT)Leds_greenOff();
else Leds_greenOn();

_atomic_end(_atomic); }

 //line 153
return;
}

void Leds_yellowOn(){

 //line 162

   { _atomic_t _atomic = _atomic_start();
TOSH_CLR_YELLOW_LED_PIN();
Leds_ledsOn|=Leds_YELLOW_BIT;

_atomic_end(_atomic); }

 //line 167
return;
}

void Leds_yellowOff(){

 //line 176

   { _atomic_t _atomic = _atomic_start();
TOSH_SET_YELLOW_LED_PIN();
Leds_ledsOn&=~Leds_YELLOW_BIT;

_atomic_end(_atomic); }

 //line 181
return;
}

void Leds_yellowToggle(){

 //line 190

   { _atomic_t _atomic = _atomic_start();
if(Leds_ledsOn&Leds_YELLOW_BIT)Leds_yellowOff();
else Leds_yellowOn();

_atomic_end(_atomic); }

 //line 197
return;
}
