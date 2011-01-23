#include "avrhardware.h"
#include "micazhardware.h"
#include "scheduling.h"
#include "Leds.h"
//#include "Timer.h"
#include "amcommon.h"
#include "amradio.h"
#include "byteorder.h"
#include "cc2420Const.h"
#include "CC2420ControlM.h"
#include "CC2420RadioM.h"
#include "HPLCC2420FIFOM.h"
#include "HPLCC2420InterruptM.h"
#include "HPLCC2420M.h"
#include "HPLTimer1.h"
#include "Types.h"
 

bool HPLCC2420FIFOM_bSpiAvail;
uint8_t *HPLCC2420FIFOM_txbuf;


uint8_t *HPLCC2420FIFOM_rxbuf;
uint8_t HPLCC2420FIFOM_txlength;
 

uint8_t HPLCC2420FIFOM_rxlength;
bool HPLCC2420FIFOM_rxbufBusy;


bool HPLCC2420FIFOM_txbufBusy;





inline result_t HPLCC2420FIFOM_HPLCC2420FIFO_TXFIFODone(uint8_t arg_0xa40cd20, uint8_t *arg_0xa40ce80){

  unsigned char result;



  result = CC2420RadioM_HPLChipconFIFO_TXFIFODone(arg_0xa40cd20, arg_0xa40ce80);



  return result;

}

 inline  

void HPLCC2420FIFOM_signalTXdone(void)

{
  uint8_t _txlen;
  uint8_t *_txbuf;

  { _atomic_t _atomic = _atomic_start();

    {
      _txlen = HPLCC2420FIFOM_txlength;
      _txbuf = HPLCC2420FIFOM_txbuf;
      HPLCC2420FIFOM_txbufBusy = FALSE;
    }

    _atomic_end(_atomic); }

  HPLCC2420FIFOM_HPLCC2420FIFO_TXFIFODone(_txlen, _txbuf);
}


 inline   

result_t HPLCC2420FIFOM_HPLCC2420FIFO_writeTXFIFO(uint8_t len, uint8_t *msg)

{
  uint8_t i = 0;
  uint8_t status;
  bool returnFail = FALSE;

  { _atomic_t _atomic = _atomic_start();

    {
      if (HPLCC2420FIFOM_txbufBusy) {
        returnFail = TRUE;
        }
      else {

        HPLCC2420FIFOM_txbufBusy = TRUE;
        }
    }

    _atomic_end(_atomic); }
  if (returnFail) {
    return FAIL;
    }


  { _atomic_t _atomic = _atomic_start();

    {
      HPLCC2420FIFOM_bSpiAvail = FALSE;
      HPLCC2420FIFOM_txlength = len;
      HPLCC2420FIFOM_txbuf = msg;
      TOSH_CLR_CC_CS_PIN();
     	outp(CC2420_TXFIFO,SPDR);
		  while (!(inp(SPSR) & 0x80)){};          //wait for spi xfr to complete
		  status = inp(SPDR);
		  for (i=0; i < HPLCC2420FIFOM_txlength; i++){ 
			outp(*HPLCC2420FIFOM_txbuf,SPDR);
			HPLCC2420FIFOM_txbuf++;
			while (!(inp(SPSR) & 0x80)){};          //wait for spi xfr to complete
			}
		 HPLCC2420FIFOM_bSpiAvail = TRUE;
    }
   

    _atomic_end(_atomic); }
  TOSH_SET_CC_CS_PIN();
  if (postTask(HPLCC2420FIFOM_signalTXdone, 5) == FAIL) {
      { _atomic_t _atomic = _atomic_start();

        HPLCC2420FIFOM_txbufBusy = FALSE;

        _atomic_end(_atomic); }
      return FAIL;
    }
  return status;
}





inline    result_t HPLCC2420FIFOM_HPLCC2420FIFO_RXFIFODone(uint8_t arg_0xa40c690, uint8_t *arg_0xa40c7f0){

  unsigned char result;



  result = CC2420RadioM_HPLChipconFIFO_RXFIFODone(arg_0xa40c690, arg_0xa40c7f0);



  return result;

}

 inline  

void HPLCC2420FIFOM_signalRXdone(void)

{
  uint8_t _rxlen;
  uint8_t *_rxbuf;

  { _atomic_t _atomic = _atomic_start();

    {
      _rxlen = HPLCC2420FIFOM_rxlength;
      _rxbuf = HPLCC2420FIFOM_rxbuf;
      HPLCC2420FIFOM_rxbufBusy = FALSE;
    }

    _atomic_end(_atomic); }

  HPLCC2420FIFOM_HPLCC2420FIFO_RXFIFODone(_rxlen, _rxbuf);
}

 inline   

//Seems that the transmitted len is the actual length over the air 
//The first number in the frame is the remaining part 

result_t HPLCC2420FIFOM_HPLCC2420FIFO_readRXFIFO(uint8_t len, uint8_t *msg)

{
  uint8_t status;

  uint8_t i;
  bool returnFail = FALSE;

  { _atomic_t _atomic = _atomic_start();

    {
      if (HPLCC2420FIFOM_rxbufBusy) {
        returnFail = TRUE;
        }
      else {

        HPLCC2420FIFOM_rxbufBusy = TRUE;
        }
    }

    _atomic_end(_atomic); }
  if (returnFail) {
    return FAIL;
    }


  { _atomic_t _atomic = _atomic_start();

    {
      HPLCC2420FIFOM_bSpiAvail = FALSE;
      HPLCC2420FIFOM_rxbuf = msg;
      TOSH_CLR_CC_CS_PIN();
    
       outp(CC2420_RXFIFO | 0x40 ,SPDR);       //output Rxfifo address
	  while (!(inp(SPSR) & 0x80)){};          //wait for spi xfr to complete
	  status = inp(SPDR);
      outp(0, SPDR);
	  while (!(inp(SPSR) & 0x80)){};          //wait for spi xfr to complete
	  HPLCC2420FIFOM_rxlength = inp(SPDR);
	  
	  //Qing revise
	  //The first byte, however, only contains a part of the whole, which needs to add 1 to mean that it is the total to be read 
	  //and this also defaults to len, which is the MSG_DATA_SIZE
	  
      if (HPLCC2420FIFOM_rxlength > 0) {
          HPLCC2420FIFOM_rxbuf[0] = HPLCC2420FIFOM_rxlength;

          HPLCC2420FIFOM_rxlength++;

          if (HPLCC2420FIFOM_rxlength > len) {

            HPLCC2420FIFOM_rxlength = len;
            }
          for (i = 1; i < HPLCC2420FIFOM_rxlength; i++) {
             outp(0,SPDR);
			while (!(inp(SPSR) & 0x80)){};          //wait for spi xfr to complete
			HPLCC2420FIFOM_rxbuf[i] = inp(SPDR);
            }
        }

      HPLCC2420FIFOM_bSpiAvail = TRUE;
    }

    _atomic_end(_atomic); }
  TOSH_SET_CC_CS_PIN();
  if (postTask(HPLCC2420FIFOM_signalRXdone, 5) == FAIL) {
      { _atomic_t _atomic = _atomic_start();

        HPLCC2420FIFOM_rxbufBusy = FALSE;

        _atomic_end(_atomic); }
      return FAIL;
    }
  return SUCCESS;
}
