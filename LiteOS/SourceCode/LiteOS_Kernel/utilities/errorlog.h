#ifndef ERRORLOGH
#define ERRORLOGH


/* \defgroup debug Debugging Utilities

  These definitons collectively form kernel support for output information as it runs. The system uses preprocessor to reduce the system overhead. To use, follow
  the following example. The printf function sends the data to the standard output, usually serial output on MicaZ, for example 

  \code
     #ifdef AMRADIO_FUNCTIONA
          printfstr("This is trace point A at AM RADIO Function A"); 
     #endif
  \endcode

  
*/




//Radio stack output debugging information

#define DEUBG_RADIO_ON 


#ifdef DEBUG_RADIO_ON
         //The radio moduel debugging output is turned on 
         #define AMRADIO_FUNCTIONA   
#else
         //The radio module debugging output is turned off 
#endif






#endif
