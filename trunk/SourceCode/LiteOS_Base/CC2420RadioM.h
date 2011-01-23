#ifndef CC2420RADIOMH
#define CC2420RADIOMH

#include "amcommon.h"
#include "Types.h"


inline result_t CC2420RadioM_SplitControl_default_initDone(void);
inline result_t CC2420RadioM_CC2420SplitControl_initDone(void);
inline result_t CC2420RadioM_SplitControl_init(void);
inline result_t CC2420RadioM_CC2420SplitControl_start(void);
inline result_t CC2420RadioM_SplitControl_start(void);
inline void     CC2420RadioM_startRadio(void);
inline result_t CC2420RadioM_StdControl_start(void);
inline result_t CC2420RadioM_SFD_enableCapture(bool arg_0xa41e260);
inline uint8_t CC2420RadioM_HPLChipcon_cmd(uint8_t arg_0xa403928);
inline void CC2420RadioM_sendPacket(void);
inline void CC2420RadioM_tryToSend(void);
inline result_t CC2420RadioM_HPLChipconFIFO_TXFIFODone(uint8_t length, uint8_t *data);
inline result_t CC2420RadioM_HPLChipconFIFO_writeTXFIFO(uint8_t arg_0xa40c010, uint8_t *arg_0xa40c170);
inline void CC2420RadioM_startSend(void);
inline result_t CC2420RadioM_Send_send(Radio_MsgPtr pMsg);
inline result_t CC2420RadioM_SplitControl_default_startDone(void);
inline result_t CC2420RadioM_FIFOP_startWait(bool arg_0xa422588);
inline result_t CC2420RadioM_CC2420Control_RxMode(void);
inline result_t CC2420RadioM_CC2420SplitControl_startDone(void);
inline uint16_t CC2420RadioM_HPLChipcon_read(uint8_t arg_0xa4103b0);
inline result_t CC2420RadioM_FIFOP_disable(void);
inline void CC2420RadioM_delayedRXFIFOtask(void);
inline result_t CC2420RadioM_FIFOP_fired(void);
inline Radio_MsgPtr CC2420RadioM_Receive_receive(Radio_MsgPtr arg_0xa31b650);
inline void CC2420RadioM_PacketRcvd(void);
inline result_t CC2420RadioM_HPLChipconFIFO_RXFIFODone(uint8_t length, uint8_t *data);
inline result_t CC2420RadioM_HPLChipconFIFO_readRXFIFO(uint8_t arg_0xa4118a0, uint8_t *arg_0xa411a00);
inline result_t CC2420RadioM_Send_sendDone(Radio_MsgPtr arg_0xa3c3710, result_t arg_0xa3c3860);
inline void CC2420RadioM_PacketSent(void);
inline result_t CC2420RadioM_SFD_disable(void);
inline result_t CC2420RadioM_SFD_captured(uint16_t time);
inline void CC2420RadioM_sendFailed(void);
inline void CC2420RadioM_flushRXFIFO(void);
inline void CC2420RadioM_delayedRXFIFO(void);
inline   result_t CC2420RadioM_SplitControl_initDone(void);


#endif
