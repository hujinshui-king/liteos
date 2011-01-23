#include "avrhardware.h"
#include "micazhardware.h"
#include "Types.h"
#include "threads.h"
#include "scheduling.h"
#include "TimerRaw.h"
#include "amradio.h"
#include "Leds.h"
#include "Socket.h"
#include "commondefines.h"
#include "commandHandle.h"
#include "PacketHandler.h"
#include "Main.h"
#include "CC2420ControlM.h"


extern volatile uint8_t currentuse; 


extern void Task1();

extern char networkid[6]; 
extern char filenameid[11]; 
extern uint8_t nodeid; 
//nst char *programstring = "0C94C6640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E3640C94E36411241FBEC0E0D7E0DEBFCDBF11E0A0E0B1E0EEEDFAEC00E00BBF02C007900D92A030B107D9F715E0A0E0B5E001C01D92A630B107E1F70C94E5640C948064C0E0D7E0DEBFCDBF88EE93E00E940A650E945565F9CFCF93DF93CDB7DEB722970FB6F894DEBF0FBECDBF80E09AEE9A838983E981FA81099522960FB6F894DEBF0FBECDBFDF91CF910895CF93DF93CDB7DEB722970FB6F894DEBF0FBECDBF9A8389838BE494E09093030580930205E0910205F09103050190F081E02D84E08283E0910205F09103050190F081E02D89819A81968385830E94F06422960FB6F894DEBF0FBECDBFDF91CF910895CF93DF93CDB7DEB722970FB6F894DEBF0FBECDBF89E09AEE9A838983E981FA81099522960FB6F894DEBF0FBECDBFDF91CF910895CF93DF93CDB7DEB722970FB6F894DEBF0FBECDBF82E19AEE9A838983E981FA81099522960FB6F894DEBF0FBECDBFDF91CF910895";

//char *programstring ="400C8467500C9446C80C9463C80C9463C80C9463C80C9463C80C9463C80C9463C80C9463C80C9463C80C9463C80C9463C80C9463C80C9463C80C9463C80C9463C80C9463C80C9463C80C9463C80C9463C80C9463C80C9463C80C9463C80C9463C80C9463C80C9463C80C9463C80C9463C80C9463C80C9463C80C9463C80C9463C80C9463C80C9463C80C9463C80C9463C811241FBEC0E5D7E0DEBFCDBF17E0A0E0B7E0E4EFF1E901E00BBF02C007900D92A030B107D9F717E0A0E0B7E001C01D92A232B107E1F70C9487C80C9400C84F935F93ECE0FAEE0995842F952F4F915F910895E0E0FAEE099508950F931F938C010E9465C8DC01ED91FC91119784E08283ED91FC91168305830E946FC81F910F910895FF920F931F93CF93DF9381E090E0909321078093200786E490E00E9473C82091200730912107832F992780930007209301078EEE80930307809302074F935F93E0E1FAEE0995C42FD52F4F915F910E9465C88C010E9465C8DC01FFB6F8948881882389F0ED91FC91119783E08283ED91FC91D683C58389818F5F89830E946FC889818150898321E028838F2D99278FBFD801ED91FC9111972583ED91FC91119717821682ED91FC91119780E18087ED91FC9180E097E092878187E4E1FAEE09958EE190E00E9473C84F935F934C2F5D2FE8E1FAEE09954F915F918091200790912107019694CF";
char *programstring = "a"; 


extern uint8_t shellbuffer[128]; 
extern void ShellThread(); 


//start testing 

struct OscopeMsg
{
	uint16_t filter;
    uint16_t sourceMoteID;
    uint16_t lastSampleNumber;
    uint16_t channel;
    uint16_t data;
    uint8_t pad;
};



uint8_t packetReadingNumber;
uint16_t readingNumber;
Radio_Msg msg[2];

uint8_t currentMsg;
uint16_t lastreading[10];
uint16_t averagereading;
uint8_t nextindex;


 
#define STACK_TOP(stack)    \
  (&(((uint16_t *)stack)[(sizeof(stack) / sizeof(uint16_t)) - 1]))



result_t Blink_SendMsg_send(uint16_t address, uint8_t size, Radio_MsgPtr pointer){

  unsigned char result;



  result = AMStandard_SendMsg_send(0xfefe, address, size, pointer);



  return result;

}



 void dataTask() {
 	
    struct OscopeMsg *pack;
     { _atomic_t _atomic = _atomic_start();
    
     {pack = (struct OscopeMsg *)msg[currentMsg].data;
      packetReadingNumber = 0;
	  readingNumber++; 
      pack->lastSampleNumber = readingNumber;
     }
     _atomic_end(_atomic); }  

    pack->channel = 1;
	pack->filter = 1; 
    pack->sourceMoteID = CURRENT_NODE_ID;
    
    if (Blink_SendMsg_send(BCAST_ADDRESS, sizeof(struct OscopeMsg),
			      &msg[currentMsg]))
      {
	   { _atomic_t _atomic = _atomic_start();
	   	{
	    currentMsg ^= 0x1;
	   }
	     _atomic_end(_atomic); }  

	
	   Leds_redToggle();
      }
  }




void mytimer_fired(){

 
 postTask(dataTask,3);
//add
//PhotoTempM_ExternalPhotoADC_getData();
//PhotoTempM_ExternalPhotoADC_dataReady(6);

}


//add
result_t Blink_SendMsg_sendDone(Radio_MsgPtr msg, result_t success)
{
  return SUCCESS;
}



//add

Radio_MsgPtr Blink_ReceiveMsg_receive(Radio_MsgPtr msg)
{
  
  return msg;
}





int main(){


 TOSH_SET_PIN_DIRECTIONS();
   initScheduling();  
   thread_init();

  currentuse = 0; 
 
  printString("Start!\n");

  sei();



   mystrncpy(networkid, "sn03\0", 5);
   nodeid = 3;   
   mystrncpy(filenameid, "nodeA\0", 6); 
  


  


   TimerM_StdControl_init();

  



   AMStandard_Control_init();
   AMStandard_Control_start();
   
   SocketInit();
 




   currentMsg = 0;
   packetReadingNumber = 0;
   readingNumber = 0;
      
      
   nextindex = 0;
   
   
  
  CURRENT_NODE_ID = 17;

 
//add end
 //line 39
//Timer_start(2,REPEAT ,15000*MILLISECOND);

//TimerM_Timer_start(15, TIMER_REPEAT, 5000*MILLISECOND);

 //line 40
//Timer_start(1,REPEAT,1*MINUTE);


  initRadioHandle();

 
  CC2420ControlM_CC2420Control_TuneChannel(15);
  
  CC2420ControlM_CC2420Control_TunePower( 31) ; 


  __nesc_enable_interrupt();

  while (1) {
      runNextTask();
    }

	return 0;
}








