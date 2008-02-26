#include "leds.h"
#include "thread.h"
#include "radio.h"
#include "string.h"
#include "serial.h"





int main()
{
    int i;
	//or (i=0;i<100;i++)
	
	while (1)
	{
  radioSend_string("Hello, world!\n"); 
	greenToggle();
	sleepThread(100);
	}
	return 0; 
}


