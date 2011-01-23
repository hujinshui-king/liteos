#include "avrhardware.h"
#include "micazhardware.h"
#include "scheduling.h"
#include "Leds.h"
#include "amcommon.h"
#include "amradio.h"
#include "TimerRaw.h"
#include "byteorder.h"
#include "cc2420Const.h"
#include "CC2420ControlM.h"
#include "CC2420RadioM.h"
#include "HPLCC2420FIFOM.h"
#include "HPLCC2420InterruptM.h"
#include "HPLCC2420M.h"
#include "HPLTimer1.h"
#include "Types.h"
#include "Socket.h"
#include "PacketHandler.h"
#include "commandProcessor.h"




bool AMStandard_state;
Radio_MsgPtr AMStandard_buffer;
uint16_t AMStandard_lastCount;
uint16_t AMStandard_counter;



inline  
bool AMStandard_Control_init(void)
{

  result_t ok2;

  ok2 = CC2420RadioM_SplitControl_init();
    
  AMStandard_state = FALSE;
  AMStandard_lastCount = 0;
  AMStandard_counter = 0;

  return ok2;
}








inline   result_t AMStandard_RadioControl_start(void){

  unsigned char result;



  result = CC2420RadioM_StdControl_start();



  return result;

}


 inline  

bool AMStandard_Control_start(void)

{

  result_t ok2 = AMStandard_RadioControl_start();


  AMStandard_state = FALSE;


  return ok2; 
}





inline   result_t AMStandard_RadioSend_send(Radio_MsgPtr arg_0xa3c31f8){

  unsigned char result;



  result = CC2420RadioM_Send_send(arg_0xa3c31f8);



  return result;

}

 inline  

void AMStandard_sendTask(void)

{
  result_t ok;
  Radio_MsgPtr buf;


  buf = AMStandard_buffer;





  ok = AMStandard_RadioSend_send(buf);

  if (ok == FAIL) {
    AMStandard_reportSendDone(AMStandard_buffer, FAIL);
    }
}



//addr means the current broadcast address, et. id is the port 
//Send out a message and takes a while to complete 

result_t AMStandard_SendMsg_send(uint16_t port, uint16_t addr, uint8_t length, Radio_MsgPtr data)

{
  if (!AMStandard_state) {
      AMStandard_state = TRUE;
      if (length > DATA_LENGTH) {

          AMStandard_state = FALSE;
          return FAIL;
        }
      if (!postTask(AMStandard_sendTask, 3)) {
          {
          }

          ;
          AMStandard_state = FALSE;
          return FAIL;
        }
      else {

		//length is the first one that means the actual data length
		//adr is the next hop id
		//type is the port
		//group is manmade result 
		
          AMStandard_buffer = data;
          data->length = length;
          data->addr = addr;
          data->port = port;

        }
      return SUCCESS;
    }

  return FAIL;
}





 inline   

result_t AMStandard_SendMsg_default_sendDone(uint8_t id, Radio_MsgPtr msg, result_t success)

{
  return SUCCESS;
}





inline   result_t AMStandard_SendMsg_sendDone(uint16_t arg_0xa3b8f90, Radio_MsgPtr arg_0xa31a0a0, result_t arg_0xa31a1f0){

  unsigned char result;



  //switch (arg_0xa3b8f90) {

 //   case 9:
	//  SocketSendDone(9,arg_0xa31a0a0, 0);

  //  case 10:

   //   SocketSendDone(10,arg_0xa31a0a0, 0);
  //	  break;

      //break;

    //default:

      //result = AMStandard_SendMsg_default_sendDone(arg_0xa3b8f90, arg_0xa31a0a0, arg_0xa31a1f0);

    //}



  return result;

}

 inline   

result_t AMStandard_default_sendDone(void)

{
  return SUCCESS;
}


inline   result_t AMStandard_sendDone(void){

  unsigned char result;



  result = AMStandard_default_sendDone();



  return result;

}




 inline   

Radio_MsgPtr AMStandard_ReceiveMsg_default_receive(uint8_t id, Radio_MsgPtr msg)

{
  return msg;
}



//Another critical function that bridges to the application 
inline   Radio_MsgPtr AMStandard_ReceiveMsg_receive(uint16_t port, Radio_MsgPtr msg){

 return msg; 

}


   
   
 void AMStandard_Restart(void){

   
   
//   CC2420ControlM_CC2420Control_VREFOff();
//   AMStandard_Control_init();
//   AMStandard_Control_start();


   //TOSH_SET_PIN_DIRECTIONS();
   //AMStandard_Control_init();
   //AMStandard_Control_start();



//  CC2420ControlM_HPLChipcon_cmd(0x06);
  
   asm volatile("ldi	r30, 0x00"::);
   asm volatile("ldi	r31, 0x00"::);
   asm volatile("icall"::);


  //CC2420ControlM_CC2420Control_OscillatorOn();
  	
  return; 
}


   	



//This function is really really critical to the correct behaviro of the radio stack 

//Basically it returns a radio message pointer that must be reused 

//and the content of the packet is the correct packet parsed, and is useful 

//The content starts with a length that is the actual payload length and all information are correct

//Must copy this content to the user supplied buffer, put the user action into a task, and then return this buffer to the recevie module 


Radio_MsgPtr   received(Radio_MsgPtr packet)

{

  AMStandard_counter++;
 
 /* if (

  packet->crc == 1 && packet->port == 0xefef &&
  (
  packet->addr == BCAST_ADDRESS || 
  packet->addr == addr)) 
    {

      //uint16_t port = packet->port;
      Radio_MsgPtr tmp;

 
     //tmp = Standard_Receive_Packet(port, packet);
	  Leds_redToggle(); 
	  
      tmp = Broadcast2Serial(packet); 

     if (tmp) {
       packet = tmp;
     }
    }


else*/ if (packet->crc == 1)
 {
 
      //uint16_t port = packet->port;
      Radio_MsgPtr tmp;

 
     //tmp = Standard_Receive_Packet(port, packet);
	  Leds_redToggle(); 
	  
      tmp = Broadcast2SerialAlternative(packet); 

     if (tmp) {
       packet = tmp;
     }
 
 }

  return packet;
}                    

 inline  



Radio_MsgPtr AMStandard_RadioReceive_receive(Radio_MsgPtr packet)

{
  return received(packet);
}



 inline  

result_t AMStandard_RadioSend_sendDone(Radio_MsgPtr msg, result_t success)

{
  return AMStandard_reportSendDone(msg, success);
}



 

result_t AMStandard_reportSendDone(Radio_MsgPtr msg, result_t success)

{
  AMStandard_state = FALSE;
//  AMStandard_SendMsg_sendDone(msg->port, msg, success);
//  AMStandard_sendDone();

  return SUCCESS;
}




