#ifndef AMH
#define AMH

#include "Types.h"


/*This file contains the definitions of the radio am.h file*/


enum  {
  BCAST_ADDRESS = 0xffff, 
};


//This number should ensure that the total length is no longer than 128. 
//100 works 

#ifndef PAYLOAD_LENGTH
#define PAYLOAD_LENGTH 100
#endif

//return the relative address of member in a type, which is usually a struct 

#define offsetof(type, member) ((size_t) &((type *)0)->member)


//The message format is specified by the standard 802.15.4 by IEEE (version 2003). check the library for the file 

typedef struct Radio_Msg{
 
 //length is not part of the header according to the format 
  uint8_t length;

 //802.15.4 definitions 
  uint8_t fcfhi;
  uint8_t fcflo;
  uint8_t dsn;
  uint16_t destpan;
  uint16_t addr;

  //frame payload 
  //  uint8_t type;
  //  uint8_t group;
  uint16_t port; 
  int8_t data[PAYLOAD_LENGTH];

 //remaining part of the radio message
  uint8_t strength;
  uint8_t lqi;
  bool crc;
  uint8_t ack;
  uint16_t time;
} Radio_Msg;



enum {
  // size of the header NOT including the length byte
  MSG_HEADER_SIZE = offsetof(struct Radio_Msg, data) - 1,	  //(2+1+2+2)+(TOSHeader=5)=11

  // size of the footer
  MSG_FOOTER_SIZE = 2,

  // size of the full packet-including length byte
  MSG_DATA_SIZE = offsetof(struct Radio_Msg, strength) + sizeof(uint16_t), //1+7+5+29+1+2
  
  // size of the data length
  DATA_LENGTH = PAYLOAD_LENGTH,
  
  // position of the length byte
  LENGTH_BYTE_NUMBER = offsetof(struct Radio_Msg, length) + 1,

  // size of MAC Header 
  TOS_HEADER_SIZE = 5, 	   //TOSHeader=addr(2)+type(1)+groupid(1)+length(1)
};



typedef Radio_Msg *Radio_MsgPtr;


#endif

