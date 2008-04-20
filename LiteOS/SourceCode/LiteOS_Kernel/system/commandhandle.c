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

#include "commandhandle.h"
#include "../filesys/stdfsa.h"
#include "../system/stdserial.h"
#include "../filesys/fsconfig.h"
#include "../filesys/inode.h"
#include "../system/threads.h"
#include "../system/scheduling.h"
#include "../system/generictimer.h"
#include "../types/types.h"
#include "../system/packethandler.h"
#include "../system/socket.h"
#include "../filesys/fs_structure.h"
#include "../types/string.h"
#include "../system/amradio.h"
#include "../system/amcommon.h"
#include "../system/bytestorage.h"
#include "../system/pagestorage.h"
#include "../filesys/vectornode.h"
#include "../filesys/vectorflash.h"
#include "../system/nodeconfig.h"
#include "../utilities/math.h"


#ifdef PLATFORM_AVR
 #include "../platform/avr/bootloader.h"
 #include "../platform/avr/avrhardware.h"
 #include "../platform/micaz/micazhardware.h"
 #include <stdlib.h>
#endif
 

extern volatile uint16_t *old_stack_ptr;

extern void (*timercallback[8])(); 


uint8_t IncomingLength;

uint8_t IncomingMsg[ 64 ];

uint8_t PacketInfo[ 2 ];

uint16_t chunksequencenum, expectNextNum;

uint8_t createthreadname[ 12 ];

uint8_t shellbuffer[ 256 ];

volatile thread *thisthread;
void( *createtaskthread )( void );

uint16_t *createstackpointer;
uint16_t createstaticdatasize;
uint16_t *createramstart;

//the start location for writing the rom as user application
uint16_t createflashromstart;

//the size of written user application 
uint16_t createflashromsize; 


MYFILE *openedfile;
MYFILE *fp;
unsigned char reply[ 32 ];

//for reply 231
uint8_t blockid;
char filename[ 13 ];
uint16_t nodeid; 


static uint16_t kernelromsize = 41131;
static uint16_t kernelramsize = 2275; 

//-------------------------------------------------------------------------

void reply_devicehandling( uint8_t *receivebuffer ) {
   //the coming receive buffer is organized as follows
   //byte 0: the length
   //byte 1: the type 11
   //byte 2: the nodeid
   //byte 3: the block id
   //byte 4: the length of the name of the file dev
   //byte 5-- :
   //byte 10: the length of the parameters
   //parameters. for "" type, use " to decide. Otherwise it is 10 etc encoded
   //here it finds the block fo the dev, put the parameters into the buffer, call the task, and loop until the task finishes
   
   uint8_t newblockid, blockid;
   uint8_t *ramstart;
   uint16_t *ramreturn;
   uint8_t paramlength;
   uint8_t paramindex;
   uint8_t countparam;
   uint8_t i;
   void( *entryfunc )( void );
   uint8_t looproundcount;
   uint8_t loopcycle;
   uint16_t returnval;
   
   paramindex = 0;
   looproundcount = loopcycle = 0;
   countparam = 0;
   blockid = receivebuffer[ 3 ];
   
   mystrncpy( (char *)filename, ( char* )& receivebuffer[ 5 ], receivebuffer[ 4 ] );
   filename[ receivebuffer[ 4] ] = 0;
   newblockid = existBlockAddr( filename, ( int )blockid );
   //the total length of the param length, including the length field itself 
   paramlength = receivebuffer[ 5+receivebuffer[ 4] ];
   //the start position of the parameters
   paramindex = 5+receivebuffer[ 4 ] + 1;
   if ( newblockid == 0 ) {
      reply[ 0 ] = 3;
      reply[ 1 ] = 10;
      reply[ 2 ] = nodeid;
      StandardSocketSend( 0xefef, 0xffff, 32, reply );
      return ;
   }
   for ( i = 0; i < paramlength; i ++ ) {
      if ( receivebuffer[ paramindex + i ] != 34 ) {
         countparam ++;
      } else {
         break;
      } 
   }
   if ( countparam >= 1 ) {
      looproundcount = receivebuffer[ paramindex ];
   } 
   if ( countparam >= 2 ) {
      loopcycle = receivebuffer[ paramindex + 1 ];
   } 
   ramstart = ( uint8_t* )fsread16uint( newblockid, DEV_PROGRAMENTRYOFFSET + 2 );
   if ( paramlength > 2 ) {
      mystrncpy(( char* )ramstart, ( char* )& receivebuffer[ paramindex + countparam + 1 ], paramlength - countparam - 2 );
      ramstart[ paramlength - countparam - 1 ] = '\0';
   }
   ramreturn = ( uint16_t* )fsread16uint( newblockid, DEV_PROGRAMENTRYOFFSET + 4 );
   entryfunc = ( void( * )( void ))fsread16uint( newblockid, DEV_PROGRAMENTRYOFFSET );
   
   if ( looproundcount == 0 ) {
      looproundcount = 1;
   } 
   for ( i = 0; i < looproundcount; i ++ ) {
      entryfunc();
      returnval =  *ramreturn;
      reply[ 0 ] = 5;
      reply[ 1 ] = 12;
      if ( i == looproundcount - 1 ) {
         reply[ 1 ] = 11;
      } reply[ 2 ] = nodeid;
      reply[ 3 ] = returnval / 256;
      reply[ 4 ] = returnval % 256;
      StandardSocketSend( 0xefef, 0xffff, 32, reply );
      sleepThread( loopcycle );
   }
}


//This could be avr specific, but assuming that most devices have led one type or another. 

//-------------------------------------------------------------------------
void reply_led_control( uint8_t *receivebuffer ) {
   //the coming receive buffer is organized as follows
   //byte 0: the length
   //byte 1: the type 12
   //byte 2: the nodeid
   //byte 3: the block id
   //byte 4: red
   //byte 5: green
   //byte 6: yellow
   uint8_t newblockid, blockid;
   uint8_t *ramstart;
   void( *entryfunc )( void );
   blockid = receivebuffer[ 3 ];
   newblockid = existBlockAddr( "led", ( int )blockid );
   ramstart = ( uint8_t* )fsread16uint( newblockid, DEV_PROGRAMENTRYOFFSET + 2 );
   ramstart[ 0 ] = receivebuffer[ 4 ];
   ramstart[ 1 ] = receivebuffer[ 5 ];
   ramstart[ 2 ] = receivebuffer[ 6 ];
   entryfunc = ( void( * )( void ))fsread16uint( newblockid, DEV_PROGRAMENTRYOFFSET );
   entryfunc();
   return ;
}


//-------------------------------------------------------------------------
//Note that this function should consider splitting a message into multiple pieces, if the requested memory chunk is too large 

void reply_debugging_print( uint8_t *receivebuffer ) { //This is the reply for the print command 
   uint16_t addr;
   uint16_t size;
   uint8_t chunkindex;
   uint8_t numOfChunks; 
   
   addr = ( uint16_t )receivebuffer[ 3 ] *256+receivebuffer[ 4 ];
   size =  (uint16_t)receivebuffer[ 5 ] * 256 + receivebuffer[6];
   numOfChunks = size/24 + 1; 

   for (chunkindex = 0; chunkindex < numOfChunks; chunkindex++)   

   	{
    reply[ 0 ] = 24 + 6;
    reply[ 1 ] = 91;
    reply[ 2 ] = nodeid;
		reply[ 3 ] = chunkindex; 
		reply[ 4 ] = size/256;
		reply[ 5 ] = size%256;  
	
    //Note that this expose some piece of memory that is NOT part of the requested data 
    
    mystrncpy( (char *)&reply[ 6 ], ( char* )addr, 24 );
		addr = addr + 24; 
    StandardSocketSend( 0xefef, 0xffff, 32, reply );
   	}
	
   //printStringN(reply,32); 
}


//this is the reply for the set command 
void reply_debugging_set( uint8_t *receivebuffer )
 {
   uint16_t addr;
   uint8_t size;
   uint8_t i;
   addr = ( uint16_t )receivebuffer[ 3 ] *256+receivebuffer[ 4 ];
   size = receivebuffer[ 5 ];
   for ( i = 0; i < size; i ++ ) {
      *(( unsigned char* )( addr + i )) = receivebuffer[ 6+i ];
   }
   reply[ 0 ] = 3;
   reply[ 1 ] = 96;
   reply[ 2 ] = nodeid;
   //mystrncpy(&reply[3], (unsigned char *)addr, size); 
   StandardSocketSend( 0xefef, 0xffff, 32, reply );
   //printStringN(reply,32); 
}



//-------------------------------------------------------------------------
#ifdef PLATFORM_AVR
void reply_debugging_insert_avr_breakpoint( uint8_t *receivebuffer ) {
   uint16_t pagecount;
   uint8_t offset;
   uint8_t instructions[ 6 ];
   pagecount = ( uint16_t )receivebuffer[ 3 ] *256+receivebuffer[ 4 ];
   offset = receivebuffer[ 5 ];
   
   
   boot_insertBreakPoint( pagecount, offset, instructions );
   
   reply[ 0 ] = 12;
   reply[ 1 ] = 92;
   reply[ 2 ] = nodeid;
   reply[ 3 ] = receivebuffer[ 3 ];
   reply[ 4 ] = receivebuffer[ 4 ];
   reply[ 5 ] = receivebuffer[ 5 ];
   for ( offset = 0; offset < 6; offset ++ ) {
      reply[ 6+offset ] = instructions[ offset ];
   } 
   StandardSocketSend( 0xefef, 0xffff, 32, reply );
   StandardSocketSend( 0xefef, 0xffff, 32, reply );
   StandardSocketSend( 0xefef, 0xffff, 32, reply );
}
#endif


//this should copy the thread information from a file 
//to be written as 93, and use the name of the thread to continue its operations. 
//the shell should support operating on the files to directly find out the instructions 
//Using python or Perl should be OK as well 
//Support for breakpoints should be finished tomorrow
//And the thread dump and thread_state_restore should be the next task. 
//and this task should check the address to see which location is the address to be need. 
//use uint32_t because uint16_t is too small 

#ifdef PLATFORM_AVR
void reply_debugging_remove_avr_breakpoint( uint8_t *receivebuffer ) {
   uint32_t addrbreakpoint;
   uint16_t pagecount;
   uint8_t offset;
   uint8_t i;
   uint8_t breakpointfound = 0;
   addrbreakpoint = (( uint16_t )receivebuffer[ 3 ] *256 ) + receivebuffer[ 4 ];
   for ( i = 0; i < LITE_MAX_THREADS; i ++ ) {
      if ( thread_table[ i ].state == 8 ) {
         uint8_t *tempptr = ( uint8_t* )thread_table[ i ].sp + 47;
         uint16_t addr = ((( uint16_t )( *tempptr )) *256+*( tempptr + 1 ) - 3 );
         if ( addr == addrbreakpoint ) {
            thread_table[ i ].state = STATE_ACTIVE;
            *tempptr = addrbreakpoint / 256;
            *( tempptr + 1 ) = addrbreakpoint % 256;
            break;
         }
      }
   }
   if ( i < LITE_MAX_THREADS ) {
      pagecount = addrbreakpoint / 128;
      offset = addrbreakpoint * 2 % 256;
      boot_removeBreakPoint( pagecount, offset, &receivebuffer[ 5 ] );
      breakpointfound = 1;
   }
   reply[ 0 ] = 5;
   reply[ 1 ] = 93;
   reply[ 2 ] = nodeid;
   reply[ 3 ] = breakpointfound;
   StandardSocketSend( 0xefef, 0xffff, 32, reply );
}
#endif

//-------------------------------------------------------------------------
static void thread_state_restore( uint8_t index, uint8_t *filename ) {
   MYFILE *fp;
   uint8_t threadsize;
   uint16_t threadramsize;
   uint8_t *internal_ram_start;
   threadsize = sizeof( struct thread );
   threadramsize = ( uint16_t )(( uint8_t* )thread_table[ index ].ramend - ( uint8_t* )thread_table[ index ].ramstart + 1 );
   internal_ram_start = ( uint8_t* )thread_table[ index ].ramstart;
   fp = fopen2( (char *)filename, "r" );
   fread2( fp, &thread_table[ index ], threadsize );
   fseek2( fp, threadsize, 1 );
   fread2( fp, internal_ram_start, threadramsize );
   fclose2( fp );
}


//-------------------------------------------------------------------------
static void thread_state_snapshot( uint8_t index, uint8_t *filename ) {
   MYFILE *fp;
   uint8_t threadsize;
   uint16_t threadramsize;
   uint8_t *internal_ram_start;
   threadsize = sizeof( struct thread );
   threadramsize = ( uint16_t )(( uint8_t* )thread_table[ index ].ramend - ( uint8_t* )thread_table[ index ].ramstart + 1 );
   internal_ram_start = ( uint8_t* )thread_table[ index ].ramstart;
   fp = fopen2( (char *)filename, "w" );
   fwrite2( fp, &thread_table[ index ], threadsize );
   fseek2( fp, threadsize, 1 );
   fwrite2( fp, internal_ram_start, threadramsize );
   fclose2( fp );
}

//this should copy the thread information into a file 
//The packet should contain the information regarding the user 
//THe packet should contain the index of the thread and the file name that will be written as the new file 
//the file name part requires careful debugging of the file system 
void reply_thread_state_snapshot_tofile( uint8_t *receivebuffer ) {
   uint8_t i;
   uint8_t testtrue = 0;
   receivebuffer[ receivebuffer[ 0] ] = '\0';
   for ( i = 0; i < LITE_MAX_THREADS; i ++ ) {
      if ( thread_table[ i ].state != STATE_NULL ) {
         if ( superstring( (char*) thread_table[ i ].threadName, ( char* )& receivebuffer[ 5 ] ) == 0 ) {
            testtrue = i;
            break;
         } 
      }
   }
   if ( testtrue != 0 ) {
      receivebuffer[ receivebuffer[ 3 ] + 5 ] = '/';
      thread_state_snapshot( testtrue, ( uint8_t* )& receivebuffer[ receivebuffer[ 3 ] + 5 ] );
   }
   reply[ 0 ] = 4;
   reply[ 1 ] = 94;
   reply[ 2 ] = nodeid;
   reply[ 3 ] = testtrue;
   StandardSocketSend( 0xefef, 0xffff, 32, reply );
}

//this should copy the thread information from the file to the thread
//the packet should ocntain the file name as well as the index of the thread to be restored
//note that it will check the file size to ensure that is sufficient information for the thread to be restored
void reply_thread_state_restore_fromfile( uint8_t *receivebuffer ) {
   uint8_t i;
   uint8_t testtrue = 0;
   receivebuffer[ receivebuffer[ 0] ] = '\0';
   for ( i = 0; i < LITE_MAX_THREADS; i ++ ) {
      if ( thread_table[ i ].state != STATE_NULL ) {
         if ( superstring( (char *) thread_table[ i ].threadName, ( char* )& receivebuffer[ 5 ] ) == 0 ) {
            testtrue = i;
            break;
         }
      }
   }
   i = receivebuffer[ 3 ];
   receivebuffer[ 5+i ] = '/';
   if ( testtrue != 0 ) {
      thread_state_restore( testtrue, ( uint8_t* )& receivebuffer[ 5+i ] );
   } 
   reply[ 0 ] = 4;
   reply[ 1 ] = 95;
   reply[ 2 ] = nodeid;
   reply[ 3 ] = testtrue;
   StandardSocketSend( 0xefef, 0xffff, 32, reply );
}

//-------------------------------------------------------------------------
void reply_ls_networkname() {
   //   uint16_t temp2; 
   uint8_t temp = mystrlen( networkid );
   uint16_t randomsleep;
   reply[ 0 ] = temp + 3;
   reply[ 1 ] = 101;
   //   foobar(); 
   reply[ 2 ] = nodeid;
   mystrncpy( (char*)&reply[ 3 ], networkid, temp );
   randomsleep = getRandomNumber() % 400;
   sleepThread( randomsleep );
   // for (temp2=0;temp2<9000;temp2++)
   // {
   StandardSocketSend( 0xefef, 0xffff, 32, reply );
   // sleepThread(20);
   // }
   //   printStringN(reply,32); 
   //printString("hello,reply2!\n\0");
}

//-------------------------------------------------------------------------
void reply_ls_nodename( uint8_t *receivebuffer ) {
   uint8_t temp = receivebuffer[ 0 ] - 3;
   uint16_t randomsleep;
   if ( mystrncmp(( char* )receivebuffer, 3, ( char* )networkid, 0, temp ) == 0 ) {
      temp = mystrlen( filenameid );
      reply[ 0 ] = temp + 3;
      reply[ 1 ] = 102;
      reply[ 2 ] = nodeid;
      mystrncpy( (char *)&reply[ 3 ], filenameid, temp );
      randomsleep = getRandomNumber() % 400;
      sleepThread( randomsleep );
      StandardSocketSend( 0xefef, 0xffff, 32, reply );
      //printStringN(reply,32); 
   }
}
//Get the block information of sub directory and files using simplfied version 
void reply_ls_short( uint8_t block ) {
   //Actually this is not used because to ensure that the system is simple, we only use the long version 
   return ;
}
//Get the block information of sub directory and files using long version 
void reply_ls_long( uint8_t block ) {
   uint8_t blockindex;
   uint8_t childblock;
   uint8_t seq = 0;
   if ( openedfile != NULL ) {
      //fclose2( openedfile );
       openedfile = NULL; 
   
   } 
   for ( blockindex = 0; blockindex < 10; blockindex ++ ) {
      childblock = fsread8uint( block, DIR_ADDRSUBOFFSET + blockindex );
      if ( childblock == 0 ) {
         continue;
      } 
      reply[ 0 ] = 32;
      reply[ 1 ] = 104;
      reply[ 2 ] = nodeid;
      reply[ 3 ] = childblock;
      reply[ 4 ] = seq;
      seq ++;
      finfonode( (char*) &reply[ 5 ], ( int )childblock );
      StandardSocketSend( 0xefef, 0xffff, 32, reply );
      //     printStringN(reply,32); 
   }
}

//-------------------------------------------------------------------------
void reply_mkdir( uint8_t *receivebuffer ) {
   uint8_t blockid;
   char dirname[ 13 ];
   uint8_t newblockid;
   blockid = receivebuffer[ 3 ];
   mystrncpy( dirname, (char *)&receivebuffer[ 4 ], receivebuffer[ 0 ] - 4 );
   dirname[ receivebuffer[ 0 ] - 4 ] = '\0';
   newblockid = createDir( dirname, ( int )blockid );
   reply[ 0 ] = 4;
   reply[ 1 ] = 141;
   reply[ 2 ] = nodeid;
   reply[ 3 ] = newblockid;
   StandardSocketSend( 0xefef, 0xffff, 32, reply );
   // printStringN(reply,32); 	
}

//-------------------------------------------------------------------------
void reply_cp_fromPCtoNode_type1( uint8_t *receivebuffer ) {
   uint8_t blockid;
   char filename[ 13 ];
   int fid;
   uint8_t newblockid;
   blockid = receivebuffer[ 3 ];
   mystrncpy( filename, (char *)&receivebuffer[ 4 ], receivebuffer[ 0 ] - 4 );
   filename[ receivebuffer[ 0 ] - 4 ] = '\0';
   newblockid = createFileFromDirectory( filename, ( int )blockid );
   chunksequencenum = 0;
   expectNextNum = 1;
   reply[ 0 ] = 4;
   reply[ 1 ] = 121;
   reply[ 2 ] = nodeid;
   reply[ 3 ] = newblockid;
   fid = getFreeFid();
   openFile( newblockid, fid, 2 );
   openedfile = &fidtable[ fid ];
   // printStringN(reply,32); 	
   StandardSocketSend( 0xefef, 0xffff, 32, reply );
}

//-------------------------------------------------------------------------
void reply_cp_fromPCtoNode_type2( uint8_t *receivebuffer ) {
   uint16_t SyncNum;
   if ( receivebuffer[ 3 ] == 0xff ) {
      //now this is a sync
      SyncNum = (( uint16_t )receivebuffer[ 4 ] ) *256+receivebuffer[ 5 ];
      if ( SyncNum > expectNextNum - 1 ) {
         reply[ 0 ] = 7;
         reply[ 1 ] = 122;
         reply[ 2 ] = nodeid;
         reply[ 3 ] = 15;
         reply[ 4 ] = 15;
         reply[ 5 ] = ( expectNextNum - 1 ) / 256;
         reply[ 6 ] = ( expectNextNum - 1 ) % 256;
         StandardSocketSend( 0xefef, 0xffff, 32, reply );
      } else {
         reply[ 0 ] = 7;
         reply[ 1 ] = 122;
         reply[ 2 ] = nodeid;
         reply[ 3 ] = 15;
         reply[ 4 ] = 15;
         reply[ 5 ] = 0;
         reply[ 6 ] = 0;
         StandardSocketSend( 0xefef, 0xffff, 32, reply );
      }
      return ;
   }
   chunksequencenum = (( uint16_t )receivebuffer[ 4 ] ) *256+receivebuffer[ 5 ];
   if ( chunksequencenum != expectNextNum ) {
      return ;
   } 
   fwrite2( openedfile, &receivebuffer[ 7 ], receivebuffer[ 6 ] );
   fseek2( openedfile, receivebuffer[ 6 ], 1 );
   expectNextNum ++;
   // printStringN(reply,32); 	
   //StandardSocketSend(0xefef, 0xffff, 32, reply);
}

//-------------------------------------------------------------------------
void reply_cp_fromPCtoNode_type3() {
   fclose2( openedfile );
   // printStringN(reply,32); 	
   StandardSocketSend( 0xefef, 0xffff, 32, reply );
}

//-------------------------------------------------------------------------

void reply_touch( uint8_t *receivebuffer ) {
   uint8_t blockid;
   char filename[ 13 ];
   uint8_t newblockid;
   blockid = receivebuffer[ 3 ];
   mystrncpy( filename, (char *)&receivebuffer[ 4 ], receivebuffer[ 0 ] - 4 );
   filename[ receivebuffer[ 0 ] - 4 ] = '\0';
   newblockid = createFileFromDirectory( filename, ( int )blockid );
   reply[ 0 ] = 4;
   reply[ 1 ] = 151;
   reply[ 2 ] = nodeid;
   reply[ 3 ] = newblockid;
   StandardSocketSend( 0xefef, 0xffff, 32, reply );
   //printStringN(reply,32); 	
}

//-------------------------------------------------------------------------
void reply_fromNodeToPC_type1( uint8_t *receivebuffer ) {
   uint8_t blockid;
   char filename[ 13 ];
   int fid;
   uint16_t filesize;
   uint16_t round;
   uint16_t remain;
   uint16_t i;
   uint8_t newblockid;
   uint8_t temp1, temp2;
   blockid = receivebuffer[ 3 ];
   mystrncpy( filename, (char *)&receivebuffer[ 4 ], receivebuffer[ 0 ] - 4 );
   filename[ receivebuffer[ 0 ] - 4 ] = '\0';
   newblockid = existBlockAddr( filename, ( int )blockid );
   reply[ 1 ] = 124;
   reply[ 2 ] = nodeid;
   fid = getFreeFid();
   openFile( newblockid, fid, 1 );
   fp = &fidtable[ fid ];
   filesize = fp->size;
   temp1 = filesize / 256;
   temp2 = filesize % 256;
   reply[ 3 ] = temp1;
   reply[ 4 ] = temp2;
   //send out the size 
   StandardSocketSend( 0xefef, 0xffff, 32, reply );
   //    StandardSocketSend(0xefef, 0xffff, 32, reply);
   //sleepThread( 100 );
   reply[ 3 ] = newblockid;
   round = filesize / 20;
   remain = filesize % 20;
   for ( i = 0; i < round; i ++ ) {
      fread2( fp, &reply[ 6 ], 20 );
      fseek2( fp, 20, 1 );
      reply[ 0 ] = 26;
      temp1 = i / 256;
      temp2 = i % 256;
      reply[ 4 ] = temp1;
      reply[ 5 ] = temp2;
      //printStringN(reply,32); 		    
     // sleepThread( 100 );
      StandardSocketSend( 0xefef, 0xffff, 32, reply );
   }
   if ( remain > 0 ) {
      fread2( fp, &reply[ 6 ], remain );
      reply[ 0 ] = ( uint8_t )remain + 6;
      reply[ 4 ] = round / 256;
      reply[ 5 ] = round % 256;
      round ++;
      //printStringN(reply,32);
      //sleepThread( 25 );
      StandardSocketSend( 0xefef, 0xffff, 32, reply );
   }
   temp1 = round / 256;
   temp2 = round % 256;
   //terminate signal  is here!
   reply[ 0 ] = 7;
   reply[ 3 ] = 0xef;
   reply[ 4 ] = 0xef;
   reply[ 5 ] = temp1;
   reply[ 6 ] = temp2;
   //ephread( 100 );
   StandardSocketSend( 0xefef, 0xffff, 32, reply );
   //sleepThread( 100 );
   StandardSocketSend( 0xefef, 0xffff, 32, reply );
   //sleepThread( 100 );
   StandardSocketSend( 0xefef, 0xffff, 32, reply );
   //sleepThread( 100 );
   StandardSocketSend( 0xefef, 0xffff, 32, reply );
   //do not close now!
   //	 fclose2(fp); 
}

//-------------------------------------------------------------------------
void reply_rm( uint8_t *receivebuffer ) {
   uint8_t blockid;
   char filename[ 13 ];
   uint8_t newblockid;
   blockid = receivebuffer[ 3 ];
   mystrncpy( filename, (char *)&receivebuffer[ 4 ], receivebuffer[ 0 ] - 4 );
   filename[ receivebuffer[ 0 ] - 4 ] = '\0';
   newblockid = existBlockAddr( filename, ( int )blockid );
   if ( newblockid > 0 ) {
      deleteNode( newblockid );
   } 
   reply[ 0 ] = 4;
   reply[ 1 ] = 161;
   reply[ 2 ] = nodeid;
   reply[ 3 ] = newblockid;
   StandardSocketSend( 0xefef, 0xffff, 32, reply );
   //printStringN(reply,32); 	
}


//get patch request
void reply_fromNodeToPC_type2( uint8_t *receivebuffer )
 {
   uint16_t pos;
   pos = (( uint16_t )receivebuffer[ 4 ] ) *256+receivebuffer[ 5 ];
   reply[ 0 ] = 26;
   reply[ 1 ] = 125;
   reply[ 2 ] = nodeid;
   reply[ 3 ] = pos / 256;
   reply[ 4 ] = pos % 256;
   fseek2( fp, ( int32_t )pos, 0 );
   fread2( fp, &reply[ 6 ], 20 );
   StandardSocketSend( 0xefef, 0xffff, 32, reply );
}

//-------------------------------------------------------------------------
//possible bug here 
void reply126( uint8_t *receivebuffer ) {
   fclose2( fp );
}

//-------------------------------------------------------------------------
void reply_ps( uint8_t *receivebuffer ) {
   uint8_t i, len;
   //reply[0] = 15;
   reply[ 1 ] = 171;
   reply[ 2 ] = nodeid;
   for ( i = 0; i < LITE_MAX_THREADS; i ++ ) {
      if ( thread_table[ i ].state != STATE_NULL ) {
         len = mystrlen( (char *)thread_table[ i ].threadName );
         reply[ 0 ] = len + 4;
         reply[ 3 ] = thread_table[ i ].state;
         //if this is a break thread, then fetch the real number
         if ( reply[ 3 ] == 8 ) {
            uint8_t *tempptr = ( uint8_t* )thread_table[ i ].sp + 47;
            uint16_t addr = ((( uint16_t )( *tempptr )) *256+*( tempptr + 1 ) - 3 );
            reply[ 4 ] = addr / 256;
            reply[ 5 ] = addr % 256;
            reply[ 0 ] += 2;
            mystrncpy( (char *)&reply[ 6 ], (char *)thread_table[ i ].threadName, len );
         }
          else {
            mystrncpy( (char *)&reply[ 4 ], (char *)thread_table[ i ].threadName, len );
         } 
         StandardSocketSend( 0xefef, 0xffff, 32, reply );
      }
   }
}


//-------------------------------------------------------------------------

//Is this function avr specific? Does not look so, though. 

void reply_memory( uint8_t *receivebuffer ) {
   uint8_t i, len;

   //reply[0] = 15;

   reply[ 1 ] = 181;
   reply[ 2 ] = nodeid;
   reply[ 3 ] = kernelromsize/256;
   reply[ 4 ] = kernelromsize%256; 
   reply[ 5 ] = kernelramsize/256; 
   reply[ 6 ] = kernelramsize%256; 
  

   for ( i = 0; i < LITE_MAX_THREADS; i ++ ) {
      if ( thread_table[ i ].state != STATE_NULL ) {
         len = mystrlen( (char *)thread_table[ i ].threadName );

         reply[7] = len; 

		 mystrncpy( (char *)&reply[ 8 ], (char *)thread_table[ i ].threadName, len );
         
		 reply[8+len] = 0; 

		 //Note that here the romstart is the exact start location in words
		 //the romesize is in bytes
		 //the ramstart is in bytes
		 //the ramsize is in bytes

		 reply[len+9] =  thread_table[i].romstart/256; 
		 reply[len+10] = thread_table[i].romstart%256; 
		 reply[len+11] = thread_table[i].romsize/256; 
		 reply[len+12] = thread_table[i].romsize%256;
		 reply[len+13] = ((uint16_t)thread_table[i].ramstart)/256; 
		 reply[len+14] = ((uint16_t)thread_table[i].ramstart)%256;
		 reply[len+15] = ((uint16_t)thread_table[i].ramend)/256; 
		 reply[len+16] = ((uint16_t)thread_table[i].ramend)%256;

         reply[len+17] = ((uint16_t)thread_table[i].sp)/256;
		 reply[len+18] = ((uint16_t)thread_table[i].sp)%256;
		 reply[len+19] = ((uint16_t)thread_table[i].sizeofBss)/256;
		 reply[len+20] = ((uint16_t)thread_table[i].sizeofBss)%256;

		 reply[len+21] = ((uint16_t)old_stack_ptr)/256;
		 reply[len+22] = ((uint16_t)old_stack_ptr)%256;

		 reply[0] = len+16+1; 

		  
         StandardSocketSend( 0xefef, 0xffff, 32, reply );


      }
   }
}



//-------------------------------------------------------------------------
void reply_killthread( uint8_t *receivebuffer ) {
   uint8_t i;
   uint8_t testtrue = 0;
   uint8_t *start, *end; 
   uint8_t index; 
   start = NULL;
   end = NULL; 
   
   receivebuffer[ receivebuffer[ 0] ] = '\0';

   for ( i = 0; i < LITE_MAX_THREADS; i ++ ) {
      if ( thread_table[ i ].state != STATE_NULL ) {
         if ( superstring( (char *)thread_table[ i ].threadName, ( char* )& receivebuffer[ 3 ] ) == 0 ) {

            start = (uint8_t*)thread_table [ i ].ramstart; 
			end   = (uint8_t*)thread_table [ i ].ramend; 
			index = i; 
			
			thread_table[ i ].state = STATE_NULL;
            testtrue = 1;

			
         }
      }
   	}

  
 
 if (testtrue == 1)
 	{
 	 deleteThreadRegistrationInReceiverHandles(start, end);
	 releaseMutexLockUponThreadKill( index ); 
	 
   
    if (thread_clear_func_table[index] != NULL)
       { (*thread_clear_func_table[index])(); 
	     thread_clear_func_table[index] = NULL; 


       }

   if (timercallback[index] != NULL)
      {
	    timercallback[index] = NULL; 
		GenericTimerStop(	index + 12 ); 	  }   



   /*for ( i = 0; i < RECEIVE_HANDLE_NUM; i ++ )
   { if (( receivehandles[ i ].handlevalid == 1 ) && ( receivehandles[ i ].dataReady <= end ) && ( receivehandles[ i ].dataReady >= start )) {
      receivehandles[ i ].handlevalid = 0;
      receivehandles[ i ].port = 0;     
   	}
   	}
   	*/
 	}



   reply[ 0 ] = 4;
   reply[ 1 ] = 232;
   reply[ 2 ] = nodeid;
   reply[ 3 ] = testtrue;
   StandardSocketSend( 0xefef, 0xffff, 32, reply );
}


  //-------------------------------------------------------------------------
void reply_du( uint8_t *receivebuffer ) {
   int e1, e2, e3;
   e1 = fcheckFSAllocation();
   e2 = fcheckStorageAllocation();
   e3 = 0;
   if ( e2 == 256 ) {
      e2 = 128;
      e3 = 128;
   }
   reply[ 0 ] = 6;
   reply[ 1 ] = 211;
   reply[ 2 ] = nodeid;
   reply[ 3 ] = e1;
   reply[ 4 ] = e2;
   reply[ 5 ] = e3;
   //printStringN(reply,32); 	
   StandardSocketSend( 0xefef, 0xffff, 32, reply );
}

//-------------------------------------------------------------------------
void reply_search( uint8_t *receivebuffer ) {
   char filename[ 13 ];
   uint8_t searchresults[ 20 ];
   uint8_t numofsearch;
   uint8_t i;
   mystrncpy( filename, (char *)&receivebuffer[ 3 ], receivebuffer[ 0 ] - 3 );
   filename[ receivebuffer[ 0 ] - 3 ] = '\0';
   fsearch( searchresults, &numofsearch, filename );
   for ( i = 0; i < numofsearch; i ++ ) {
      fdirnode( (char *)&reply[ 3 ], 29, searchresults[ i ] );
      reply[ 0 ] = mystrlen( (char *)&reply[ 3 ] ) + 3;
      reply[ 1 ] = 221;
      reply[ 2 ] = nodeid;
      //printStringN(reply,32); 	
      StandardSocketSend( 0xefef, 0xffff, 32, reply );
   }
}

//-------------------------------------------------------------------------
void createNewThread() {
   create_thread( createtaskthread, createramstart, createstackpointer, createstaticdatasize, 1, (char *)createthreadname, createflashromstart, createflashromsize);
}






//-------------------------------------------------------------------------
//This function reads from filename and then creates a thread out of it
//Therefore, this function is highly avr-specific

#ifdef PLATFORM_AVR
void create_thread_task() {
   int fid;
   uint8_t tempbuffer[ 20 ];
   MYFILE *fp;
   uint16_t filesize;
   uint16_t round;
   uint8_t remain;
   uint16_t i;
   uint8_t newblockid;
   uint8_t sysinfo[ 16 ];
   uint16_t pageno;
   uint16_t entryaddr;
   uint16_t ramstackend;
   uint16_t ramstackstart;
   newblockid = existBlockAddr( filename, ( int )blockid );
   if ( newblockid == 0 ) {
      //reply[ 0 ] = 4;
      //reply[ 1 ] = 231;
      //reply[ 2 ] = nodeid;
      //reply[ 3 ] = 0;
      //StandardSocketSend( 0xefef, 0xffff, 32, reply );
      return ;
   }
   fid = getFreeFid();
   openFile( newblockid, fid, 1 );
   fp = &fidtable[ fid ];
   filesize = fp->size;
   fread2( fp, sysinfo, 16 );
   fseek2( fp, 16, 1 );
   //  mystrncpy(sysinfo, &programimagestring[0], 10); 
   //  filesize = mystrlen(programimagestring); 
   pageno = hex2value( sysinfo[ 0 ] ) *100+hex2value( sysinfo[ 1 ] ) *10+hex2value( sysinfo[ 2 ] );
   entryaddr = ( uint16_t )( hex2value( sysinfo[ 3 ] )) *4096+( uint16_t )( hex2value( sysinfo[ 4 ] )) *256+( uint16_t )( hex2value( sysinfo[ 5 ] )) *16+( uint16_t )( hex2value( sysinfo[ 6 ] ));
   ramstackstart = ( uint16_t )( hex2value( sysinfo[ 7 ] )) *256+( uint16_t )( hex2value( sysinfo[ 8 ] )) *16+( uint16_t )( hex2value( sysinfo[ 9 ] ));
   ramstackend = ( uint16_t )( hex2value( sysinfo[ 10 ] )) *256+( uint16_t )( hex2value( sysinfo[ 11 ] )) *16+( uint16_t )( hex2value( sysinfo[ 12 ] ));
   createstaticdatasize = ( uint16_t )( hex2value( sysinfo[ 13 ] )) *100+( uint16_t )( hex2value( sysinfo[ 14 ] )) *10+( uint16_t )( hex2value( sysinfo[ 15 ] ));

   createflashromstart = ((uint16_t)pageno)*(uint16_t)128;
   createflashromsize = (uint16_t)(filesize-16)/2;       
   
   if ((createflashromstart <= kernelromsize) || ( ramstackstart <= kernelramsize + 256))
   	 {
	  fclose2( fp );
      //reply[ 0 ] = 4;
      //reply[ 1 ] = 231;
      //reply[ 2 ] = nodeid;
      //reply[ 3 ] = 1;
      //StandardSocketSend( 0xefef, 0xffff, 32, reply );
	  return; 	  
	  }
  
    if (memory_conflict_detect(createflashromstart, createflashromsize, ramstackstart, ramstackend ) == 1)
      {
	  fclose2( fp );
      //reply[ 0 ] = 4;
      //reply[ 1 ] = 231;
      //reply[ 2 ] = nodeid;
      //reply[ 3 ] = 1;
      //StandardSocketSend( 0xefef, 0xffff, 32, reply );
	  return; 	  
	  }

     

   round = ( filesize - 16 ) / 16;
   remain = ( filesize - 16 ) % 16;
   ProgramSetPage( pageno );
   for ( i = 0; i < round; i ++ ) {
      fread2( fp, tempbuffer, 16 );
      fseek2( fp, 16, 1 );
      //  mystrncpy(tempbuffer, &programimagestring[i*16+10], 16); 
      ProgramString( (char *)tempbuffer );
   }
   if ( remain > 0 ) {
      fread2( fp, tempbuffer, remain );
      // mystrncpy(tempbuffer, &programimagestring[round*16+10], remain);
      ProgramLastString( (char *)tempbuffer, remain );
   } else {
      ProgramRemains();
   }
   fclose2( fp );
    {
      createtaskthread = ( void( * )( void ))entryaddr;
      createramstart = ( uint16_t* )ramstackstart;
      
     
      createstackpointer = ( uint16_t* )ramstackend;
      mystrncpy( (char *)createthreadname, filename, mystrlen( filename ) + 1 );

      

      postTask( createNewThread, 1 );
   }
   // reply[0] = 4; 
   //	reply[1] = 231;
   //  reply[2] = nodeid;
   //	reply[3] = 1; 
   //		StandardSocketSend(0xefef, 0xffff, 32, reply);	
   // printStringN(reply,32); 
}

#endif 

//-------------------------------------------------------------------------
void reply_create_thread( uint8_t *receivebuffer )
//void reply_create_thread()
 {
   //file system version 
   blockid = receivebuffer[ 3 ];
   mystrncpy( filename, (char *)&receivebuffer[ 4 ], receivebuffer[ 0 ] - 4 );
   filename[ receivebuffer[ 0 ] - 4 ] = '\0';
   reply[ 1 ] = 231;
   reply[ 2 ] = nodeid;
   postTask( create_thread_task, 11 );
   reply[ 0 ] = 4;
   reply[ 1 ] = 231;
   reply[ 2 ] = nodeid;
   reply[ 3 ] = 2;
   StandardSocketSend( 0xefef, 0xffff, 32, reply );
   
   return ;
}

//-------------------------------------------------------------------------
void reply_formatSystem( uint8_t *receivebuffer ) {
   formatSystem();
   buildRootNode();
   buildDeviceDirectory();
}


//-------------------------------------------------------------------------
void reply_nodeRestart( uint8_t *receivebuffer ) {
   resetNode();
}


//-------------------------------------------------------------------------
void WakeupMe() {
   thisthread->state = STATE_ACTIVE;
   postNewThreadTask(); 
}


//-------------------------------------------------------------------------
void InitShell() {
   IncomingLength = 0;
   nodeid = CURRENT_NODE_ID; 
   registerEvent( 0xfefe, 64, &IncomingLength, IncomingMsg, PacketInfo, WakeupMe );
   
}


//This is the stub function of the shell thread that is used to handle incoming packets 
void ShellThread() {
   while ( 1 ) {
      if ( IncomingLength > 0 ) {
         commandHandle( IncomingMsg, IncomingLength );
         IncomingLength = 0;
      } else {
         current_thread->state = STATE_SLEEP;
         thisthread = current_thread;
         thread_yield();
      }
   }
}




//-------------------------------------------------------------------------
void commandHandle( uint8_t *receivebuffer, uint8_t total ) {
   uint8_t commandtype;
  
  /* if ( receivebuffer[ 1 ] == 255 ) {
   		  writeVectorNodeToExternalStorage(); 
   		  writeVectorFlashToExternalStorage();   		  
   		  resetNode();
   }
   */ 
   
   if (  ! (( receivebuffer[ 2 ] == 0 ) || ( receivebuffer[ 2 ] == nodeid ))) {
      return ;
   } 
   
   commandtype = receivebuffer[ 1 ];
   
   switch ( commandtype ) {
      //reply_devicehandling handles the device driver issues 
      case 11:
      reply_devicehandling( receivebuffer );
      break;
      case 12:
      reply_led_control( receivebuffer );
      break;
      case 91:
      reply_debugging_print( receivebuffer );
      break;
      #ifdef PLATFORM_AVR
      case 92:
      reply_debugging_insert_avr_breakpoint( receivebuffer );
      break;
      case 93:
      reply_debugging_remove_avr_breakpoint( receivebuffer );
      break;
      #endif 
      case 94:
      reply_thread_state_snapshot_tofile( receivebuffer );
      break;
      case 95:
      reply_thread_state_restore_fromfile( receivebuffer );
      break;
      case 96:
      reply_debugging_set( receivebuffer );
      break;
      case 101:
      reply_ls_networkname();
      break;
      case 102:
      reply_ls_nodename( receivebuffer );
      break;
      case 103:
      reply_ls_long( receivebuffer[ 3 ] );
      break;
      case 104:
      reply_ls_long( receivebuffer[ 3 ] );
      break;
      case 121:
      reply_cp_fromPCtoNode_type1( receivebuffer );
      break;
      case 122:
      reply_cp_fromPCtoNode_type2( receivebuffer );
      break;
      case 123:
      reply_cp_fromPCtoNode_type3();
      break;
      case 124:
      reply_fromNodeToPC_type1( receivebuffer );
      break;
      case 125:
      reply_fromNodeToPC_type2( receivebuffer );
      break;
      case 141:
      reply_mkdir( receivebuffer );
      break;
      case 151:
      reply_touch( receivebuffer );
      break;
      case 161:
      reply_rm( receivebuffer );
      break;
      case 171:
      reply_ps( receivebuffer );
      break;
      case 181:
      reply_memory( receivebuffer ); 
      break; 
      case 211:
      reply_du( receivebuffer );
      break;
      case 221:
      reply_search( receivebuffer );
      break;
      case 231:
      reply_create_thread( receivebuffer );
      break;
      case 232:
      reply_killthread( receivebuffer );
      break;
      case 233:
      reply_formatSystem( receivebuffer );
      break;
      case 242:
      reply_nodeRestart( receivebuffer );
      break;
   }
}
