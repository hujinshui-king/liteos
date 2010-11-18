#ifndef COMMANDPROCESSORH
#define COMMANDPROCESSORH

#include "Types.h"
#include "amcommon.h"


void broadcastCommand(uint8_t *receivebuffer, uint8_t length); 

Radio_MsgPtr Broadcast2Serial(Radio_MsgPtr msg); 

Radio_MsgPtr Broadcast2SerialAlternative(Radio_MsgPtr msg);

#endif
