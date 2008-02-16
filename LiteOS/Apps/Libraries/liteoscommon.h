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
#define YIELDFUNCTION  0xEA00

//green toggle function 
#define GREENTOGGLEFUNCTION 0xEA04

//red toggle function 
#define REDTOGGLEFUNCTION  0xEA08

//get current thread address
#define CURRENTTHREADFUNCTION 0xEA0C

//get the radio mutex address
#define GETRADIOMUTEXFUNCTION 0xEA10


//send by radio 
#define RADIOSENDFUNCTION 0xEA14

//unlock the mutex
#define MUTEXUNLOCKFUNCTION 0xEA18

//get the index of the current thread 
#define CURRENTTHREADINDEXFUNCTION 0xEA1C

//get the file path parameter address
#define GETFILEPATHADDRESS 0xEA20

//get the file mode parameter address
#define GETFILEMODEADDRESS 0xEA24

//get the file operation mutex address
#define GETFILEMUTEXADDRESS 0xEA28

//open a file
#define OPENFILESYSCALL 0xEA2C

//close a file 
#define CLOSEFILESYSCALL 0xEA30

//read from a file 
#define READFILESYSCALL 0xEA34

//write to a file 
#define WRITEFILESYSCALL 0xEA38

//seek between the file location 
#define SEEKFILESYSCALL 0xEA3C

//ADC interfaces 
#define ADCLIGHT 0xEA40

#define ADCTEMP 0xEA44

#define ADCMAGX 0xEA48

#define ADCMAGY 0xEA4C

#define ADCACCX 0xEA50

#define ADCACCY 0xEA54

#define POSTTASKSYSCALL 0xEA58

#define GETCURRENTRADIOINFO 0xEA5C

#define GETCURRENTRADIOHANDLE 0xEA60

#define SETCURRENTRADIOHANDLE 0xEA64

#define POSTTHREADTASK 0xEA68

#define DEBUGSYSCALL 0xEA6C

#define YELLOWTOGGLEFUNCTION 0xEA70

#define REDONFUNCTION 0xEA74

#define REDOFFFUNCTION 0xEA78

#define YELLOWONFUNCTION 0xEA7C

#define YELLOWOFFFUNCTION 0xEA80

#define GREENONFUNCTION 0xEA84

#define GREENOFFFUNCTION 0xEA88

#define BREAKFUNCTION 0xEA8C

#define GETSERIALMUTEXFUNCTION 0xEA90

#define GETCURRENTSERIALINFO 0xEA94

#define SERIALSENDFUNCTION 0xEA98

#define GETCURRENTSERIALHANDLE 0xEA9C

#define SETCURRENTSERIALHANDLE 0xEAA0

#define GETCURRENTEEPROMINFOHANDLE 0xEAA4

#define READFROMEEPROM 0xEAA8

#define WRITETOEEPROM 0xEAAC

#define MALLOCHANDLE 0xEAB0

#define FREEMALLOCHANDLE 0xEAB4

#define DISABLERADIOSTATE 0xEAB8

#define GETRANDOM16FUNCTION 0xEABC

#define SETRADIOFREQFUNCTION 0xEAC0

#define SETRADIOCHANNELFUNCTION 0xEAC4

#define SETRADIOPOWERFUNCTION 0xEAC8

#define GETNODEIDFUNCTION 0xEACC

#define SETNODEIDFUNCTION 0xEAD0



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



#endif 
