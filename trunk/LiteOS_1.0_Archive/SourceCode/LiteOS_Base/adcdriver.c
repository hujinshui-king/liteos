#include <avr/io.h>
#include <avr/signal.h>
#include <avr/interrupt.h>
#include "adcdriver.H"

//volatile int flag=0;

/*ISR(ADC_vect){
       ADCSRA |= _BV(ADIF);
       ADCSRA &= ~_BV(ADEN);  // disable interrupt
       flag=1;
}

*/

void init_adc(){
       ADMUX = 0;
       ADCSRA = 6;  // does not enable interrupt and set prescaler to 64
}

int get_adc(int channel){
       int reading;

       ADMUX = 0;
       ADMUX |= (channel & 0x1F);

       ADCSRA |=_BV(ADEN) | _BV(ADSC);  // start conversion

       while ( ADCSRA & ( 1 << ADSC ));
       
       reading = ADCL;
       reading |= (ADCH & 3) << 8;

	   ADCSRA &= ~_BV(ADEN); 

       return reading;
}

int get_light(){
       int reading;

       // turn on --> light sensor : INT1
       PORTE &= ~_BV(6);
       DDRE &= ~_BV(6);
       PORTE |= _BV(5);
       DDRE |= _BV(5);

       reading = get_adc(LIGHT_CHANNEL);

       // turn off
       PORTE &= ~_BV(5);
       DDRE &= ~_BV(5);

       return reading;
}

int get_temp(){
       int reading;

       // turn on --> temp sensor : INT2
       PORTE &= ~_BV(5);
       DDRE &= ~_BV(5);
       PORTE |= _BV(6);
       DDRE |= _BV(6);

       reading = get_adc(TEMP_CHANNEL);

       // turn off
       PORTE &= ~_BV(6);
       DDRE &= ~_BV(6);

       return reading;
}

int get_magx(){
       int reading;

       // turn on --> mag sensor : PW5
       DDRC |=_BV(5);
       PORTC |= _BV(5);

       reading = get_adc(MAGX_CHANNEL);

       // turn off
       DDRC &= ~_BV(5);
       PORTC &= ~_BV(5);

       return reading;
}

int get_magy(){
       int reading;

       // turn on --> mag sensor : PW5
       DDRC |= _BV(5);
       PORTC |= _BV(5);

       reading = get_adc(MAGY_CHANNEL);

       // turn off
       DDRC &= ~_BV(5);
       PORTC &= ~_BV(5);

       return reading;
}

int get_accx(){
       int reading;

       // turn on --> acc sensor : PW4
       DDRC |=_BV(4);
       PORTC |= _BV(4);

       reading = get_adc(ACCX_CHANNEL);

       // turn off
       DDRC &= ~_BV(4);
       PORTC &= ~_BV(4);

       return reading;
}

int get_accy(){
       int reading;

       // turn on --> acc sensor : PW4
       DDRC |= _BV(4);
       PORTC |= _BV(4);

       reading = get_adc(ACCY_CHANNEL);

       // turn off
       DDRC &= ~_BV(4);
       PORTC &= ~_BV(4);

       return reading;
}
