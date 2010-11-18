/* The LiteOS Operating System Kernel */
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
 
 
#include "filesocket.h"
#include "../system/threads.h"
#include "fsapi.h"
#include "../system/scheduling.h"

extern volatile thread *current_thread;
char filepathaddr[20];
char filemodeaddr[5];
volatile mutex filemutex;
volatile thread *openthread;

uint8_t threadindex;
MYFILE *filehandle;
uint8_t *databuffer;
uint16_t nBytes;
int offset, position;

//-------------------------------------------------------------------------
void filesocketInit()
{
    filemutex.lock = filemutex.waiting = filemutex.lockingthreadid = 0;
    return;
}

//-------------------------------------------------------------------------
//Get the file mutex address
void *getFileMutexAddress()
{
    volatile mutex *mfile = &filemutex;

    return (void *)mfile;
}


//-------------------------------------------------------------------------
//Get the file path address
void *getFilePathAddress()
{
    return (void *)filepathaddr;
}

//-------------------------------------------------------------------------
//Get the file mode address
void *getFileModeAddress()
{
    return (void *)filemodeaddr;
}

//-------------------------------------------------------------------------
void openfile_task()
{
    MYFILE *temp = fopen2(filepathaddr, filemodeaddr);

    openthread->filedata.filestate.fileptr = (uint8_t *) temp;

    //unblock the file thread 
    Barrier_unblock(7, 1);
    return;
}

//-------------------------------------------------------------------------
void openFileTask()
{
    openthread = current_thread;
    postTask(openfile_task, 5);
    return;
}

//-------------------------------------------------------------------------
void closefile_task()
{
    fclose2(filehandle);
    Barrier_unblock(7, 2);
}

//-------------------------------------------------------------------------
void closeFileTask()
{
    filehandle = (MYFILE *) current_thread->filedata.filestate.fileptr;
    postTask(closefile_task, 5);
}

//-------------------------------------------------------------------------
void readfile_task()
{
    fread2(filehandle, databuffer, nBytes);
    Barrier_unblock(7, 3);
}

//-------------------------------------------------------------------------
void readFileTask()
{
    filehandle = (MYFILE *) current_thread->filedata.filestate.fileptr;
    databuffer = current_thread->filedata.filestate.bufferptr;
    nBytes = current_thread->filedata.filestate.bytes;
    postTask(readfile_task, 5);
}

//-------------------------------------------------------------------------
void writefile_task()
{
    fwrite2(filehandle, databuffer, nBytes);
    Barrier_unblock(7, 4);
}

//-------------------------------------------------------------------------
void writeFileTask()
{
    filehandle = (MYFILE *) current_thread->filedata.filestate.fileptr;
    databuffer = current_thread->filedata.filestate.bufferptr;
    nBytes = current_thread->filedata.filestate.bytes;
    postTask(writefile_task, 5);
}

//-------------------------------------------------------------------------
void seekFileTask()
{
    filehandle = (MYFILE *) current_thread->filedata.fileseekstate.fileptr;
    offset = current_thread->filedata.fileseekstate.offset;
    position = current_thread->filedata.fileseekstate.position;
    fseek2(filehandle, (int32_t) offset, position);
    return;
}
