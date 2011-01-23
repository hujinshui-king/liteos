#ifndef SOCKETH
#define SOCKETH

#include "Types.h"
#include "amcommon.h"
#include "scheduling.h"
#include "thread_data.h"
#include "amradio.h"
#include "threads.h"


int SocketInit();

void send_task();

void SocketRadioSend();

void SocketSendDone(uint8_t id, Radio_MsgPtr msg, result_t success);

void send_taskdebug();

void SocketRadioReceive(uint16_t port, Radio_MsgPtr msg);

#endif 
