#include "adcSocket.h"
#include "adcdriver.h"
#include "thread_data.h"

extern volatile thread * current_thread;


void ADCLight()
{
    int temp;
	temp = get_light();
	current_thread->data.adcstate.adcreading = temp;
	return; 
}


void ADCTemp()
{
    int temp;
	temp = get_temp();
	current_thread->data.adcstate.adcreading = temp;
	return; 

}
 
void ADCMagX()
{   int temp;
	temp = get_magx();
	current_thread->data.adcstate.adcreading = temp;
	return;  
}
 
void ADCMagY()
{   int temp;
	temp = get_magy();
	current_thread->data.adcstate.adcreading = temp;
	return;  
}
 
void ADCAccX()
{   int temp;
	temp = get_accx();
	current_thread->data.adcstate.adcreading = temp;
	return;  
}
 
void ADCAccY()
{   int temp;
	temp = get_accy();
	current_thread->data.adcstate.adcreading = temp;
	return;  
}
