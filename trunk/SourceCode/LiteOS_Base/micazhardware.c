#include "micazhardware.h"
void inline TOSH_wait_250ns() {
      asm volatile  ("nop" ::);
      asm volatile  ("nop" ::);
}

void inline TOSH_uwait(int u_sec) {
    while (u_sec > 0) {
      asm volatile  ("nop" ::);
      asm volatile  ("nop" ::);
      asm volatile  ("nop" ::);
      asm volatile  ("nop" ::);
      asm volatile  ("nop" ::);
      asm volatile  ("nop" ::);
      asm volatile  ("nop" ::);
      asm volatile  ("nop" ::);
      u_sec--;
    }
}


// LED assignments
TOSH_ASSIGN_PIN(RED_LED, A, 2);
TOSH_ASSIGN_PIN(GREEN_LED, A, 1);
TOSH_ASSIGN_PIN(YELLOW_LED, A, 0);

TOSH_ASSIGN_PIN(SERIAL_ID, A, 4);
TOSH_ASSIGN_PIN(BAT_MON, A, 5);
TOSH_ASSIGN_PIN(THERM_PWR, A, 7);




void inline CC2420_FIFOP_INT_MODE(bool LowToHigh) {
        sbi(EICRB,ISC61);				 // edge mode
        if( LowToHigh)
	        sbi(EICRB,ISC60);              //trigger on rising level
        else
	        cbi(EICRB,ISC60);              //trigger on falling level
}



TOSH_ASSIGN_PIN(CC_RSTN, A, 6); // chipcon reset
TOSH_ASSIGN_PIN(CC_VREN, A, 5); // chipcon power enable
//TOSH_ASSIGN_PIN(CC_FIFOP1, D, 7);  // fifo interrupt
TOSH_ASSIGN_PIN(CC_FIFOP, E, 6);  // fifo interrupt
TOSH_ASSIGN_PIN(CC_FIFOP1, E, 6);  // fifo interrupt

TOSH_ASSIGN_PIN(CC_CCA, D, 6);	  // 
TOSH_ASSIGN_PIN(CC_SFD, D, 4);	  // chipcon packet arrival
TOSH_ASSIGN_PIN(CC_CS, B, 0);	  // chipcon enable
TOSH_ASSIGN_PIN(CC_FIFO, B, 7);	  // chipcon fifo

TOSH_ASSIGN_PIN(RADIO_CCA, D, 6);	  // 

// Flash assignments
TOSH_ASSIGN_PIN(FLASH_SELECT, A, 3);
TOSH_ASSIGN_PIN(FLASH_CLK,  D, 5);
TOSH_ASSIGN_PIN(FLASH_OUT,  D, 3);
TOSH_ASSIGN_PIN(FLASH_IN,  D, 2);

// interrupt assignments
TOSH_ASSIGN_PIN(INT0, E, 4);
TOSH_ASSIGN_PIN(INT1, E, 5);
TOSH_ASSIGN_PIN(INT2, E, 6);
TOSH_ASSIGN_PIN(INT3, E, 7);

// spibus assignments 
TOSH_ASSIGN_PIN(MOSI,  B, 2);
TOSH_ASSIGN_PIN(MISO,  B, 3);
//TOSH_ASSIGN_PIN(SPI_OC1C, B, 7);
TOSH_ASSIGN_PIN(SPI_SCK,  B, 1);

// power control assignments
TOSH_ASSIGN_PIN(PW0, C, 0);
TOSH_ASSIGN_PIN(PW1, C, 1);
TOSH_ASSIGN_PIN(PW2, C, 2);
TOSH_ASSIGN_PIN(PW3, C, 3);
TOSH_ASSIGN_PIN(PW4, C, 4);
TOSH_ASSIGN_PIN(PW5, C, 5);
TOSH_ASSIGN_PIN(PW6, C, 6);
TOSH_ASSIGN_PIN(PW7, C, 7);

// i2c bus assignments
TOSH_ASSIGN_PIN(I2C_BUS1_SCL, D, 0);
TOSH_ASSIGN_PIN(I2C_BUS1_SDA, D, 1);

// uart assignments
TOSH_ASSIGN_PIN(UART_RXD0, E, 0);
TOSH_ASSIGN_PIN(UART_TXD0, E, 1);
TOSH_ASSIGN_PIN(UART_XCK0, E, 2);
TOSH_ASSIGN_PIN(AC_NEG, E, 3);  // RFID Reader Red LED

TOSH_ASSIGN_PIN(UART_RXD1, D, 2);
TOSH_ASSIGN_PIN(UART_TXD1, D, 3);
TOSH_ASSIGN_PIN(UART_XCK1, D, 5);


TOSH_ALIAS_PIN(TONE_DECODE_SIGNAL, INT3);

TOSH_ALIAS_PIN(PHOTO_CTL, INT1);
TOSH_ALIAS_PIN(TEMP_CTL, INT2);
TOSH_ALIAS_OUTPUT_ONLY_PIN(MIC_CTL, PW3);
TOSH_ALIAS_OUTPUT_ONLY_PIN(SOUNDER_CTL, PW2);
TOSH_ALIAS_OUTPUT_ONLY_PIN(ACCEL_CTL, PW4);
TOSH_ALIAS_OUTPUT_ONLY_PIN(MAG_CTL, PW5);
TOSH_ALIAS_OUTPUT_ONLY_PIN(MIC_MUX_SEL, PW6);


void TOSH_SET_PIN_DIRECTIONS(void)
{
  /*  outp(0x00, DDRA);
  outp(0x00, DDRB);
  outp(0x00, DDRD);
  outp(0x02, DDRE);
  outp(0x02, PORTE);
  */

  TOSH_MAKE_RED_LED_OUTPUT();
  TOSH_MAKE_YELLOW_LED_OUTPUT();
  TOSH_MAKE_GREEN_LED_OUTPUT();

      
  TOSH_MAKE_PW7_OUTPUT();
  TOSH_MAKE_PW6_OUTPUT();
  TOSH_MAKE_PW5_OUTPUT();
  TOSH_MAKE_PW4_OUTPUT();
  TOSH_MAKE_PW3_OUTPUT(); 
  TOSH_MAKE_PW2_OUTPUT();
  TOSH_MAKE_PW1_OUTPUT();
  TOSH_MAKE_PW0_OUTPUT();

//CC2420 pins  
  TOSH_MAKE_MISO_INPUT();
  TOSH_MAKE_MOSI_OUTPUT();
  TOSH_MAKE_SPI_SCK_OUTPUT();
  TOSH_MAKE_CC_RSTN_OUTPUT();    
  TOSH_MAKE_CC_VREN_OUTPUT();
  TOSH_MAKE_CC_CS_INPUT(); 
  TOSH_MAKE_CC_FIFOP1_INPUT();    
  TOSH_MAKE_CC_CCA_INPUT();
  TOSH_MAKE_CC_SFD_INPUT();
  TOSH_MAKE_CC_FIFO_INPUT(); 

  TOSH_MAKE_RADIO_CCA_INPUT();



  TOSH_MAKE_SERIAL_ID_INPUT();
  TOSH_CLR_SERIAL_ID_PIN();  // Prevent sourcing current

  TOSH_MAKE_FLASH_SELECT_OUTPUT();
  TOSH_MAKE_FLASH_OUT_OUTPUT();
  TOSH_MAKE_FLASH_CLK_OUTPUT();
  TOSH_SET_FLASH_SELECT_PIN();
    
  TOSH_SET_RED_LED_PIN();
  TOSH_SET_YELLOW_LED_PIN();
  TOSH_SET_GREEN_LED_PIN();

}

