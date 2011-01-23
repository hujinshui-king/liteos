#include "Socket.h"
#include "thread_data.h"
#include "scheduling.h"
#include "threads.h"
#include "commondefines.h"
#include "avrhardware.h"

extern volatile thread* current_thread; 
extern volatile uint8_t thread_task_active ;
extern thread thread_table[TOSH_MAX_THREADS];

volatile mutex msend; 
volatile mutex *m_unlock;

static uint8_t pending_id;
static uint16_t pending_address;
static uint8_t pending_length;
static uint8_t *pending_msg; 

Radio_Msg currentMsg; 




struct msgData 
  { 
    uint8_t data[16]; 
  };


int SocketInit()
{
   msend.lock = msend.waiting = 0; 
   return 0;
}



void send_taskdebug(){
    result_t retval;
    int i; 
    struct msgData * pack; 
	char *p; 

	pack = (struct msgData *)currentMsg.data; 
    p  = (char *)pack; 

    pending_length = 10; 
 
      
    //mystrncpy((char *)pack, (char*)pending_msg, pending_length); 
	for (i=0;i<10;i++)
	{
	    *p = 'a' +i; 
		p++; 
	  
	}

    retval = AMStandard_SendMsg_send(0, 0xffff, 10, &currentMsg); 
    //if(retval != SUCCESS)  postTask(send_task, 5);
}




void send_task(){
    result_t retval;

    struct msgData * pack; 

	pack = (struct msgData *)currentMsg.data; 



	if (pending_length == 0)
	  pending_length = strlen((char *)pending_msg);
 
      
    mystrncpy((char *)pack, (char*)pending_msg, pending_length); 

    retval = AMStandard_SendMsg_send(pending_id, pending_address, pending_length, &currentMsg); 
    //if(retval != SUCCESS)  postTask(send_task, 5);
}


void SocketRadioSend()
 {
 
 
   pending_id = current_thread->data.radiostate.id; 
   pending_address = current_thread->data.radiostate.address; 
   pending_length =  current_thread->data.radiostate.length;
   pending_msg = current_thread->data.radiostate.msg;

   postTask(send_task, 5);

   
    //After it is woken up, return
   return;
}



void SocketSendDone(uint8_t id, Radio_MsgPtr msg, result_t success)
{
 
 //change the barrier
 // Barrier_unblock(6, id);
  return; 
}

void SocketRadioReceive(uint16_t port, Radio_MsgPtr msg)
{}


/*
void SocketRadioReceive(uint16_t port, Radio_MsgPtr msg)
{
     
    uint8_t i;
      _atomic_t currentatomic;

	//patch
	port = 5; 

    currentatomic = _atomic_start();
    
    
      for(i=0;i<TOSH_MAX_THREADS;i++){
        //Look for a thread waiting on this IO
        if((thread_table[i].state == STATE_IO) && 
            (thread_table[i].data.iostate.type == 6) &&
            (thread_table[i].data.iostate.id == id)){
          //Mark that thread as active
          thread_table[i].state = STATE_ACTIVE;
          mystrncpy((char *)thread_table[i].data.iostate.msg, (char *)msg, sizeof(Radio_Msg));

          if (thread_task_active == 0)
			    postTask(thread_task, 2);
          break; 
        }
      }
    
    _atomic_end(currentatomic);

}

*/


