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


uint8_t CC2420RadioM_countRetry;
uint8_t CC2420RadioM_stateRadio;
 
uint8_t CC2420RadioM_currentDSN;
bool CC2420RadioM_bAckEnable;
bool CC2420RadioM_bPacketReceiving;
uint8_t CC2420RadioM_txlength;
Radio_MsgPtr CC2420RadioM_txbufptr;
Radio_MsgPtr CC2420RadioM_rxbufptr;
Radio_Msg CC2420RadioM_RxBuf;

volatile uint16_t CC2420RadioM_LocalAddr;




enum CC2420RadioM___nesc_unnamed4269 {
  CC2420RadioM_DISABLED_STATE = 0, 
  CC2420RadioM_DISABLED_STATE_STARTTASK, 
  CC2420RadioM_IDLE_STATE, 
  CC2420RadioM_TX_STATE, 
  CC2420RadioM_TX_WAIT, 
  CC2420RadioM_PRE_TX_STATE, 
  CC2420RadioM_POST_TX_STATE, 
  CC2420RadioM_POST_TX_ACK_STATE, 
  CC2420RadioM_RX_STATE, 
  CC2420RadioM_POWER_DOWN_STATE, 
  CC2420RadioM_WARMUP_STATE, 

  CC2420RadioM_TIMER_INITIAL = 0, 
  CC2420RadioM_TIMER_BACKOFF, 
  CC2420RadioM_TIMER_ACK
};



/*
uint8_t CC2420RadioM_countRetry;
uint8_t CC2420RadioM_stateRadio;
 
uint8_t CC2420RadioM_currentDSN;
bool CC2420RadioM_bAckEnable;
bool CC2420RadioM_bPacketReceiving;
uint8_t CC2420RadioM_txlength;
Radio_MsgPtr CC2420RadioM_txbufptr;
Radio_MsgPtr CC2420RadioM_rxbufptr;
Radio_Msg CC2420RadioM_RxBuf;

volatile uint16_t CC2420RadioM_LocalAddr;
*/



 inline   

result_t CC2420RadioM_SplitControl_default_initDone(void)

{
  return SUCCESS;
}



 inline  

result_t CC2420RadioM_CC2420SplitControl_initDone(void)

{
  return CC2420RadioM_SplitControl_initDone();
}




 inline  
result_t CC2420RadioM_SplitControl_init(void)
{


  { _atomic_t _atomic = _atomic_start();

    {
      CC2420RadioM_stateRadio = CC2420RadioM_DISABLED_STATE;
	  
      CC2420RadioM_currentDSN = 0;

	  CC2420RadioM_bAckEnable = FALSE;

	  CC2420RadioM_bPacketReceiving = FALSE;

	  CC2420RadioM_rxbufptr = &CC2420RadioM_RxBuf;

	  CC2420RadioM_rxbufptr->length = 0;
    }

    _atomic_end(_atomic); }


    CC2420RadioM_LocalAddr = CURRENT_NODE_ID;
  return CC2420ControlM_SplitControl_init();
}






inline   result_t CC2420RadioM_CC2420SplitControl_start(void){

  unsigned char result;



  result = CC2420ControlM_SplitControl_start();



  return result;

}

 inline  

result_t CC2420RadioM_SplitControl_start(void)

{
  uint8_t chkstateRadio;

  { _atomic_t _atomic = _atomic_start();

    chkstateRadio = CC2420RadioM_stateRadio;

    _atomic_end(_atomic); }

  if (chkstateRadio == CC2420RadioM_DISABLED_STATE) {
      { _atomic_t _atomic = _atomic_start();

        {
          CC2420RadioM_stateRadio = CC2420RadioM_WARMUP_STATE;
          CC2420RadioM_countRetry = 0;
          CC2420RadioM_rxbufptr->length = 0;
        }

        _atomic_end(_atomic); }

      return CC2420RadioM_CC2420SplitControl_start();
    }
  return FAIL;
}

 inline  

void CC2420RadioM_startRadio(void)

{
  result_t success = FAIL;


  { _atomic_t _atomic = _atomic_start();

    {
      if (CC2420RadioM_stateRadio == CC2420RadioM_DISABLED_STATE_STARTTASK) {
          CC2420RadioM_stateRadio = CC2420RadioM_DISABLED_STATE;
          success = SUCCESS;
        }
    }

    _atomic_end(_atomic); }

  if (success == SUCCESS) {
    CC2420RadioM_SplitControl_start();
    }
}

 inline  result_t CC2420RadioM_StdControl_start(void)

{


  result_t success = FAIL;

  { _atomic_t _atomic = _atomic_start();

    {
      if (CC2420RadioM_stateRadio == CC2420RadioM_DISABLED_STATE) {

          if (postTask(CC2420RadioM_startRadio, 5)) {     
		  	  success = SUCCESS;
              CC2420RadioM_stateRadio = CC2420RadioM_DISABLED_STATE_STARTTASK;
            }
        }
    }

    _atomic_end(_atomic); }

  return success;
}



 inline   

result_t CC2420RadioM_SplitControl_default_startDone(void)

{
  return SUCCESS;
}


inline   result_t CC2420RadioM_SplitControl_startDone(void){

  unsigned char result;



  result = CC2420RadioM_SplitControl_default_startDone();



  return result;

}












inline    result_t CC2420RadioM_SFD_enableCapture(bool arg_0xa41e260){

  unsigned char result;



  result = HPLCC2420InterruptM_SFD_enableCapture(arg_0xa41e260);



  return result;

}


inline    uint8_t CC2420RadioM_HPLChipcon_cmd(uint8_t arg_0xa403928){

  unsigned char result;



  result = HPLCC2420M_HPLCC2420_cmd(arg_0xa403928);



  return result;

}

 inline 

void CC2420RadioM_sendPacket(void)

{
  uint8_t status;

  CC2420RadioM_HPLChipcon_cmd(0x05);
  status = CC2420RadioM_HPLChipcon_cmd(0x00);
  if ((status >> 3) & 0x01) {

      CC2420RadioM_SFD_enableCapture(TRUE);
    }
  else {
      CC2420RadioM_sendFailed();
    }
}




 inline 

void CC2420RadioM_tryToSend(void)

{
  uint8_t currentstate;


  { _atomic_t _atomic = _atomic_start();

    currentstate = CC2420RadioM_stateRadio;

    _atomic_end(_atomic); }


  if (currentstate == CC2420RadioM_PRE_TX_STATE) {



      if (!TOSH_READ_CC_FIFO_PIN() && !TOSH_READ_CC_FIFOP_PIN()) {
          CC2420RadioM_flushRXFIFO();
        }

      if (TOSH_READ_RADIO_CCA_PIN()) {
          { _atomic_t _atomic = _atomic_start();

            CC2420RadioM_stateRadio = CC2420RadioM_TX_STATE;

            _atomic_end(_atomic); }
          CC2420RadioM_sendPacket();
        }
    }
   else
 	 CC2420RadioM_flushRXFIFO();

}




 inline   

result_t CC2420RadioM_HPLChipconFIFO_TXFIFODone(uint8_t length, uint8_t *data)

{
  CC2420RadioM_tryToSend();
  return SUCCESS;
}






inline    result_t CC2420RadioM_HPLChipconFIFO_writeTXFIFO(uint8_t arg_0xa40c010, uint8_t *arg_0xa40c170){

  unsigned char result;



  result = HPLCC2420FIFOM_HPLCC2420FIFO_writeTXFIFO(arg_0xa40c010, arg_0xa40c170);



  return result;

}



 inline  

void CC2420RadioM_startSend(void)

{

  if (!CC2420RadioM_HPLChipcon_cmd(0x09)) {
      CC2420RadioM_sendFailed();
      return;
    }

  if (!CC2420RadioM_HPLChipconFIFO_writeTXFIFO(CC2420RadioM_txlength + 1, (uint8_t *)CC2420RadioM_txbufptr)) {
      CC2420RadioM_sendFailed();
      return;
    }
}





 inline  

result_t CC2420RadioM_Send_send(Radio_MsgPtr pMsg)

{
  uint8_t currentstate;


  { _atomic_t _atomic = _atomic_start();

    currentstate = CC2420RadioM_stateRadio;

    _atomic_end(_atomic); }

  if (currentstate == CC2420RadioM_IDLE_STATE) {

      pMsg->fcflo = 0x08;
      if (CC2420RadioM_bAckEnable) {
        pMsg->fcfhi = 0x21;
        }
      else {

        pMsg->fcfhi = 0x01;
        }
      pMsg->destpan = BCAST_ADDRESS;

 //don't understand why it has to be LSB. Must check later. 
 
      pMsg->addr = toLSB16(pMsg->addr);

//The length now includes both the 
      pMsg->length = pMsg->length + MSG_HEADER_SIZE + MSG_FOOTER_SIZE;

      pMsg->dsn = ++CC2420RadioM_currentDSN;

      pMsg->time = 0;

      CC2420RadioM_txlength = pMsg->length - MSG_FOOTER_SIZE;
      CC2420RadioM_txbufptr = pMsg;
      CC2420RadioM_countRetry = 8;

      { _atomic_t _atomic = _atomic_start();

        CC2420RadioM_stateRadio = CC2420RadioM_PRE_TX_STATE;

        _atomic_end(_atomic); }
      if (!postTask(CC2420RadioM_startSend, 5)) {
        CC2420RadioM_sendFailed();
        }
      return SUCCESS;
    }
  return FAIL;
}








inline    result_t CC2420RadioM_FIFOP_startWait(bool arg_0xa422588){

  unsigned char result;



  result = HPLCC2420InterruptM_FIFOP_startWait(arg_0xa422588);



  return result;

}




inline    result_t CC2420RadioM_CC2420Control_RxMode(void){

  unsigned char result;



  result = CC2420ControlM_CC2420Control_RxMode();



  return result;

}

 inline  

result_t CC2420RadioM_CC2420SplitControl_startDone(void)

{
  uint8_t chkstateRadio;

  { _atomic_t _atomic = _atomic_start();

    chkstateRadio = CC2420RadioM_stateRadio;

    _atomic_end(_atomic); }

  if (chkstateRadio == CC2420RadioM_WARMUP_STATE) {
      CC2420RadioM_CC2420Control_RxMode();

      CC2420RadioM_FIFOP_startWait(FALSE);

      CC2420RadioM_SFD_enableCapture(TRUE);

      { _atomic_t _atomic = _atomic_start();

        CC2420RadioM_stateRadio = CC2420RadioM_IDLE_STATE;

        _atomic_end(_atomic); }
    }
  CC2420RadioM_SplitControl_startDone();
  return SUCCESS;
}






inline    uint16_t CC2420RadioM_HPLChipcon_read(uint8_t arg_0xa4103b0){

  unsigned int result;



  result = HPLCC2420M_HPLCC2420_read(arg_0xa4103b0);



  return result;

}






inline    result_t CC2420RadioM_FIFOP_disable(void){

  unsigned char result;



  result = HPLCC2420InterruptM_FIFOP_disable();



  return result;

}

 inline  

void CC2420RadioM_delayedRXFIFOtask(void)

{
  CC2420RadioM_delayedRXFIFO();
}

 inline   

result_t CC2420RadioM_FIFOP_fired(void)

{






  if (!TOSH_READ_CC_FIFO_PIN()) {
      CC2420RadioM_flushRXFIFO();
      return SUCCESS;
    }

  { _atomic_t _atomic = _atomic_start();

    {
      if (postTask(CC2420RadioM_delayedRXFIFOtask, 5)) {
          CC2420RadioM_FIFOP_disable();
        }
      else {
          CC2420RadioM_flushRXFIFO();
        }
    }

    _atomic_end(_atomic); }


  return SUCCESS;
}




inline   Radio_MsgPtr CC2420RadioM_Receive_receive(Radio_MsgPtr arg_0xa31b650){

  struct Radio_Msg *result;



  result = AMStandard_RadioReceive_receive(arg_0xa31b650);



  return result;

}

 inline  

void CC2420RadioM_PacketRcvd(void)

{
  Radio_MsgPtr pBuf;


  { _atomic_t _atomic = _atomic_start();

    {
      pBuf = CC2420RadioM_rxbufptr;
    }

    _atomic_end(_atomic); }
  pBuf = CC2420RadioM_Receive_receive((Radio_MsgPtr )pBuf);
  { _atomic_t _atomic = _atomic_start();

    {
      if (pBuf) {

        CC2420RadioM_rxbufptr = pBuf;
        }

      CC2420RadioM_rxbufptr->length = 0;
      CC2420RadioM_bPacketReceiving = FALSE;
    }

    _atomic_end(_atomic); }
}


//ALL THE DATA IN LENGTH IS THE CORRECT FRAME 

 inline   

result_t CC2420RadioM_HPLChipconFIFO_RXFIFODone(uint8_t length, uint8_t *data)

{




  //The data now contains the full packet, starting with a packet length which is the remaining part of the packet, not including itself

  //The length, however, includes everything 
  
  uint8_t currentstate;


  { _atomic_t _atomic = _atomic_start();

    {
      currentstate = CC2420RadioM_stateRadio;
    }

    _atomic_end(_atomic); }




  if (((

  !TOSH_READ_CC_FIFO_PIN() && !TOSH_READ_CC_FIFOP_PIN())
   || length == 0) || length > MSG_DATA_SIZE) {
      CC2420RadioM_flushRXFIFO();
      { _atomic_t _atomic = _atomic_start();

        CC2420RadioM_bPacketReceiving = FALSE;

        _atomic_end(_atomic); }
      return SUCCESS;
    }

  CC2420RadioM_rxbufptr = (Radio_MsgPtr )data;






  if ((CC2420RadioM_rxbufptr->fcfhi & 0x07) != 0x01 || 
  CC2420RadioM_rxbufptr->fcflo != 0x08) {
      CC2420RadioM_flushRXFIFO();
      { _atomic_t _atomic = _atomic_start();

        CC2420RadioM_bPacketReceiving = FALSE;

        _atomic_end(_atomic); }
      return SUCCESS;
    }

 //Now modifies the length such that it only points to the actual payload defined by the user 
 
  CC2420RadioM_rxbufptr->length = CC2420RadioM_rxbufptr->length - MSG_HEADER_SIZE - MSG_FOOTER_SIZE;

 //Revised by Qing, from 29 to PAYLOAD_LENGTH 
 
  if (CC2420RadioM_rxbufptr->length > PAYLOAD_LENGTH) {
      CC2420RadioM_flushRXFIFO();
      { _atomic_t _atomic = _atomic_start();

        CC2420RadioM_bPacketReceiving = FALSE;

        _atomic_end(_atomic); }
      return SUCCESS;
    }


  CC2420RadioM_rxbufptr->addr = fromLSB16(CC2420RadioM_rxbufptr->addr);

  //CRC and LQI share the information 
  
  CC2420RadioM_rxbufptr->crc = data[length - 1] >> 7;

  CC2420RadioM_rxbufptr->strength = data[length - 2];

  CC2420RadioM_rxbufptr->lqi = data[length - 1] & 0x7F;

  { _atomic_t _atomic = _atomic_start();

    {
      if (!postTask(CC2420RadioM_PacketRcvd, 5)) {
          CC2420RadioM_bPacketReceiving = FALSE;
        }
    }

    _atomic_end(_atomic); }

  if (!TOSH_READ_CC_FIFO_PIN() && !TOSH_READ_CC_FIFOP_PIN()) {
      CC2420RadioM_flushRXFIFO();
      return SUCCESS;
    }

  if (!TOSH_READ_CC_FIFOP_PIN()) {
      if (postTask(CC2420RadioM_delayedRXFIFOtask, 5)) {
        return SUCCESS;
        }
    }

  CC2420RadioM_flushRXFIFO();


  return SUCCESS;
}




inline    result_t CC2420RadioM_HPLChipconFIFO_readRXFIFO(uint8_t arg_0xa4118a0, uint8_t *arg_0xa411a00){

  unsigned char result;



  result = HPLCC2420FIFOM_HPLCC2420FIFO_readRXFIFO(arg_0xa4118a0, arg_0xa411a00);



  return result;

}




inline   result_t CC2420RadioM_Send_sendDone(Radio_MsgPtr arg_0xa3c3710, result_t arg_0xa3c3860){

  unsigned char result;



  result = AMStandard_RadioSend_sendDone(arg_0xa3c3710, arg_0xa3c3860);



  return result;

}

 inline  

void CC2420RadioM_PacketSent(void)

{
  Radio_MsgPtr pBuf;

  { _atomic_t _atomic = _atomic_start();

    {
      CC2420RadioM_stateRadio = CC2420RadioM_IDLE_STATE;
      pBuf = CC2420RadioM_txbufptr;
      pBuf->length = pBuf->length - MSG_HEADER_SIZE - MSG_FOOTER_SIZE;
    }

    _atomic_end(_atomic); }

  CC2420RadioM_Send_sendDone(pBuf, SUCCESS);
}




inline    result_t CC2420RadioM_SFD_disable(void){

  unsigned char result;



  result = HPLCC2420InterruptM_SFD_disable();



  return result;

}
 inline   

result_t CC2420RadioM_SFD_captured(uint16_t time)

{
  switch (CC2420RadioM_stateRadio) {
      case CC2420RadioM_TX_STATE: 

        CC2420RadioM_SFD_enableCapture(FALSE);


      if (!TOSH_READ_CC_SFD_PIN()) {
          CC2420RadioM_SFD_disable();
        }
      else {
          CC2420RadioM_stateRadio = CC2420RadioM_TX_WAIT;
        }

      CC2420RadioM_txbufptr->time = time;


      if (CC2420RadioM_stateRadio == CC2420RadioM_TX_WAIT) {
          break;
        }
      case CC2420RadioM_TX_WAIT: 

        CC2420RadioM_stateRadio = CC2420RadioM_POST_TX_STATE;
      CC2420RadioM_SFD_disable();

      CC2420RadioM_SFD_enableCapture(TRUE);

      if (!postTask(CC2420RadioM_PacketSent, 5)) {
        CC2420RadioM_sendFailed();
        }

      break;
      default: ;
    }

  return SUCCESS;
}



 

void CC2420RadioM_sendFailed(void)

{
  { _atomic_t _atomic = _atomic_start();

    CC2420RadioM_stateRadio = CC2420RadioM_IDLE_STATE;

    _atomic_end(_atomic); }
  CC2420RadioM_txbufptr->length = CC2420RadioM_txbufptr->length - MSG_HEADER_SIZE - MSG_FOOTER_SIZE;
  CC2420RadioM_Send_sendDone(CC2420RadioM_txbufptr, FAIL);
}



 

void CC2420RadioM_flushRXFIFO(void)

{
  CC2420RadioM_FIFOP_disable();
  CC2420RadioM_HPLChipcon_read(0x3F);
  CC2420RadioM_HPLChipcon_cmd(0x08);
  CC2420RadioM_HPLChipcon_cmd(0x08);
  { _atomic_t _atomic = _atomic_start();

    CC2420RadioM_bPacketReceiving = FALSE;

    _atomic_end(_atomic); }
  CC2420RadioM_FIFOP_startWait(FALSE);
}




 

void CC2420RadioM_delayedRXFIFO(void)

{
//The MSG_DATA_SIZE is defined such that it contains the total length of data plus 2 bytes of the RSSI and LQI 
	  uint8_t len = MSG_DATA_SIZE;
  uint8_t _bPacketReceiving;

  if (!TOSH_READ_CC_FIFO_PIN() && !TOSH_READ_CC_FIFOP_PIN()) {
      CC2420RadioM_flushRXFIFO();
      return;
    }

  { _atomic_t _atomic = _atomic_start();

    {
      _bPacketReceiving = CC2420RadioM_bPacketReceiving;

      if (_bPacketReceiving) {
          if (!postTask(CC2420RadioM_delayedRXFIFOtask, 5)) {
            CC2420RadioM_flushRXFIFO();
            }
        }
      else 

        {
          CC2420RadioM_bPacketReceiving = TRUE;
        }
    }

    _atomic_end(_atomic); }




 //Now read the RX to get the data 
 
  if (!_bPacketReceiving) {
      if (!CC2420RadioM_HPLChipconFIFO_readRXFIFO(len, (uint8_t *)CC2420RadioM_rxbufptr)) {
          { _atomic_t _atomic = _atomic_start();

            CC2420RadioM_bPacketReceiving = FALSE;

            _atomic_end(_atomic); }
          if (!postTask(CC2420RadioM_delayedRXFIFOtask, 5)) {
              CC2420RadioM_flushRXFIFO();
            }
          return;
        }
    }
  CC2420RadioM_flushRXFIFO();
}






inline   result_t CC2420RadioM_SplitControl_initDone(void){

  unsigned char result;



  result = CC2420RadioM_SplitControl_default_initDone();



  return result;

}


