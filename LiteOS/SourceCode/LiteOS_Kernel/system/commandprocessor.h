#ifndef COMMANDPROCESSORH
#define COMMANDPROCESSORH

#include "../types/types.h"
#include "amcommon.h"

void broadcastCommand(uint8_t * receivebuffer, uint8_t length);
Radio_MsgPtr Broadcast2SerialAlternative(Radio_MsgPtr msg);

#endif
