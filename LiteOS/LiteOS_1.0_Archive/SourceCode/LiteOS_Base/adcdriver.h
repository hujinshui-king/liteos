#ifndef ADCDRIVERH
#define ADCDRIVERH


#include <avr/io.h>
#include <avr/signal.h>
#include <avr/interrupt.h>

enum{
       LIGHT_CHANNEL = 1,
       TEMP_CHANNEL = 1,
       ACCX_CHANNEL = 3,
       ACCY_CHANNEL = 4,
       MAGX_CHANNEL = 5,
       MAGY_CHANNEL = 6
};

void init_adc();
int get_adc(int channel);
int get_light();
int get_temp();
int get_magx();
int get_magy();
int get_accx();
int get_accy();


#endif 
