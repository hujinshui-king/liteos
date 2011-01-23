#ifndef PACKETHANDLERH
#define PACKETHANDLERH

#include "amcommon.h"
#include "Amradio.h"


void initRadioHandle();
void registerEvent(uint16_t port, uint8_t maxLength, uint8_t *dataReady, uint8_t *data, uint8_t *packetinfo, void (*handlefunc)(void));
void deRegisterEvent(uint16_t port);
Radio_MsgPtr Standard_Receive_Packet(uint16_t port, Radio_MsgPtr packet); 



#endif 
