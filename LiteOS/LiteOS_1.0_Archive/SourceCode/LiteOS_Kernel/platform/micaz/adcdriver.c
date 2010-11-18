/* The LiteOS Operating System Kernel */
/*
   The following is the license of LiteOS.
   This file is part of LiteOS.
   Copyright Qing Cao, 2007-2008, University of Illinois , qcao2@uiuc.edu
   LiteOS is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   LiteOS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with LiteOS.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "adcdriver.h"

/** \ingroup device 
Global variable. The parameter is used by the OS to handle interactive user commands. Basically it is only currently used in association with set_led_task function, which
handles the led status according to the parameter.
*/
static uint8_t adc_parameter[12];

/** \ingroup device 
This is a global variable. The parameter is used to implement ADC tasks and to get the values of return readings . 
*/
static uint16_t adc_returnvalue;
void *getAdcParameterAddr()
{
    return (void *)adc_parameter;
}

//-------------------------------------------------------------------------
void *getAdcReturnValueAddr()
{
    return (void *)&adc_returnvalue;
}

//the definitions of adc implementations 
#define adc_on() ADCSRA |= (1 << ADEN) | (1 << ADIE) | (1 << ADIF)
#define adc_off() ADCSRA &= ~(1 << ADEN)
//The critical ISR function to come back when the interrupt of ADC finishes 
ISR(ADC_vect)
{
    ADCSRA |= _BV(ADIF);
    ADCSRA &= ~_BV(ADEN);       // disable interrupt
    //   flag = 1;
}

//-------------------------------------------------------------------------
void adcdriver_init_adc()
{
    ADMUX = 0;
    ADCSRA = _BV(ADIE) | 6;
    // does not enable interrupt and set prescaler to 64
}

//poll based approach. After all, requrieing another adc thread is just too expensive!
uint16_t get_adc(int channel)
{
    uint16_t reading;

    ADMUX = 0;
    ADMUX |= (channel & 0x1F);
    ADCSRA &= ~(1 << ADIE);
    ADCSRA |= (1 << ADIF);
    ADCSRA |= (1 << ADEN) | (1 << ADSC);
    // start conversion
    while (ADCSRA & (1 << ADSC))
        ;
    //the interrupt based adc is currently not used. 
    //while (flag == 0); 
    //flag = 0; 
    reading = ADCL;
    reading |= (ADCH & 3) << 8;
    ADCSRA &= ~_BV(ADEN);
    return reading;
}

//-------------------------------------------------------------------------
uint16_t get_light()
{
    uint16_t reading;

    PORTE &= ~_BV(6);
    DDRE &= ~_BV(6);
    PORTE |= _BV(5);
    DDRE |= _BV(5);
    reading = get_adc(LIGHT_CHANNEL);
    PORTE &= ~_BV(5);
    DDRE &= ~_BV(5);
    return reading;
}

//-------------------------------------------------------------------------
uint16_t get_temp()
{
    uint16_t reading;

    PORTE &= ~_BV(5);
    DDRE &= ~_BV(5);
    PORTE |= _BV(6);
    DDRE |= _BV(6);
    reading = get_adc(TEMP_CHANNEL);
    PORTE &= ~_BV(6);
    DDRE &= ~_BV(6);
    return reading;
}

//-------------------------------------------------------------------------
uint16_t get_magx()
{
    uint16_t reading;

    DDRC |= _BV(5);
    PORTC |= _BV(5);
    reading = get_adc(MAGX_CHANNEL);
    DDRC &= ~_BV(5);
    PORTC &= ~_BV(5);
    return reading;
}

//-------------------------------------------------------------------------
uint16_t get_magy()
{
    uint16_t reading;

    DDRC |= _BV(5);
    PORTC |= _BV(5);
    reading = get_adc(MAGY_CHANNEL);
    DDRC &= ~_BV(5);
    PORTC &= ~_BV(5);
    return reading;
}

//-------------------------------------------------------------------------
uint16_t get_accx()
{
    uint16_t reading;

    DDRC |= _BV(4);
    PORTC |= _BV(4);
    reading = get_adc(ACCX_CHANNEL);
    DDRC &= ~_BV(4);
    PORTC &= ~_BV(4);
    return reading;
}

//-------------------------------------------------------------------------
uint16_t get_accy()
{
    uint16_t reading;

    DDRC |= _BV(4);
    PORTC |= _BV(4);
    reading = get_adc(ACCY_CHANNEL);
    DDRC &= ~_BV(4);
    PORTC &= ~_BV(4);
    return reading;
}

//-------------------------------------------------------------------------
void get_light_task()
{
    uint16_t reading;

    reading = get_light();
    adc_returnvalue = reading;
    return;
}

//-------------------------------------------------------------------------
void get_temp_task()
{
    uint16_t reading;

    reading = get_temp();
    adc_returnvalue = reading;
    return;
}

//-------------------------------------------------------------------------
void get_acc_task_x()
{
    uint16_t reading;

    reading = get_accx();
    adc_returnvalue = reading;
    return;
}

//-------------------------------------------------------------------------
void get_acc_task_y()
{
    uint16_t reading;

    reading = get_accy();
    adc_returnvalue = reading;
    return;
}

//-------------------------------------------------------------------------
void get_mag_task_x()
{
    uint16_t reading;

    reading = get_magx();
    adc_returnvalue = reading;
    return;
}

//-------------------------------------------------------------------------
void get_mag_task_y()
{
    uint16_t reading;

    reading = get_magy();
    adc_returnvalue = reading;
    return;
}
