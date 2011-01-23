#ifndef CC2420CONTROLMH
#define CC2420CONTROLMH


#include "amcommon.h"
#include "Types.h"

enum CC2420ControlM___nesc_unnamed4270 {
  CC2420ControlM_IDLE_STATE = 0, 
  CC2420ControlM_INIT_STATE, 
  CC2420ControlM_INIT_STATE_DONE, 
  CC2420ControlM_START_STATE, 
  CC2420ControlM_START_STATE_DONE, 
  CC2420ControlM_STOP_STATE
};


 
 
inline result_t CC2420ControlM_SplitControl_init(void);
inline    result_t CC2420ControlM_CCA_startWait(bool arg_0xa422588);
inline result_t CC2420ControlM_CC2420Control_VREFOn(void);
inline   result_t CC2420ControlM_SplitControl_initDone(void);
inline void CC2420ControlM_taskInitDone(void);
inline    uint8_t CC2420ControlM_HPLChipcon_cmd(uint8_t arg_0xa403928);
inline    uint8_t CC2420ControlM_HPLChipcon_write(uint8_t arg_0xa403d80, uint16_t arg_0xa403ed0);
inline result_t CC2420ControlM_CC2420Control_OscillatorOn(void);
inline   result_t CC2420ControlM_HPLChipconControl_start(void);
inline result_t CC2420ControlM_SplitControl_start(void);
inline result_t CC2420ControlM_CC2420Control_RxMode(void);
inline   result_t CC2420ControlM_SplitControl_startDone(void);

inline    result_t CC2420ControlM_HPLChipconRAM_write(uint16_t arg_0xa45ad38, uint8_t arg_0xa45ae80, uint8_t *arg_0xa45afe0);
inline result_t CC2420ControlM_CC2420Control_setShortAddress(uint16_t addr);
inline    uint16_t CC2420ControlM_HPLChipcon_read(uint8_t arg_0xa4103b0);
inline bool CC2420ControlM_SetRegs(void);
inline void CC2420ControlM_PostOscillatorOn(void);
inline result_t CC2420ControlM_CCA_fired(void);
inline result_t CC2420ControlM_HPLChipconRAM_writeDone(uint16_t addr, uint8_t length, uint8_t *buffer);
inline   result_t CC2420ControlM_HPLChipconControl_init(void);

inline  result_t CC2420ControlM_CC2420Control_VREFOff(void); 
inline result_t CC2420ControlM_CC2420Control_TuneManual(uint16_t DesiredFreq);
inline 
result_t CC2420ControlM_CC2420Control_TunePower( uint8_t powerlevel ); 
inline result_t CC2420ControlM_CC2420Control_TuneChannel( uint8_t channel ); 
#endif

