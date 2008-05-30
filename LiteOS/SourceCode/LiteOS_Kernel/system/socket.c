/* LiteOS Version 0.3 */
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

#include "socket.h"
#include "threaddata.h"
#include "scheduling.h"
#include "threads.h"
#include "amradio.h"
#include "amcommon.h"
#include "../types/string.h"
#include "../types/byteorder.h"
#include "packethandler.h"
#include "stdserial.h"
#include "../filesys/filesocket.h"


#ifdef SERIAL_COMMAND_REPLY
#include "../io/avr_serial/serialprint.h"
#endif
extern uint8_t cc2420radiom_stateRadio;



//defined as global variables so that these variables are visible to the rest components 

//These are for external usage, like system calls 
//for radio used by user threads
volatile mutex msend;

//for serial used by user threads
volatile mutex mserialsend;

//radio mutex used by kernel to implement radio 
volatile mutex radio_mutex;


Radio_Msg currentMsg;



//In this part, socket_port, socket_addr specifies the next hop node information 
//msg_len specifices the message length to be broadcast over the air
//msg is the content of the data message to be broadcast 
//socket_send_action means the actual action of sending the packet is performed by the socket 



/** \ingroup radio */

radioinfotype radioinfo;

/** \ingroup serial */
serialinfotype serialinfo;


struct msgData {
   uint8_t data[ 16 ];
};


//Init function. Must be called by main()

int SocketInit() {
   msend.lock = msend.waiting = msend.lockingthreadid = 0;
   mserialsend.lock = mserialsend.waiting = mserialsend.lockingthreadid = 0;
   radio_mutex.lock = radio_mutex.waiting = radio_mutex.lockingthreadid = 0;

   
   return 0;
}




//-------------------------------------------------------------------------
//If the destination is another node, send the packet through radio. 
//If the destination is the current node, then send the packet locally by calling Standard_Receive_Packet 

static void send_task() {
	
   struct msgData *dataPayloadPtr_currentMsg;
  
   dataPayloadPtr_currentMsg = ( struct msgData* )currentMsg.data;
  
   if ( radioinfo.socket_msg_len == 0 ) {
      radioinfo.socket_msg_len = mystrlen(( char* )radioinfo.socket_msg );
   } 
   
   mystrncpy(( char* )dataPayloadPtr_currentMsg, ( char* )radioinfo.socket_msg, radioinfo.socket_msg_len );
   

  #if defined(PLATFORM_AVR_IRIS)
  {
  
      currentMsg.length = radioinfo.socket_msg_len;
      currentMsg.addr = radioinfo.socket_addr;
      currentMsg.port = radioinfo.socket_port;
	  Broadcast2SerialAlternative(&currentMsg); 

  
  
  
  
  }
  #elif defined(PLATFORM_AVR)

   if ( radioinfo.socket_addr != 0 ) {
      AMStandard_SendMsg_send( radioinfo.socket_port, radioinfo.socket_addr, radioinfo.socket_msg_len, &currentMsg );
   }
    
   else {
      currentMsg.length = radioinfo.socket_msg_len;
      currentMsg.addr = radioinfo.socket_addr;
      currentMsg.port = radioinfo.socket_port;
      Standard_Receive_Packet( radioinfo.socket_port, &currentMsg );
   } 
  #endif
}




//-------------------------------------------------------------------------
void SocketRadioSend() {
   postTask( send_task, 9 );
   //After it is woken up, return
   return ;
}



//-------------------------------------------------------------------------
void StandardSocketSend( uint16_t port, uint16_t address, uint8_t msglength, uint8_t *msg ) {
	
   Socket_Mutex_lock( &radio_mutex );
   radioinfo.socket_port = port;
   radioinfo.socket_addr = address;
   radioinfo.socket_msg_len = msglength;
   radioinfo.socket_msg = msg;
   
   SocketRadioSend();
   
   sleepThread( 40 );
   
   restoreRadioState(); 
   
   Socket_Mutex_unlock( &radio_mutex );
   return ;
}




//-------------------------------------------------------------------------
//Return the radio info address by putting it into the registers r20, r21

//void getRadioInfo()  __attribute__((naked));

void* getRadioInfo() {
   return  (void *)&radioinfo;
   //asm volatile( "mov r20, %A0""\n\t""mov r21, %B0""\n\t": : "r"( addr ) );
   // asm volatile("ret"::); 
}

//Return the serial info address. By populating this address and call socketserialsend, the data will be printed out. 

//void getRadioInfo()  __attribute__((naked));

void* getSerialInfo() {
   return (void *)&serialinfo;
   //asm volatile( "mov r20, %A0""\n\t""mov r21, %B0""\n\t": : "r"( addr ) );
   // asm volatile("ret"::); 
}


//-------------------------------------------------------------------------
void serial_send_task() {
   printfstrlen(( char* )serialinfo.socket_msg, serialinfo.socket_msg_len );
}



//-------------------------------------------------------------------------
void SocketSerialSend() {
   postTask( serial_send_task, 9 );
   //After it is woken up, return
   return ;
}




//-------------------------------------------------------------------------
void Socket_Mutex_lock( volatile mutex *m ) {
   _atomic_t currentatomic;
   currentatomic = _atomic_start();
   if ( m->lock ) {
      current_thread->state = STATE_BLOCKED;
      current_thread->data.m = m;
      //Notify others we are waiting
      m->waiting ++;
      //Switch back to scheduler
      thread_yield();
      //Remove notification
      m->waiting --;
   }
   m->lock = 1;
   _atomic_end( currentatomic );
}

//-------------------------------------------------------------------------
void Socket_Mutex_unlock( volatile mutex *m ) {
   _atomic_t currentatomic;
   currentatomic = _atomic_start();
   if ( m->lock ) {
      if ( m->waiting ) {
         int i;
         //Go wake up the next thread in line
         for ( i = 0; i <= LITE_MAX_THREADS; i ++ ) {
            //here I removed the threadidx
            int idx = i;
            //Look for the next thread waiting on this mutex
            if (( thread_table[ idx ].state == STATE_BLOCKED ) && ( thread_table[ idx ].data.m == m )) {
               //Mark that thread as active
               thread_table[ idx ].state = STATE_ACTIVE;
               postNewThreadTask(); 
               //Let next task run (we are active, but will thread_yield anyways
               thread_yield();
               break;
            }
         }
      } else {
         m->lock = 0;
      } //m-waiting
   } //m->lock
   _atomic_end( currentatomic );
}


/*
* mutex_unlock
 * This routine unlocks a mutex, and will yield the current thread
 * if there are any other threads waiting on this mutex
 * This routine is safe to call inside or outside of a thread
 */
void Mutex_unlock( volatile mutex *m ) {
   _atomic_t currentatomic;
   currentatomic = _atomic_start();
   if ( m->lock ) {
      if ( m->waiting ) {
         int i;
         //Go wake up the next thread in line
         for ( i = 0; i < LITE_MAX_THREADS; i ++ ) {
            //here I removed the threadidx
            int idx = i;
            //Look for the next thread waiting on this mutex
            if (( thread_table[ idx ].state == STATE_BLOCKED ) && ( thread_table[ idx ].data.m == m )) {
               //Mark that thread as active
               thread_table[ idx ].state = STATE_ACTIVE;
               postNewThreadTask();
               //Let next task run (we are active, but will yield anyways
               thread_yield();
               break;
            }
         }
      } else {
         m->lock = 0;
      } //m-waiting
   } //m->lock
   _atomic_end( currentatomic );
}


//void getRadioMutexAddress()  __attribute__((naked)) ;
void* getRadioMutexAddress() {
   volatile mutex *msendaddr = &msend;
   //asm volatile( "mov r20, %A0""\n\t""mov r21, %B0""\n\t": : "r"( msendaddr ) );
   return (void *)msendaddr; 
   //asm volatile("ret"::); 
}

//-------------------------------------------------------------------------
void* getSerialMutexAddress() {
   volatile mutex *msendaddr = &mserialsend;
   //asm volatile( "mov r20, %A0""\n\t""mov r21, %B0""\n\t": : "r"( msendaddr ) );
   return (void *)msendaddr; 
   //asm volatile("ret"::); 
}

void releaseMutexLockUponThreadKill(uint8_t index){}

/*

void releaseMutexLockUponThreadKill(uint8_t index) {


   _atomic_t currentatomic;
   volatile mutex *maddr; 

   //use index+1 to differentiate from 0 
   //thread table starts with 0, therefore, 1, 2, 3, etc 
   
   index++; 
   
   maddr = &msend; 
   
   if (maddr->lockingthreadid == index)
   	{
   	
     currentatomic = _atomic_start();
   
    if ( maddr->lock ) {
      if ( maddr->waiting ) {
         int i;
         //Go wake up the next thread in line
         for ( i = 0; i < LITE_MAX_THREADS; i ++ ) {
            //here I removed the threadidx
            int idx = i;
            //Look for the next thread waiting on this mutex
            if (( thread_table[ idx ].state == STATE_BLOCKED ) && ( thread_table[ idx ].data.m == maddr )) {
               //Mark that thread as active
               thread_table[ idx ].state = STATE_ACTIVE;
               postNewThreadTask();
               //Let next task run (we are active, but will yield anyways
             //  thread_yield();
               break;
            }
         	}
      } else {
         maddr->lock = 0;
		 maddr->lockingthreadid = 0; 
      } //m-waiting
   } //m->lock
   _atomic_end( currentatomic );
   	}

   maddr = &mserialsend; 
   
	
   if (msend.lockingthreadid == index)
   
   {
	   
		currentatomic = _atomic_start();
	  
	   if ( maddr->lock ) {
		 if ( maddr->waiting ) {
			int i;
			//Go wake up the next thread in line
			for ( i = 0; i < LITE_MAX_THREADS; i ++ ) {
			   //here I removed the threadidx
			   int idx = i;
			   //Look for the next thread waiting on this mutex
			   if (( thread_table[ idx ].state == STATE_BLOCKED ) && ( thread_table[ idx ].data.m == maddr )) {
				  //Mark that thread as active
				  thread_table[ idx ].state = STATE_ACTIVE;
				  postNewThreadTask();
				  //Let next task run (we are active, but will yield anyways
				//	thread_yield();
				  break;
			   }
			   }
		 } else {
			maddr->lock = 0;
			maddr->lockingthreadid = 0; 
		 } //m-waiting
	  } //m->lock
	  _atomic_end( currentatomic );
  }

  maddr = &filemutex; 
  {
   currentatomic = _atomic_start();
	 if ( maddr->lock ) {
		   if ( maddr->waiting ) {
			  int i;
			  //Go wake up the next thread in line
			  for ( i = 0; i < LITE_MAX_THREADS; i ++ ) {
				 //here I removed the threadidx
				 int idx = i;
				 //Look for the next thread waiting on this mutex
				 if (( thread_table[ idx ].state == STATE_BLOCKED ) && ( thread_table[ idx ].data.m == maddr )) {
					//Mark that thread as active
					thread_table[ idx ].state = STATE_ACTIVE;
					postNewThreadTask();
					//Let next task run (we are active, but will yield anyways
				  //  thread_yield();
					break;
				 }
				 }
		   } else {
			  maddr->lock = 0;
			  maddr->lockingthreadid = 0; 
		   } //m-waiting
		} //m->lock
		_atomic_end( currentatomic );
	}


}



*/

