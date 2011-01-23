#include "leds.h"
#include "thread.h"
#include "adc.h"
#include "radio.h"

int msn;
typedef struct 
{ int a;
	int b;
} structab; 

structab cde; 
int reading; 
int array[10];


void crashNode(){

  uint16_t problem_buffer[10]; 

  uint8_t i; 

  for (i=0;i<=12;i++)

    problem_buffer[i] = 0; 

}


int main()
{
 __asm__ __volatile__("sei" ::); 
 	for (msn=0;msn<=10;msn++)
 	  array[msn] = 0; 
 	  crashNode();
	for (msn=0;msn<300;msn++)
	{
  greenToggle();
  cde.a = 1 ;
  cde.b = 2; 
	yellowToggle(); 
	reading = get_light();
	radioSend_uint16(reading); 	
	sleepThread(1000);
	if (msn==299)
		msn = 1; 
	}
	return 0; 
}

