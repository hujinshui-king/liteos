#include "eventlogger.h"
#include "../filesys/fsapi.h"
#include "../io/avr_serial/serialprint.h"

#define EVENTTRACESIZE 128

uint8_t trace[EVENTTRACESIZE];

uint16_t currentindex;

MYFILE *logptr;

uint8_t enabletracing;


void enabletracingfunction() {
   enabletracing = 1;
}

void disabletracingfunction(){
	 enabletracing = 0;

}
void initTrace()
{
  currentindex = 0;
  enabletracing = 1;
  logptr = NULL;


}


#ifdef SERIAL_TRACE

void addTrace(uint8_t traceid)
{
	usartPrint('K');
	usartPrint(' ');
	usartPrint(48 + (traceid / 100));
	usartPrint(48 + ((traceid / 10) % 10));
	usartPrint(48 + (traceid % 10));
	usartPrint('\n');
}

#else

void addTrace(uint8_t traceid)
{

	openTraceFile();

  _atomic_t _atomic = _atomic_start();

  if (enabletracing == 1)

   trace[currentindex++] = traceid;

  _atomic_end(_atomic);

 if (currentindex == EVENTTRACESIZE)

  	{
	     //Now write the trace into the file and clean it up
		 fwrite2(logptr, trace, EVENTTRACESIZE);
		 fseek2(logptr, EVENTTRACESIZE, 1);
		 currentindex = 0;
    }

}

void dumpTrace()
{
  if (currentindex >0)
  {
         fwrite2(logptr, trace, currentindex);
	     fseek2 (logptr, currentindex, 1);
  }
  currentindex = 0;
}



void closeTraceFile()
{
   fclose2(logptr);
   logptr = NULL;
}


void openTraceFile()
{
   if (logptr == NULL)
   logptr = fopen2("/logtrace", "w");
}


void cleanTrace()
{

  fclose2(logptr);
  fdelete2("/logtrace");
  logptr = fopen2("/logtrace", "w");

}

#endif
