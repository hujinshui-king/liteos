#include "syscall.h"
#include "Leds.h"
#include "threads.h"
#include "Socket.h"




//EA00
 void yieldfunction() __attribute__((section (".systemcall"))) __attribute__((naked));
 void yieldfunction()
{
  yield(); 
  asm volatile("nop"::);
  asm volatile("ret"::); 
}

void greentogglefunction() __attribute__((section (".systemcall")))  __attribute__((naked)) ;
void greentogglefunction()
{
   Leds_greenToggle(); 
   asm volatile("nop"::);
   asm volatile("ret"::); 
}

void redtogglefunction() __attribute__((section (".systemcall"))) __attribute__((naked)) ;
void redtogglefunction()
{
   Leds_redToggle();
   asm volatile("nop"::);
   asm volatile("ret"::); 

}


//EA09
//This function is going to put the address into two registers, R24 and R25
void getCurrentThreadAddress() __attribute__((section (".systemcall"))) __attribute__((naked));
void getCurrentThreadAddress()
{
  getThreadAddress(); 
  asm volatile("nop"::);
  asm volatile("ret"::); 
}


//Get the mutex of radio send address 
void getRadioMutex() __attribute__((section (".systemcall"))) __attribute__((naked));
void getRadioMutex()
{
  getRadioMutexAddress();
  asm volatile("nop"::); 
  asm volatile("ret"::); 

}

void getRadioSendFunction() __attribute__((section (".systemcall"))) __attribute__((naked));
void getRadioSendFunction()
{
  SocketRadioSend(); 
  asm volatile("nop"::);
  asm volatile("ret"::); 

}

void unlockMutex() __attribute__((section (".systemcall"))) __attribute__((naked)) ;
void unlockMutex()
{
  mutexUnlockFunction();
  asm volatile("nop"::); 
  asm volatile("ret"::); 

}



//This function is going to put the address into two registers, R24 and R25
void getCurrentThreadIndex() __attribute__((section (".systemcall"))) __attribute__((naked));
void getCurrentThreadIndex()
{
  getThreadIndexAddress();
  asm volatile("nop"::); 
  asm volatile("ret"::); 
}



void getFilePathAddr() __attribute__((section (".systemcall"))) __attribute__((naked));
void getFilePathAddr()
{
  getFilePathAddress(); 
  asm volatile("nop"::);
  asm volatile("ret"::); 
}


void getFileModeAddr() __attribute__((section (".systemcall"))) __attribute__((naked));
void getFileModeAddr()
{
  getFileModeAddress(); 
  asm volatile("nop"::);
  asm volatile("ret"::); 
}


void getFileMutexAddr() __attribute__((section (".systemcall"))) __attribute__((naked));
void getFileMutexAddr()
{
  getFileMutexAddress();
  asm volatile("nop"::); 
  asm volatile("ret"::); 
}



void openFileSysCall() __attribute__((section (".systemcall"))) __attribute__((naked));
void openFileSysCall()
{
  openFileTask(); 
  asm volatile("nop"::);
  asm volatile("ret"::); 
}

void closeFileSysCall() __attribute__((section (".systemcall"))) __attribute__((naked));
void closeFileSysCall()
{
  closeFileTask(); 
  asm volatile("nop"::);
  asm volatile("ret"::); 
}

void readFileSysCall() __attribute__((section (".systemcall"))) __attribute__((naked));
void readFileSysCall()
{
  readFileTask(); asm volatile("nop"::);
  asm volatile("ret"::); 
}

void writeFileSysCall() __attribute__((section (".systemcall"))) __attribute__((naked));
void writeFileSysCall()
{
  writeFileTask(); 
  asm volatile("nop"::);
  asm volatile("ret"::); 
}



void seekFileSysCall() __attribute__((section (".systemcall"))) __attribute__((naked));
void seekFileSysCall()
{
  seekFileTask(); asm volatile("nop"::);
  asm volatile("ret"::); 
}


void ADCLightSysCall() __attribute__((section (".systemcall"))) __attribute__((naked));
void ADCLightSysCall()
{
  ADCLight(); 
  asm volatile("nop"::);
  asm volatile("ret"::); 
}

void ADCTempSysCall() __attribute__((section (".systemcall"))) __attribute__((naked));
void ADCTempSysCall()
{
  ADCTemp(); 
  asm volatile("nop"::);
  asm volatile("ret"::); 
}


void ADCMagXSysCall() __attribute__((section (".systemcall"))) __attribute__((naked));
void ADCMagXSysCall()
{
  ADCMagX(); 
  asm volatile("nop"::);
  asm volatile("ret"::); 
}


void ADCMagYSysCall() __attribute__((section (".systemcall"))) __attribute__((naked));
void ADCMagYSysCall()
{
  ADCMagY(); 
  asm volatile("nop"::);
  asm volatile("ret"::); 
}


void ADCAccXSysCall() __attribute__((section (".systemcall"))) __attribute__((naked));
void ADCAccXSysCall()
{
  ADCAccX(); 
  asm volatile("nop"::);
  asm volatile("ret"::); 
}


void ADCAccYSysCall() __attribute__((section (".systemcall"))) __attribute__((naked));
void ADCAccYSysCall()
{
  ADCAccY(); 
  asm volatile("nop"::);
  asm volatile("ret"::); 
}
