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


#ifndef LITEOSCOMMONH
#define LITEOSCOMMONH




#include "types.h"


#define CURRENTADDRESS 1

//yied function
#define YIELD_FUNCTION  												0xEA00

//green toggle function 
#define GREEN_LED_TOGGLE_FUNCTION 							0xEA04

//red toggle function 
#define RED_LED_TOGGLE_FUNCTION  								0xEA08

//get current thread address
#define GET_CURRENT_THREAD_ADDRESS_FUNCTION 		0xEA0C

//get the radio mutex address
#define GET_RADIO_MUTEX_ADDRESS_FUNCTION 				0xEA10

//send by radio 
#define SOCKET_RADIO_SEND_FUNCTION 							0xEA14

//unlock the mutex
#define MUTEX_UNLOCK_FUNCTION										0xEA18

//get the index of the current thread 
#define GET_CURRENT_THREAD_INDEX_FUNCTION			  0xEA1C

//get the file path parameter address
#define GET_FILE_PATH_ADDRESS										0xEA20

//get the file mode parameter address
#define GET_FILE_MODE_ADDRESS 									0xEA24

//get the file operation mutex address
#define GET_FILE_MUTEX_ADDRESS									0xEA28

//open a file
#define OPEN_FILE_SYSCALL 											0xEA2C

//close a file 
#define CLOSE_FILE_SYSCALL 											0xEA30

//read from a file 
#define READ_FILE_SYSCALL 											0xEA34

//write to a file 
#define WRITE_FILE_SYSCALL 											0xEA38

//seek between the file location 
#define SEEK_FILE_SYSCALL 											0xEA3C

//ADC interfaces 
#define ADC_READ_LIGHT 													0xEA40

#define ADC_READ_TEMP 													0xEA44

#define ADC_READ_MAGX 													0xEA48

#define ADC_READ_MAGY 													0xEA4C

#define ADC_READ_ACCX 													0xEA50

#define ADC_READ_ACCY 													0xEA54

#define POST_TASK_SYSCALL 											0xEA58

#define GET_CURRENT_RADIO_INFO_ADDR 						0xEA5C

//Handle is for receiving packets 

#define GET_CURRENT_RADIO_HANDLE_ADDR 					0xEA60

//Register the radio receive event 

#define REGISTER_RADIO_RECEIVE_EVENT 						0xEA64

//Post a new thread task to the kernel to continue thread scheduling 
#define POST_NEW_THREAD_TASK 										0xEA68

//Debug purposes only to view variable values 
#define DEBUG_VALUE_SYSCALL 										0xEA6C

#define YELLOW_TOGGLE_FUNCTION 									0xEA70

#define RED_ON_FUNCTION 												0xEA74

#define RED_OFF_FUNCTION 												0xEA78

#define YELLOW_ON_FUNCTION 											0xEA7C

#define YELLOW_OFF_FUNCTION 										0xEA80

#define GREEN_ON_FUNCTION 											0xEA84

#define GREEN_OFF_FUNCTION 											0xEA88

#define BREAK_POINT_FUNCTION 										0xEA8C

#define GET_SERIAL_MUTEX_ADDRESS_FUNCTION 			0xEA90

#define GET_SERIAL_SEND_STRUCTURE_ADDRESS				0xEA94

#define SERIAL_SEND_FUNCTION 										0xEA98

#define GET_SERIAL_RECEIVE_HANDLE 							0xEA9C
	
#define REGISTER_SERIAL_RECEIVE_EVENT 					0xEAA0

#define GET_EEPROM_STRUCTURE_HANDLE 						0xEAA4

#define READ_EEPROM_TASK 												0xEAA8

#define WRITE_EEPROM_TASK 											0xEAAC

#define MALLOC_MEMORY_FUNCTION 									0xEAB0

#define FREE_MEMORY_FUNCTION 										0xEAB4

#define RESTORE_RADIO_STATE 										0xEAB8

#define GET_RANDOM_NUMBER_FUNCTION 							0xEABC

#define SET_RADIO_FREQ_FUNCTION 								0xEAC0

#define SET_RADIO_CHANNEL_FUNCTION 							0xEAC4

#define SET_RADIO_POWER_FUNCTION 								0xEAC8

#define GET_NODE_ID_FUNCTION 										0xEACC

#define SET_NODE_ID_FUNCTION 										0xEAD0

#define GET_THREAD_CONTROL_BLOCK 								0xEAD4

#define GET_THREAD_CONTROL_MUTEX 								0xEAD8

#define CREATE_THERAD_SYSCALL   								0xEADC

//No logging for the remaining
	
#define GET_LOGGER_BLOCK_ADDRESS  							0xEAE0

#define ENABLE_TRACING_SYSCALL									0xEAE4

#define DISABLE_TRACING_SYSCALL									0xEAE8

#define POST_TASK_WITHOUT_LOGGING								0xEAEC

#define POST_NEW_THREAD_TASK_NOLOG							0xEAF0

//This is for the purpose of jumping to the user trace handler 
//The prelimnary trace hanlder is included in the current version for use

#define SYSCALL_TRACE_POINT_USER								0xEAF4

#define SYSCALL_USER_ADD_TRACE_POINT						0xEAF8

#define SYSCALL_USER_ADD_TRACE_POINT_LONG				0xEAFC

#define SYSCALL_USER_THREAD_TERMINATE_TASK			0xEB00

#define SYSCALL_TIMER_FIRE_TASK	 								0xEB04

#define GET_KERNEL_STACK_PTR_ADDRESS						0xEB08

#define SYSCALL_USER_REMOVE_TRACE_POINT         0xEB0C

#define SYSCALL_USER_REMOVE_TRACE_POINT_LONG    0xEB10
	
#define SOUNDER_ON_FUNCTION									0xEB14

#define SOUNDER_OFF_FUNCTION									0xEB18
	

typedef struct mutex{
  volatile uint8_t lock;
  volatile uint8_t waiting;
  volatile uint8_t lockingthreadid; 
}mutex;

typedef struct {
 uint16_t socket_port;
 uint16_t socket_addr;
 uint8_t socket_msg_len;
 uint8_t *socket_msg; 
} radioinfotype; 

typedef struct 
{
  uint8_t socket_msg_len;
	uint8_t *socket_msg;
} serialinfotype; 


typedef struct {
  uint16_t addr;
  uint16_t nBytes;
  uint8_t *buffer; 
} genericByteStorageTaskNode; 


typedef struct {
  uint16_t port;
  uint8_t maxLength; 
  uint8_t *dataReady; 
  uint8_t *data; 
  uint8_t *packetinfo; 
  uint8_t handlevalid; 
  void (*handlefunc) (void);
  
}radiohandletype;


typedef struct {
  uint16_t port;
  uint8_t maxLength; 
  uint8_t *data; 
  uint8_t handlevalid; 
  void (*handlefunc) (void);
  
}serialhandletype;



//This is a thread datastructure
typedef struct thread{
  volatile uint16_t * sp;
  volatile uint8_t state;
 
  uint8_t priority;
  volatile uint8_t remaincredits; 

  uint8_t threadName[12];
  
  uint16_t *ramstart;
  uint16_t *ramend; 
  uint16_t sizeofBss; 
  
   uint16_t romstart; 
   uint16_t romsize; 
  
  volatile union{
    void (*tp) ();
   
    mutex *m;
    
    struct {	 
	  uint16_t sleeptime;
	  uint8_t sleeptype;
	  }sleepstate;

   /* struct {
	  uint8_t id; 
	  uint16_t address;
	  uint8_t length;
	  uint8_t *msg;
	 }radiostate; 
   */
   	     
    struct {
      uint8_t type, id;
	  uint8_t *msg; 
    }iostate;

	struct {
	 int adcreading;
	 }adcstate; 

   }data;


  volatile union{
   	struct {
	  uint8_t *fileptr;
	  uint8_t *bufferptr; 
	  uint16_t bytes;
	}filestate;
	 
	struct {
	  uint8_t *fileptr;
	  int offset; 
	  int position; 
	}fileseekstate;

   }filedata; 


}thread;


typedef struct 
{
  uint8_t valid;
  uint8_t index;
  uint8_t addr;
  uint8_t mode;
  uint16_t size; 
  int32_t fpos;
} MYFILE;


enum {
  STATE_NULL = 0,     //There is no thread here
  STATE_READY = 1,    //This thread has been created but never executed before
  STATE_ACTIVE = 2,   //There is a thread here, not blocked or sleeping
  STATE_BLOCKED = 3,  //This thread is blocked on a mutex
  STATE_PRESLEEP = 4,    //This thread is about to sleep
  STATE_SLEEP = 5,
  STATE_IO = 6,        //This thread is blocked until I/O Completion
  STATE_FILE = 7,
  STATE_BREAK = 8
};


typedef struct thread_create_block
{
  void (*fp)();
  uint8_t* ram_start;
  uint8_t *stack_ptr;
  uint8_t priority;
  const char *threadname; 
} thread_create_block_type;




#endif 
