#include "FileSocket.h"
#include "threads.h"
#include "commondefines.h"
#include "fsapi.h"
#include "scheduling.h"

extern volatile thread* current_thread; 

char filepathaddr[20]; 
char filemodeaddr[5]; 

volatile mutex filemutex; 

volatile thread *openthread; 
uint8_t threadindex; 


MYFILE *filehandle; 
uint8_t *databuffer;
uint16_t nBytes; 

int offset, position; 





void FileSocketInit()
{

   filemutex.lock = filemutex.waiting = 0; 
   return; 
}




//Get the file mutex address
void getFileMutexAddress()  __attribute__((naked));
void getFileMutexAddress() 
{
   
    volatile mutex *mfile = &filemutex; 
    asm volatile("mov r20, %A0" "\n\t"
	             "mov r21, %B0" "\n\t"
				 :
				 :"r" (mfile)
                );
    asm volatile("ret"::); 
}


//Get the file path address
void getFilePathAddress()  __attribute__((naked));
void getFilePathAddress()
{
   
    asm volatile("mov r20, %A0" "\n\t"
	             "mov r21, %B0" "\n\t"
				 :
				 :"r" (filepathaddr)
                );
    asm volatile("ret"::); 
}

//Get the file mode address
void getFileModeAddress()  __attribute__((naked)) ;
void getFileModeAddress()
{
   
   asm volatile("mov r20, %A0" "\n\t"
	             "mov r21, %B0" "\n\t"
				 :
				 :"r" (filemodeaddr)
                );
    asm volatile("ret"::); 
}




void openfile_task()
{
   MYFILE *temp =  fopen2(filepathaddr, filemodeaddr); 

   openthread->filedata.filestate.fileptr = (uint8_t*)temp; 

   //unblock the file thread 
   Barrier_unblock(7, 1); 
   return; 
}



void openFileTask()
{
   openthread = current_thread; 
   
   postTask(openfile_task, 5);
   return; 
}




void closefile_task()
{ 
  fclose2(filehandle); 
  Barrier_unblock(7, 2); 
}



void closeFileTask()
{
  filehandle = (MYFILE*)current_thread->filedata.filestate.fileptr; 
  postTask(closefile_task, 5);   
}



void readfile_task()
{
  fread2(filehandle, databuffer, nBytes); 
  Barrier_unblock(7, 3); 
  //check the barrier

}

void readFileTask()
{
   filehandle = (MYFILE*)current_thread->filedata.filestate.fileptr; 
   databuffer = current_thread->filedata.filestate.bufferptr; 
   nBytes = current_thread->filedata.filestate.bytes; 
   postTask(readfile_task, 5);  
  

  
}



void writefile_task()
{
  fwrite2(filehandle, databuffer, nBytes); 
  Barrier_unblock(7,4);


}



void writeFileTask()
{
   filehandle = (MYFILE*)current_thread->filedata.filestate.fileptr; 
   databuffer = current_thread->filedata.filestate.bufferptr; 
   nBytes = current_thread->filedata.filestate.bytes; 
   postTask(writefile_task, 5); 
}



void seekFileTask()
{
  filehandle = (MYFILE*)current_thread->filedata.fileseekstate.fileptr; 
  offset = current_thread->filedata.fileseekstate.offset; 
  position = current_thread->filedata.fileseekstate.position; 
  fseek2(filehandle, offset, position); 
  return;   
}

