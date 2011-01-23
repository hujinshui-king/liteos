#ifndef PACKETHANDLERH
#define PACKETHANDLERH
#include "amcommon.h"
#include "amradio.h"
//For the receving handles. Whenever a packet is received, content of this vector is compared and executed



enum
{
    RECEIVE_HANDLE_NUM = 5,
};

/** \ingroup radio
*/

 typedef struct 
{
    uint16_t port;
    uint8_t maxLength;
    uint8_t *dataReady;
    uint8_t *data;
    uint8_t *packetinfo;
    uint8_t handlevalid;
    void (*handlefunc) (void);
} radio_receiving_buffer;



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
void registerEvent(uint16_t port, uint8_t maxLength, uint8_t * dataReady,
                   uint8_t * data, uint8_t * packetinfo,
                   void (*handlefunc) (void));

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


void deleteThreadRegistrationInReceiverHandles(uint8_t * start, uint8_t * end);

/** @} */
#endif
