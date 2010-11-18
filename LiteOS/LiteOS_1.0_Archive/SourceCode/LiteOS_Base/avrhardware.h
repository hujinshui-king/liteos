#ifndef TOSH_AVRHARDWARE_H
#define TOSH_AVRHARDWARE_H


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/iom128.h>
#include "Types.h"



#ifndef sbi

#define sbi(port, bit) ((port) |= _BV(bit))
#define cbi(port, bit) ((port) &= ~_BV(bit))
#define inp(port) (port)
#define inb(port) (port)
#define outp(value, port) ((port) = (value))
#define outb(port, value) ((port) = (value))
#define inw(port) (*(volatile uint16_t *)&(port))
#define outw(port, value) ((*(volatile uint16_t *)&(port)) = (value))
#define PRG_RDB(addr) pgm_read_byte(addr)
#endif



#if ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 3))
#define __outw(val, port) outw(port, val);
#endif

#ifndef __inw
#ifndef __SFR_OFFSET
#define __SFR_OFFSET 0
#endif /* !__SFR_OFFSET */

#define __inw(_port) inw(_port)
#define __inw_atomic(__sfrport) ({				\
	uint16_t __t;					\
	bool bStatus;					\
	bStatus = bit_is_set(SREG,7);			\
	cli();						\
	__t = inw(__sfrport);				\
	if (bStatus) sei();				\
	__t;						\
 })

#endif /* __inw */


#define TOSH_ASSIGN_PIN_H(name, port, bit) \
inline void TOSH_SET_##name##_PIN(void); \
inline void TOSH_CLR_##name##_PIN(void); \
inline int TOSH_READ_##name##_PIN(void); \
inline void TOSH_MAKE_##name##_OUTPUT(void); \
inline void TOSH_MAKE_##name##_INPUT(void); 

#define TOSH_ASSIGN_OUTPUT_ONLY_PIN_H(name, port, bit) \
inline void TOSH_SET_##name##_PIN(void); \
inline void TOSH_CLR_##name##_PIN(void); \
inline void TOSH_MAKE_##name##_OUTPUT(void); 

#define TOSH_ALIAS_OUTPUT_ONLY_PIN_H(alias, connector)\
inline void TOSH_SET_##alias##_PIN(void); \
inline void TOSH_CLR_##alias##_PIN(void); \
inline void TOSH_MAKE_##alias##_OUTPUT(void); \

#define TOSH_ALIAS_PIN_H(alias, connector) \
inline void TOSH_SET_##alias##_PIN(void); \
inline void TOSH_CLR_##alias##_PIN(void);  \
inline char TOSH_READ_##alias##_PIN(void);  \
inline void TOSH_MAKE_##alias##_OUTPUT(void);  \
inline void TOSH_MAKE_##alias##_INPUT(void);  


#define TOSH_ASSIGN_PIN(name, port, bit) \
inline void TOSH_SET_##name##_PIN() {sbi(PORT##port , bit);} \
inline void TOSH_CLR_##name##_PIN() {cbi(PORT##port , bit);} \
inline int TOSH_READ_##name##_PIN() \
  {return (inp(PIN##port) & (1 << bit)) != 0;} \
inline void TOSH_MAKE_##name##_OUTPUT() {sbi(DDR##port , bit);} \
inline void TOSH_MAKE_##name##_INPUT() {cbi(DDR##port , bit);} 

#define TOSH_ASSIGN_OUTPUT_ONLY_PIN(name, port, bit) \
inline void TOSH_SET_##name##_PIN() {sbi(PORT##port , bit);} \
inline void TOSH_CLR_##name##_PIN() {cbi(PORT##port , bit);} \
inline void TOSH_MAKE_##name##_OUTPUT() {;} 

#define TOSH_ALIAS_OUTPUT_ONLY_PIN(alias, connector)\
inline void TOSH_SET_##alias##_PIN() {TOSH_SET_##connector##_PIN();} \
inline void TOSH_CLR_##alias##_PIN() {TOSH_CLR_##connector##_PIN();} \
inline void TOSH_MAKE_##alias##_OUTPUT() {} \

#define TOSH_ALIAS_PIN(alias, connector) \
inline void TOSH_SET_##alias##_PIN() {TOSH_SET_##connector##_PIN();} \
inline void TOSH_CLR_##alias##_PIN() {TOSH_CLR_##connector##_PIN();} \
inline char TOSH_READ_##alias##_PIN() {return TOSH_READ_##connector##_PIN();} \
inline void TOSH_MAKE_##alias##_OUTPUT() {TOSH_MAKE_##connector##_OUTPUT();} \
inline void TOSH_MAKE_##alias##_INPUT()  {TOSH_MAKE_##connector##_INPUT();} 

// We need slightly different defs than SIGNAL, INTERRUPT
#define TOSH_SIGNAL(signame)					\
void signame() __attribute__ ((signal, spontaneous, C/*, used,   externally_visible)*/))

#define _INTERRUPT(signame)				\
void   signame(void) __attribute__((interrupt/*, used,   externally_visible*/)); \
void   signame(void)

/* Watchdog Prescaler
 */
enum {
  TOSH_period16 = 0x00, // 47ms
  TOSH_period32 = 0x01, // 94ms
  TOSH_period64 = 0x02, // 0.19s
  TOSH_period128 = 0x03, // 0.38s
  TOSH_period256 = 0x04, // 0.75s
  TOSH_period512 = 0x05, // 1.5s
  TOSH_period1024 = 0x06, // 3.0s
  TOSH_period2048 = 0x07 // 6.0s
};

void TOSH_wait(void);

// atomic statement runtime support

typedef uint8_t _atomic_t;

inline _atomic_t _atomic_start(void);
inline void _atomic_end(_atomic_t oldSreg);






inline void _atomic_sleep(void);

inline void __nesc_enable_interrupt(void);

inline void __nesc_disable_interrupt(void);
#endif //TOSH_AVRHARDWARE_H

