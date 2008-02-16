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



#ifndef PACKETHANDLERH
#define PACKETHANDLERH

#include "amcommon.h"
#include "amradio.h"


//For the receving handles. Whenever a packet is received, content of this vector is compared and executed
enum {
   RECEIVE_HANDLE_NUM = 5, 
};



/** \ingroup radio

     */

/**@{ */

///The definition of a radio handle for user use 
typedef struct radio_handle{
  uint16_t port;
  uint8_t maxLength; 
  uint8_t *dataReady; 
  uint8_t *data; 
  uint8_t *packetinfo; 
  uint8_t handlevalid; 
  void (*handlefunc) (void);  
}radio_handle;



/** \ingroup radio 

      Init the radio handle data structure
      \param NONE
      \return NONE
*/


void initRadioHandle();



/** \ingroup radio 

      Allow the user appliation to register a port by providing necessary info
      \param port The port number the application listening to
      \param maxLength The maximum number of bytes provided in RAM for the incoming packet
      \param dataReady The actual number of bytes will be received. No more than maxLength
      \param packetinfo The packet lqi and rssi
      \param data The actual storage for the data packet
      \param handlefunc The task to be called once the packet comes      
      \return NONE
*/


void registerEvent(uint16_t port, uint8_t maxLength, uint8_t *dataReady, uint8_t *data, uint8_t *packetinfo, void (*handlefunc)(void));



/** \ingroup radio 

      Allow the user appliation to register a port by providing necessary info
      \param port The previous port listening to 
      \return NONE
*/



void deRegisterEvent(uint16_t port);


/* \ingroup radio 

      Called by the Received() which is the called by the coming packet trigger. This is NOT part of the user API
     
*/



Radio_MsgPtr Standard_Receive_Packet(uint16_t port, Radio_MsgPtr packet); 


/** \ingroup radio 

      Provide the handle to the user application so that they can populate this info and call regsiter.
     
*/

void* getHandleInfo();


/** \ingroup radio 

     The register system call 
     
*/


void registerReceiverHandle_syscall();

void deleteThreadRegistrationInReceiverHandles(uint8_t *start, uint8_t *end);

/** @} */

#endif 
