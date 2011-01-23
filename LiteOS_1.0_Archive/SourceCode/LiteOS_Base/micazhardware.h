


#ifndef TOSH_HARDWARE_H
#define TOSH_HARDWARE_H


#include "avrhardware.h"
#include "Types.h"




#define TOSH_CYCLE_TIME_NS 136


void inline TOSH_wait_250ns(void);

void inline TOSH_uwait(int u_sec);

// LED assignments
TOSH_ASSIGN_PIN_H(RED_LED, A, 2);
TOSH_ASSIGN_PIN_H(GREEN_LED, A, 1);
TOSH_ASSIGN_PIN_H(YELLOW_LED, A, 0);

TOSH_ASSIGN_PIN_H(SERIAL_ID, A, 4);
TOSH_ASSIGN_PIN_H(BAT_MON, A, 5);
TOSH_ASSIGN_PIN_H(THERM_PWR, A, 7);

// ChipCon control assignments

#define TOSH_CC_FIFOP_INT SIG_INTERRUPT6
// CC2420 Interrupt definition
#define CC2420_FIFOP_INT_ENABLE()  sbi(EIMSK , INT6)
#define CC2420_FIFOP_INT_DISABLE() cbi(EIMSK , INT6)
#define CC2420_FIFOP_INT_CLEAR() sbi(EIFR, INTF6)
void inline CC2420_FIFOP_INT_MODE(bool LowToHigh);


TOSH_ASSIGN_PIN_H(CC_RSTN, A, 6); // chipcon reset
TOSH_ASSIGN_PIN_H(CC_VREN, A, 5); // chipcon power enable
//TOSH_ASSIGN_PIN_H(CC_FIFOP1, D, 7);  // fifo interrupt
TOSH_ASSIGN_PIN_H(CC_FIFOP, E, 6);  // fifo interrupt
TOSH_ASSIGN_PIN_H(CC_FIFOP1, E, 6);  // fifo interrupt

TOSH_ASSIGN_PIN_H(CC_CCA, D, 6);	  // 
TOSH_ASSIGN_PIN_H(CC_SFD, D, 4);	  // chipcon packet arrival
TOSH_ASSIGN_PIN_H(CC_CS, B, 0);	  // chipcon enable
TOSH_ASSIGN_PIN_H(CC_FIFO, B, 7);	  // chipcon fifo

TOSH_ASSIGN_PIN_H(RADIO_CCA, D, 6);	  // 

// Flash assignments
TOSH_ASSIGN_PIN_H(FLASH_SELECT, A, 3);
TOSH_ASSIGN_PIN_H(FLASH_CLK,  D, 5);
TOSH_ASSIGN_PIN_H(FLASH_OUT,  D, 3);
TOSH_ASSIGN_PIN_H(FLASH_IN,  D, 2);

// interrupt assignments
TOSH_ASSIGN_PIN_H(INT0, E, 4);
TOSH_ASSIGN_PIN_H(INT1, E, 5);
TOSH_ASSIGN_PIN_H(INT2, E, 6);
TOSH_ASSIGN_PIN_H(INT3, E, 7);

// spibus assignments 
TOSH_ASSIGN_PIN_H(MOSI,  B, 2);
TOSH_ASSIGN_PIN_H(MISO,  B, 3);
//TOSH_ASSIGN_PIN_H(SPI_OC1C, B, 7);
TOSH_ASSIGN_PIN_H(SPI_SCK,  B, 1);

// power control assignments
TOSH_ASSIGN_PIN_H(PW0, C, 0);
TOSH_ASSIGN_PIN_H(PW1, C, 1);
TOSH_ASSIGN_PIN_H(PW2, C, 2);
TOSH_ASSIGN_PIN_H(PW3, C, 3);
TOSH_ASSIGN_PIN_H(PW4, C, 4);
TOSH_ASSIGN_PIN_H(PW5, C, 5);
TOSH_ASSIGN_PIN_H(PW6, C, 6);
TOSH_ASSIGN_PIN_H(PW7, C, 7);

// i2c bus assignments
TOSH_ASSIGN_PIN_H(I2C_BUS1_SCL, D, 0);
TOSH_ASSIGN_PIN_H(I2C_BUS1_SDA, D, 1);

// uart assignments
TOSH_ASSIGN_PIN_H(UART_RXD0, E, 0);
TOSH_ASSIGN_PIN_H(UART_TXD0, E, 1);
TOSH_ASSIGN_PIN_H(UART_XCK0, E, 2);
TOSH_ASSIGN_PIN_H(AC_NEG, E, 3);  // RFID Reader Red LED

TOSH_ASSIGN_PIN_H(UART_RXD1, D, 2);
TOSH_ASSIGN_PIN_H(UART_TXD1, D, 3);
TOSH_ASSIGN_PIN_H(UART_XCK1, D, 5);

void TOSH_SET_PIN_DIRECTIONS(void);

enum {
  TOSH_ADC_PORTMAPSIZE = 12
};

enum 
{
//  TOSH_ACTUAL_CC_RSSI_PORT = 0,
  TOSH_ACTUAL_BANDGAP_PORT = 30,  // 1.23 Fixed bandgap reference
  TOSH_ACTUAL_GND_PORT     = 31   // GND 
};



enum 
{
 // TOS_ADC_CC_RSSI_PORT = 0,
  TOS_ADC_BANDGAP_PORT = 10,
  TOS_ADC_GND_PORT     = 11
};


//following needs to be reconsidered

enum {
  TOSH_ACTUAL_PHOTO_PORT = 1,
  TOSH_ACTUAL_TEMP_PORT = 1, 
  TOSH_ACTUAL_MIC_PORT = 2, 
  TOSH_ACTUAL_ACCEL_X_PORT = 3, 
  TOSH_ACTUAL_ACCEL_Y_PORT = 4, 
  TOSH_ACTUAL_MAG_X_PORT = 6, 
  TOSH_ACTUAL_MAG_Y_PORT  = 5
};

enum {
  TOS_ADC_PHOTO_PORT = 1,
  TOS_ADC_TEMP_PORT = 2,
  TOS_ADC_MIC_PORT = 3,
  TOS_ADC_ACCEL_X_PORT = 4,
  TOS_ADC_ACCEL_Y_PORT = 5,
  TOS_ADC_MAG_X_PORT = 6,
  // TOS_ADC_VOLTAGE_PORT = 7,  defined this in hardware.h
  TOS_ADC_MAG_Y_PORT = 8,
};

enum {
  TOS_MAG_POT_ADDR = 0,
  TOS_MIC_POT_ADDR = 1
};

TOSH_ALIAS_PIN_H(TONE_DECODE_SIGNAL, INT3);

TOSH_ALIAS_PIN_H(PHOTO_CTL, INT1);
TOSH_ALIAS_PIN_H(TEMP_CTL, INT2);
TOSH_ALIAS_OUTPUT_ONLY_PIN_H(MIC_CTL, PW3);
TOSH_ALIAS_OUTPUT_ONLY_PIN_H(SOUNDER_CTL, PW2);
TOSH_ALIAS_OUTPUT_ONLY_PIN_H(ACCEL_CTL, PW4);
TOSH_ALIAS_OUTPUT_ONLY_PIN_H(MAG_CTL, PW5);
TOSH_ALIAS_OUTPUT_ONLY_PIN_H(MIC_MUX_SEL, PW6);




#endif //TOSH_HARDWARE_H




