#include "leds.h"
#include "system.h"
#include "liteoscommon.h"
#include "thread.h"
#include "radio.h"
#include "stringutil.h"
#include "file.h"
#include "adc.h"

 MYFILE *fileptr;
 MYFILE  *configptr; 
 uint16_t samples; 
 uint16_t reading; 
 uint16_t sleeptime; 
 uint16_t type; 



int main()
{ 

 __asm__ __volatile__("sei" ::); 
 fileptr = mfopen("/file1", "w");
 samples = 50; 
 sleeptime = 100; 

 while (1)
 {
  if (samples >0)
   samples--; 
   else
   break; 
    
   sleepThread(sleeptime);

   redToggle(); 

   {{

    char _tempbuffer[32];
    char *_temp1 = "Hello ";
    char *_temp3 = "World\n";
	  uint8_t lengthstring;
    
    _tempbuffer[0] = '\0';
    String_append(_tempbuffer, _temp1);
    String_append(_tempbuffer, _temp3);

    lengthstring = String_length(_tempbuffer); 

    mfwrite(fileptr, _tempbuffer, lengthstring);
  	mfseek(fileptr, lengthstring, 1);

    }}
   
}


mfclose(fileptr);

return 0; 

}

