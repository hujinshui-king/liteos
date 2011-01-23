#include "leds.h"
#include "system.h"
#include "liteoscommon.h"
#include "thread.h"
#include "radio.h"
#include "stringutil.h"
#include "file.h"
#include "adc.h"

 MYFILE *fileptr;

 uint16_t samples; 
 uint16_t reading; 
 uint16_t sleeptime; 
 uint16_t type; 

 uint8_t EnableSampling; 
 
 uint8_t fileinit; 

int main()
{ 



 samples = 10;
 sleeptime = 100;
 type = 1; 
 fileinit = 0; 
 EnableSampling = 0;   

 
 

 while (1)
 {
 
 if (EnableSampling == 0)
 	{
 
   sleepThread(2000);
   fileinit = 0; 
   
  }
  
  else
  {
  	
   if (fileinit == 0)
   	{
   	  fileinit = 1; 
   	  fileptr = mfopen("/deposit", "w");	   		
   	}
   	
   if (samples >0)
  {
   samples--; 
  
    
   sleepThread(sleeptime);

   if (type == 1)
   reading = get_light();
   else if (type == 2)
   reading = get_temp();
   else
   reading = 0; 

   redToggle(); 

   {{

 
    char _tempbuffer[32];
    char *_temp1 = "The reading is ";
    char *_temp2 = String_intToString(reading);
    char *_temp3 = "\n";
	  uint8_t lengthstring;
    _tempbuffer[0] = '\0';
    String_append(_tempbuffer, _temp1);
    String_append(_tempbuffer, _temp2);
    String_append(_tempbuffer, _temp3);

    lengthstring = String_length(_tempbuffer); 

    mfwrite(fileptr, _tempbuffer, lengthstring);
  	mfseek(fileptr, lengthstring, 1);

    }}
    
    if (samples == 0)
    	mfclose(fileptr); 
   }    
  }
}
return 0; 
}

