#ifndef HPLCC2420INTERRUPTMH
#define HPLCC2420INTERRUPTMH
#include "Types.h"
#include "amcommon.h"


inline result_t HPLCC2420InterruptM_CCA_startWait(bool low_to_high);
inline    void HPLCC2420InterruptM_SFDCapture_enableEvents(void);
inline    void HPLCC2420InterruptM_SFDCapture_clearOverflow(void);
inline    void HPLCC2420InterruptM_SFDCapture_setEdge(uint8_t arg_0xa4c5e80);
inline    void HPLCC2420InterruptM_SFDCapture_disableEvents(void);
inline result_t HPLCC2420InterruptM_SFD_enableCapture(bool low_to_high);
inline result_t HPLCC2420InterruptM_FIFOP_startWait(bool low_to_high);
inline    result_t HPLCC2420InterruptM_CCA_fired(void);
inline result_t HPLCC2420InterruptM_CCATimer_fired(void);
inline   result_t HPLCC2420InterruptM_FIFOTimer_start(char arg_0xa315138, uint32_t arg_0xa315290);
inline result_t HPLCC2420InterruptM_FIFO_default_fired(void);
inline    result_t HPLCC2420InterruptM_FIFO_fired(void);
inline result_t HPLCC2420InterruptM_FIFOTimer_fired(void);
inline result_t HPLCC2420InterruptM_FIFOP_disable(void);
inline result_t HPLCC2420InterruptM_FIFOP_fired(void);
inline result_t HPLCC2420InterruptM_SFD_disable(void);
inline    result_t HPLCC2420InterruptM_SFD_captured(uint16_t arg_0xa41e788);
inline    bool HPLCC2420InterruptM_SFDCapture_isOverflowPending(void);
inline void HPLCC2420InterruptM_SFDCapture_captured(uint16_t time);
inline   result_t HPLCC2420InterruptM_CCATimer_start(char arg_0xa315138, uint32_t arg_0xa315290);
#endif 

