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

#include "file.h"
#include "stringutil.h"
#include "thread.h"
#include "mutex.h"
#include "liteoscommon.h"



char *getFilePathAddress()
{
   char *pathaddr;
   void (*getaddrfp)(void) = (void (*)(void))GET_FILE_PATH_ADDRESS;
   asm volatile("push r20" "\n\t"
                "push r21" "\n\t"
                ::);
   getaddrfp();
   asm volatile(" mov %A0, r20" "\n\t"
	             "mov %B0, r21" "\n\t"
				 :"=r" (pathaddr)
				 :);
    asm volatile("pop r21" "\n\t"
	             "pop r20" "\n\t"
	              ::);
    return pathaddr;
}



char *getFileModeAddress()

{
   char *modeaddr;
   void (*getaddrfp)(void) = (void (*)(void))GET_FILE_MODE_ADDRESS;
   asm volatile("push r20" "\n\t"
                "push r21" "\n\t"
                ::);
   getaddrfp();
   asm volatile(" mov %A0, r20" "\n\t"
	             "mov %B0, r21" "\n\t"
				 :"=r" (modeaddr)
				 :);
    asm volatile("pop r21" "\n\t"
	             "pop r20" "\n\t"
	              ::);
    return modeaddr;
}




void openFileSysCall()
{
 void (*filefp)() = (void (*)(void))OPEN_FILE_SYSCALL;
 filefp();
}



void closeFileSysCall()
{
 void (*filefp)() = (void (*)(void))CLOSE_FILE_SYSCALL;
 filefp();
}


void readFileSysCall()
{
 void (*filefp)() = (void (*)(void))READ_FILE_SYSCALL;
 filefp();
}



void writeFileSysCall()
{
 void (*filefp)() = (void (*)(void))WRITE_FILE_SYSCALL;
 filefp();
}



void seekFileSysCall()
{
 void (*filefp)() = (void (*)(void))SEEK_FILE_SYSCALL;
 filefp();
}




MYFILE *mfopen(char *pathname, char *mode)
{
   char *commonpathnameaddr;

   char *commonmodeaddr;

   uint8_t currentthreadindex;

   thread** current_thread;

   mutex *mfile;

   current_thread = getCurrentThread();

   currentthreadindex = getCurrentThreadIndex();

   commonpathnameaddr =  getFilePathAddress();

   commonmodeaddr = getFileModeAddress();

   mfile = getFileMutexAddress();


   Mutex_lock(mfile);

   mystrcpy(commonpathnameaddr, pathname);
   mystrcpy(commonmodeaddr, mode);

   openFileSysCall();

   Barrier_block(7, 1);
   Mutex_unlock(mfile);

   return (MYFILE *)((*current_thread)->filedata.filestate.fileptr);
}



void mfclose(MYFILE *fp)
{
   uint8_t currentthreadindex;
   thread** current_thread;
   mutex *mfile;

   current_thread = getCurrentThread();
   currentthreadindex = getCurrentThreadIndex();
   mfile = getFileMutexAddress();

   Mutex_lock(mfile);

   (*current_thread)->filedata.filestate.fileptr = (uint8_t*)fp;
   closeFileSysCall();

   Barrier_block(7, 2);
   Mutex_unlock(mfile);

   return;

}

void mfread(MYFILE *fp, void *buffer, int nBytes)
{
   uint8_t currentthreadindex;
   thread** current_thread;
   mutex *mfile;

   current_thread = getCurrentThread();
   currentthreadindex = getCurrentThreadIndex();
   mfile = getFileMutexAddress();


   Mutex_lock(mfile);
   (*current_thread)->filedata.filestate.fileptr = (uint8_t*)fp;
   (*current_thread)->filedata.filestate.bufferptr = (uint8_t*)buffer;
   (*current_thread)->filedata.filestate.bytes = nBytes;

   readFileSysCall();

   Barrier_block(7, 3);
   Mutex_unlock(mfile);

   return;

}



void mfwrite(MYFILE *fp, void *buffer, int nBytes)
{
   uint8_t currentthreadindex;
   thread** current_thread;
   mutex *mfile;

   current_thread = getCurrentThread();
   currentthreadindex = getCurrentThreadIndex();
   mfile = getFileMutexAddress();


   Mutex_lock(mfile);
   (*current_thread)->filedata.filestate.fileptr = (uint8_t*)fp;
   (*current_thread)->filedata.filestate.bufferptr = (uint8_t*)buffer;
   (*current_thread)->filedata.filestate.bytes = nBytes;

   writeFileSysCall();

   Barrier_block(7, 4);
   Mutex_unlock(mfile);

   return;
}


void mfwrite_withoutlength(MYFILE *fp, void *buffer)
{


   uint8_t length;

   length = (uint8_t)String_length((char*)buffer);

   mfwrite(fp, buffer, length);

   return;
}






void mfseek(MYFILE *fp, int offset, int position)
{
   uint8_t currentthreadindex;
   thread** current_thread;

   current_thread = getCurrentThread();
   currentthreadindex = getCurrentThreadIndex();

   (*current_thread)->filedata.fileseekstate.fileptr = (uint8_t*)fp;
   (*current_thread)->filedata.fileseekstate.offset = offset;
   (*current_thread)->filedata.fileseekstate.position = position;

   seekFileSysCall();

   return;



}
