#include "threads.h"
#include "generictimer.h"


 #if defined(PLATFORM_AVR) && defined(RADIO_CC2420)
   #include "../platform/micaz/hplcc2420interruptm.h"
 #endif
 
 #ifdef PLATFORM_AVR
   #include "../platform/avr/timerraw.h"
 #endif
 
//Implementing platform related modules 

inline  
result_t GenericTimerInit(void)
{
	 #ifdef PLATFORM_AVR
	  return TimerM_StdControl_init();
	 #endif
	
}

inline  
result_t GenericTimerStart(uint8_t id, char type, uint32_t interval)
{
	
   #ifdef PLATFORM_AVR
    return 	TimerM_Timer_start(id, type, interval); 
   #endif
		
}


//This function is called from the particular implementation!
//This function also contains platform related defintions 
//0, 1, 2, AND 10, 11 are reserved. User is suggested to start with id 20. 


inline result_t GenericTimerFired(uint8_t id)
{
  unsigned char result;
  //0, 1, 2 for threads
  
  switch (id) {

    case 0:
      ServiceTimerFired(0);
      break;

    case 1:
	  ServiceTimerFired(1);
	  break;
  
    case 2:
	  ServiceTimerFired(2);
	  break;
	  
	  case 3:
	  ServiceTimerFired(3);
	  break;
	  
	  case 4:
	  ServiceTimerFired(4);
	  break;
	  
	  
	  case 5:
	  ServiceTimerFired(5);
	  break;
	  
	  
	  case 6:
	  ServiceTimerFired(6);
	  break;
	  
	  case 7:
	  ServiceTimerFired(7);
	  break;

	  case 9:
	  ServiceTimerFired(9);
	  break;
    
   
   #if defined(PLATFORM_AVR) && defined(RADIO_CC2420)
    case 10:
      hplcc2420interruptm_FIFOTimer_fired();
	  break;
	  
    case 11:
      hplcc2420interruptm_CCATimer_fired();
	  break;
   #endif 
   
   default:
      result = SUCCESS; 
     }
  return SUCCESS;	 
}

