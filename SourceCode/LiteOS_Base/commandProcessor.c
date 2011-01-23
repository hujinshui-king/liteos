#include "commandProcessor.h"
#include "amradio.h"
#include "avrhardware.h"
#include "testUSART2.h"
#include "scheduling.h"
#include "CC2420ControlM.h"

extern bool AMStandard_state; 
extern uint8_t CC2420RadioM_stateRadio;


Radio_Msg broadcastmsg;
Radio_Msg backupmsg[2]; 

volatile uint8_t currentlength; 
volatile uint8_t currentuse; 


uint8_t dataToSend[128]; 


//like 6, a, b, c, d, e and then this is the message sent out using port fefe with braodcast 
void broadcastCommand(uint8_t *receivebuffer, uint8_t length)
{

     uint8_t *pack;
	 uint8_t reallength = receivebuffer[0]; 
	 uint8_t i; 
    
     AMStandard_state = FALSE; 
     CC2420RadioM_stateRadio = 2; 

     if (receivebuffer[1] == 241)
	 {
	 
	  AMStandard_Restart();
	  return; 
	 
	 }

	 if (receivebuffer[1] == 2)
	  {
	   CC2420ControlM_CC2420Control_TuneChannel(receivebuffer[2]); 
	   return;   
	  
	  }



     { _atomic_t _atomic = _atomic_start();
    
     {
	  pack = (uint8_t *)broadcastmsg.data;

      for (i=0;i<reallength; i++)
	    pack[i] = receivebuffer[i]; 
     }
     _atomic_end(_atomic); }  

    broadcastmsg.port = 0xfefe; 
     
    AMStandard_SendMsg_send(0xfefe, 0xffff, reallength, &broadcastmsg); 
	
	Broadcast2SerialAlternative(&broadcastmsg); 
   
    return; 

}




void senddata(uint8_t length)
{
    printStringN(dataToSend, length); 

}



//deprecated 
Radio_MsgPtr Broadcast2Serial(Radio_MsgPtr msg)
{

   
   uint8_t *pack; 
   uint8_t i; 

{ _atomic_t _atomic = _atomic_start();
    


   pack = (uint8_t*)(msg->data); 

   currentlength = pack[0]; 

   for (i=0;i<currentlength;i++)
     dataToSend[i] = pack[i]; 
  
  _atomic_end(_atomic); }  
  
  //postTask(senddata, (uint8_t)6); 
  
  return msg;      
  


}


Radio_MsgPtr Broadcast2SerialAlternative(Radio_MsgPtr msg)
{



   uint8_t *pack; 
   uint8_t i; 

   uint8_t currentindex; 
{ _atomic_t _atomic = _atomic_start();
    


   pack = (uint8_t*)(msg); 

   currentlength = pack[0]; 

  
   currentindex = 0;

   dataToSend[currentindex++] = 0x7e;
   dataToSend[currentindex++] = 0x42;
  
     

   for (i=6;i<4+6;i++)
   {
   
   
     uint8_t temp = pack[i]; 
     if (temp == 0x7e)
      {dataToSend[currentindex++] = 0x7d;dataToSend[currentindex++] = 0x5e; }
     else  if (temp == 0x7d)
      {dataToSend[currentindex++] = 0x7d;dataToSend[currentindex++] = 0x5d; }
	  else 
	   dataToSend[currentindex++] = temp; 

   

   }

   dataToSend[currentindex++] = currentlength;


   for (i=4+6;i<currentlength+4+6;i++)
   {
   
   
     uint8_t temp = pack[i]; 
     if (temp == 0x7e)
      {dataToSend[currentindex++] = 0x7d;dataToSend[currentindex++] = 0x5e; }
     else  if (temp == 0x7d)
      {dataToSend[currentindex++] = 0x7d;dataToSend[currentindex++] = 0x5d; }
	  else 
	   dataToSend[currentindex++] = temp; 

   

   }
  

  
  dataToSend[currentindex++] = 0;
  dataToSend[currentindex++] = 0;
  dataToSend[currentindex++] = 0x7e; 
   _atomic_end(_atomic); }  
  

  senddata(currentindex); 
  
  return msg;      
  



}
