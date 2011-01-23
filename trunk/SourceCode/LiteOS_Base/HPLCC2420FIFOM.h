#ifndef HPLCC2420FIFOMH
#define HPLCC2420FIFOMH
#include "Types.h"

#include "amcommon.h"


inline result_t HPLCC2420FIFOM_HPLCC2420FIFO_TXFIFODone(uint8_t arg_0xa40cd20, uint8_t *arg_0xa40ce80);
inline void HPLCC2420FIFOM_signalTXdone(void);
inline result_t HPLCC2420FIFOM_HPLCC2420FIFO_writeTXFIFO(uint8_t len, uint8_t *msg);
inline    result_t HPLCC2420FIFOM_HPLCC2420FIFO_RXFIFODone(uint8_t arg_0xa40c690, uint8_t *arg_0xa40c7f0);
inline result_t HPLCC2420FIFOM_HPLCC2420FIFO_readRXFIFO(uint8_t len, uint8_t *msg);



#endif

