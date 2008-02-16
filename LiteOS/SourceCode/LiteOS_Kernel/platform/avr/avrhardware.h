/* LiteOS Version 0.3 */
/*
The following is the license of LiteOS.

This file is part of LiteOS.
Copyright Qing Cao, 2007-2008, University of Illinois , qcao2@uiuc.edu

LiteOS is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

LiteOS is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with LiteOS.  If not, see <http://www.gnu.org/licenses/>.
*/

/*									tab:4
 * "Copyright (c) 2000-2003 The Regents of the University  of California.  
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice, the following
 * two paragraphs and the author appear in all copies of this software.
 * 
 * IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF
 * CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
 *
 * Copyright (c) 2002-2003 Intel Corporation
 * All rights reserved.
 *
 * This file is distributed under the terms in the attached INTEL-LICENSE     
 * file. If you do not find these files, copies can be found by writing to
 * Intel Research Berkeley, 2150 Shattuck Avenue, Suite 1300, Berkeley, CA, 
 * 94704.  Attention:  Intel License Inquiry.
 */
/*
 *
 * Authors:             Jason Hill, Philip Levis, Nelson Lee
 *
 *
 */

/**
 * @author Jason Hill
 * @author Philip Levis
 * @author Nelson Lee
 */



#ifndef AVRHARDWAREH
#define AVRHARDWAREH


#include "../../types/types.h"  

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
//#include <avr/iom1281.h>
#include <avr/iom128.h>
//iom1281.h







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


#define LITE_ASSIGN_PIN_H(name, port, bit) \
inline void LITE_SET_##name##_PIN(void); \
inline void LITE_CLR_##name##_PIN(void); \
inline int LITE_READ_##name##_PIN(void); \
inline void LITE_MAKE_##name##_OUTPUT(void); \
inline void LITE_MAKE_##name##_INPUT(void); 

#define LITE_ASSIGN_OUTPUT_ONLY_PIN_H(name, port, bit) \
inline void LITE_SET_##name##_PIN(void); \
inline void LITE_CLR_##name##_PIN(void); \
inline void LITE_MAKE_##name##_OUTPUT(void); 

#define LITE_ALIAS_OUTPUT_ONLY_PIN_H(alias, connector)\
inline void LITE_SET_##alias##_PIN(void); \
inline void LITE_CLR_##alias##_PIN(void); \
inline void LITE_MAKE_##alias##_OUTPUT(void); \

#define LITE_ALIAS_PIN_H(alias, connector) \
inline void LITE_SET_##alias##_PIN(void); \
inline void LITE_CLR_##alias##_PIN(void);  \
inline char LITE_READ_##alias##_PIN(void);  \
inline void LITE_MAKE_##alias##_OUTPUT(void);  \
inline void LITE_MAKE_##alias##_INPUT(void);  


#define LITE_ASSIGN_PIN(name, port, bit) \
inline void LITE_SET_##name##_PIN() {sbi(PORT##port , bit);} \
inline void LITE_CLR_##name##_PIN() {cbi(PORT##port , bit);} \
inline int LITE_READ_##name##_PIN() \
  {return (inp(PIN##port) & (1 << bit)) != 0;} \
inline void LITE_MAKE_##name##_OUTPUT() {sbi(DDR##port , bit);} \
inline void LITE_MAKE_##name##_INPUT() {cbi(DDR##port , bit);} 

#define LITE_ASSIGN_OUTPUT_ONLY_PIN(name, port, bit) \
inline void LITE_SET_##name##_PIN() {sbi(PORT##port , bit);} \
inline void LITE_CLR_##name##_PIN() {cbi(PORT##port , bit);} \
inline void LITE_MAKE_##name##_OUTPUT() {;} 

#define LITE_ALIAS_OUTPUT_ONLY_PIN(alias, connector)\
inline void LITE_SET_##alias##_PIN() {LITE_SET_##connector##_PIN();} \
inline void LITE_CLR_##alias##_PIN() {LITE_CLR_##connector##_PIN();} \
inline void LITE_MAKE_##alias##_OUTPUT() {} \

#define LITE_ALIAS_PIN(alias, connector) \
inline void LITE_SET_##alias##_PIN() {LITE_SET_##connector##_PIN();} \
inline void LITE_CLR_##alias##_PIN() {LITE_CLR_##connector##_PIN();} \
inline char LITE_READ_##alias##_PIN() {return LITE_READ_##connector##_PIN();} \
inline void LITE_MAKE_##alias##_OUTPUT() {LITE_MAKE_##connector##_OUTPUT();} \
inline void LITE_MAKE_##alias##_INPUT()  {LITE_MAKE_##connector##_INPUT();} 

// We need slightly different defs than SIGNAL, INTERRUPT
#define LITE_SIGNAL(signame)					\
void signame() __attribute__ ((signal, spontaneous, C/*, used,   externally_visible)*/))

#define _INTERRUPT(signame)				\
void   signame(void) __attribute__((interrupt/*, used,   externally_visible*/)); \
void   signame(void)

/** Watchdog Prescaler
 */
enum {
  LITE_period16 = 0x00, // 47ms
  LITE_period32 = 0x01, // 94ms
  LITE_period64 = 0x02, // 0.19s
  LITE_period128 = 0x03, // 0.38s
  LITE_period256 = 0x04, // 0.75s
  LITE_period512 = 0x05, // 1.5s
  LITE_period1024 = 0x06, // 3.0s
  LITE_period2048 = 0x07 // 6.0s
};

void wait_cycle(void);

/// atomic statement runtime support


/** \defgroup avrhardware AVR hardware related definitions
      These functions and definitions collectively form the avr hardware definitions */


/** @{ */


inline _atomic_t _atomic_start_avr(void);

inline void _atomic_end_avr(_atomic_t oldSreg);

inline void _atomic_sleep(void);

inline void _avr_sleep(); 

inline void _avr_enable_interrupt(void);

inline void _avr_disable_interrupt(void);

void avr_resetNode();



#define PUSH_GPR()        \
  __asm__("push r0");     \
  __asm__("push r1");     \
  __asm__("push r2");     \
  __asm__("push r3");     \
  __asm__("push r4");     \
  __asm__("push r5");     \
  __asm__("push r6");     \
  __asm__("push r7");     \
  __asm__("push r8");     \
  __asm__("push r9");     \
  __asm__("push r10");    \
  __asm__("push r11");    \
  __asm__("push r12");    \
  __asm__("push r13");    \
  __asm__("push r14");    \
  __asm__("push r15");    \
  __asm__("push r16");    \
  __asm__("push r17");    \
  __asm__("push r18");    \
  __asm__("push r19");    \
  __asm__("push r20");    \
  __asm__("push r21");    \
  __asm__("push r22");    \
  __asm__("push r23");    \
  __asm__("push r24");    \
  __asm__("push r25");    \
  __asm__("push r26");    \
  __asm__("push r27");    \
  __asm__("push r28");    \
  __asm__("push r29");    \
  __asm__("push r30");    \
  __asm__("push r31")


#define PUSH_PC()          \
  __asm__("push r0")


#define PUSH_REG_STATUS()         \
  __asm__("push r31");        \
  __asm__("in r31,__SREG__"); \
  __asm__("push r31")


#define POP_PC()            \
  __asm__("pop r0")


#define POP_REG_STATUS()          \
  __asm__("pop r31");         \
  __asm__("out __SREG__,r31");\
  __asm__("pop r31")


//Pop the general purpose registers
#define POP_GPR()         \
  __asm__("pop r31");     \
  __asm__("pop r30");     \
  __asm__("pop r29");     \
  __asm__("pop r28");     \
  __asm__("pop r27");     \
  __asm__("pop r26");     \
  __asm__("pop r25");     \
  __asm__("pop r24");     \
  __asm__("pop r23");     \
  __asm__("pop r22");     \
  __asm__("pop r21");     \
  __asm__("pop r20");     \
  __asm__("pop r19");     \
  __asm__("pop r18");     \
  __asm__("pop r17");     \
  __asm__("pop r16");     \
  __asm__("pop r15");     \
  __asm__("pop r14");     \
  __asm__("pop r13");     \
  __asm__("pop r12");     \
  __asm__("pop r11");     \
  __asm__("pop r10");     \
  __asm__("pop r9");      \
  __asm__("pop r8");      \
  __asm__("pop r7");      \
  __asm__("pop r6");      \
  __asm__("pop r5");      \
  __asm__("pop r4");      \
  __asm__("pop r3");      \
  __asm__("pop r2");      \
  __asm__("pop r1");      \
  __asm__("pop r0")



#define SWAP_STACK_PTR(OLD, NEW) \
  __asm__("in %A0, __SP_L__\n\t in %B0, __SP_H__":"=r"(OLD):);\
  __asm__("out __SP_H__,%B0\n\t out __SP_L__,%A0"::"r"(NEW))
 

#define PREPARE_REG_FOR_STACK()                               \
  SWAP_STACK_PTR(old_stack_ptr, current_thread->sp);  \
  __asm__("push %A0\n push %B0"::"r"(thread_func_dispatcher));\
  for(i=0;i<36;i++)                                   \
    __asm__("push __zero_reg__");                     \
  SWAP_STACK_PTR(current_thread->sp, old_stack_ptr)


/**@}*/
#endif //LITE_AVRHARDWARE_H

