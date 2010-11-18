#ifndef TIMERH
#define TIMERH

#include "Types.h"
#include "threads.h"




enum __nesc_unnamed4259 {
  TIMER_REPEAT = 0, 
  TIMER_ONE_SHOT = 1, 
  NUM_TIMERS = 16
};

enum __nesc_unnamed4261 {
  DEFAULT_SCALE = 3, DEFAULT_INTERVAL = 127
};


inline  
result_t TimerM_StdControl_init(void);



inline  
result_t TimerM_Timer_start(uint8_t id, char type, 
uint32_t interval);



inline result_t TimerM_Clock_setRate(char arg_0xa369e18, char arg_0xa369f58);



void TimerM_adjustInterval(void);

inline 
void TimerM_enqueue(uint8_t value);


inline   
result_t TimerM_Timer_default_fired(uint8_t id);


inline result_t TimerM_Timer_fired(uint8_t arg_0xa3482e0);


inline 
uint8_t TimerM_dequeue(void);


inline  void TimerM_signalOneTimer(void);

inline  
void TimerM_HandleFire(void);

inline result_t TimerM_Clock_fire(void);


inline  
result_t TimerM_StdControl_start(void);

inline uint8_t TimerM_Clock_readCounter(void);

inline uint8_t TimerM_Clock_getInterval(void);

inline void TimerM_Clock_setInterval(uint8_t arg_0xa362928);

#endif

