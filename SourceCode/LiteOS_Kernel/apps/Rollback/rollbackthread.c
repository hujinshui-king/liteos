#include "../../libraries/libleds.h"
#include "../../libraries/libthread.h"
#include "../../libraries/libsystem.h"
#include "../../libraries/libserial.h"
#include "../../libraries/liteoscommon.h"
#include "../../libraries/libradio.h"
#include "../../libraries/libstring.h"
#include "../../libraries/libfile.h"
#include "../../libraries/libeeprom.h"
#include "../../libraries/libadc.h"
 
#include "../../types/types.h"

uint8_t snapshotbuffer[256];  

static uint8_t incomingRequest[32];
static uint8_t incomingRequestLength;
static uint8_t incomingRequestPacketInfo[4]; 
 
lib_thread **thiscurrentthread;
lib_thread *currentthread;

lib_thread *thread_table_start;

static uint8_t iswakenup; 


uint16_t seq;
uint8_t requestType;
uint8_t indexThread; 

LIB_MYFILE *fp;
char filename[8];
uint16_t seqtable[8]; 

uint8_t threadsize;
uint16_t threadramsize;
uint8_t *internal_ram_start;
    



void initSnapshotTable(){
  int i;
  for (i=0; i<8; i++)
    seqtable[i] = 0; 
}


void insertTable(uint16_t seq)
{
   int i;
   for (i=0;i<8;i++)
   {  if (seqtable[i] == 0)
	  break;
   }
     seqtable[i] = seq; 
}


int deleteTable(uint16_t seq)
{
  int i;
  for (i=0; i<8; i++)
   {
     if (seqtable[i] == seq)
	 {
	   seqtable[i] = 0; 
	   return i; 
     }
   }
   return 100; 
}

int lookupTable(uint16_t seq) { 
  int i;
  for (i=0; i<8; i++)
   {
     if (seqtable[i] == seq)
	   return i; 
   }
   return 100; 
}


void wakeupMeOnRequest()
{
	  currentthread->state = STATE_ACTIVE;
	  iswakenup = 1; 
    syscall_post_thread_task();
}




void registerHandleRequest()
{
    _atomic_t currentatomic;
    radiohandletype *radiohandleaddr;
    void (*getaddrfp) (void) = (void (*)(void))REGISTER_RADIO_RECEIVE_EVENT;

    radiohandleaddr = lib_get_current_radio_receive_handle_addr();
    incomingRequestLength = 0;
    //set up the radiohandleaddr data structures
    radiohandleaddr->port = 65;
    radiohandleaddr->maxLength = 16;
    radiohandleaddr->dataReady = &incomingRequestLength;
    radiohandleaddr->data = incomingRequest;
    radiohandleaddr->packetinfo = incomingRequestPacketInfo;
    radiohandleaddr->handlefunc = wakeupMeOnRequest;
    //close the interrupt     
    currentatomic = _atomic_start();
    //call the radio handle set to store the data structure into the handle vectors 
    getaddrfp();
    //open the interrupt 
    _atomic_end(currentatomic);
    return;
}



void initFileName()
{
	filename[0] = '_';
	filename[1] = 'f';
	filename[2] = 'i';
	filename[3] = 'l';
	filename[4] = 'e';
	filename[6] = '\0'; 
}


void snapshotmain()
{
	 

     
    initSnapshotTable();
    initFileName();
    
    thiscurrentthread = lib_get_current_thread();
    currentthread = *thiscurrentthread;
    
    registerHandleRequest();
    
    while (1)
    {
         if (iswakenup == 1)
         	{
         		iswakenup = 0;
            seq = *(uint16_t *)(&incomingRequest[0]);
            requestType = incomingRequest[2];
            indexThread = incomingRequest[3];
            
            if (requestType == 1)
            	{
            		thread_table_start = lib_get_thread_table_start();
					insertTable(seq); 
            		filename[5] = '1' +  lookupTable(seq); 
               
                threadsize = sizeof(struct lib_thread);
                threadramsize =
                (uint16_t) ((uint8_t *) thread_table_start[indexThread].ramend -
                (uint8_t *) thread_table_start[indexThread].ramstart + 1);
                internal_ram_start = 
                (uint8_t *) thread_table_start[indexThread].ramstart;
               fp = lib_mfopen((char *)filename, "w");
               lib_mfwrite(fp, &thread_table_start[indexThread], threadsize);
               lib_mfseek(fp, threadsize, 1);
               lib_mfwrite(fp, internal_ram_start, threadramsize);
               lib_mfclose(fp);
               
               thread_table_start[indexThread].state =  STATE_ACTIVE; 
              }  
               else if (requestType == 2)
            	  {
            	  thread_table_start = lib_get_thread_table_start();
            		filename[5] = '1' +  lookupTable(seq); 
               
                threadsize = sizeof(struct lib_thread);
                threadramsize =
                 (uint16_t) ((uint8_t *) thread_table_start[indexThread].ramend -
                    (uint8_t *) thread_table_start[indexThread].ramstart + 1);
                internal_ram_start = 
                (uint8_t *) thread_table_start[indexThread].ramstart;
               fp = lib_mfopen((char *)filename, "r");
               lib_mfread(fp, &thread_table_start[indexThread], threadsize);
               lib_mfseek(fp, threadsize, 1);
               lib_mfread(fp, internal_ram_start, threadramsize);
               lib_mfclose(fp);	

			   thread_table_start[indexThread].state =  STATE_ACTIVE; 
            	  	
            	  	
            	  }
            	  	
            		else if (requestType == 3)
            		  {}	         		
         		
         	}
         	
         else
         	lib_hibernate_thread();
    }  
}   
