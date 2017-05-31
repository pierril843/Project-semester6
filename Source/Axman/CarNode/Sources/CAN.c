#include <hidef.h>        /* common defines and macros */
#include "derivative.h"   /* derivative-specific definitions */
#include "CAN.h"

void MSCAN_Init(void) {
  // turn on module and force into init mode
  SET_BITS( CANCTL1, CANCTL1_CANE_MASK );               // turn on CAN module (might be already on if debugging)
  SET_BITS( CANCTL0, CANCTL0_INITRQ_MASK );             // put module into init mode
  while( BIT_IS_CLR( CANCTL1, CANCTL1_INITAK_MASK ) );  // wait for module to enter init mode

  // now in init mode
  // set up registers to control MSCAN operation
  CANCTL1 = CANCTL1_INIT;
  CANBTR0 = CANBTR0_INIT;
  CANBTR1 = CANBTR1_INIT;  
  // all done - leave init mode and enter normal mode
  CLR_BITS( CANCTL0, CANCTL0_INITRQ_MASK );             // turn off the init mode request flag
                          // Enable CAN receive interrupt
  while( BIT_IS_SET( CANCTL1, CANCTL1_INITAK_MASK ) );  // wait for module to exit init mode
}

int MSCAN_Putd( CANmsg_t CANmsg, uint_8 priority) {
  uint_8  CAN_BufferFlg;		  // copy of CANTBSEL with selected tx register set
  uint_16 i;            // loop counter for copying tx data to outptu buffer
  uint_32 CAN_IdBuffer = 0;  // local copy of 32 bit message ID - msg ID built here and then copied to outpu buffer
  uint_8 *CAN_TxDataPtr; // treating output data registers as array of 8 bytes - using this pointer to access
  uint_32 *CAN_TxIdPtr;  // treating 4 bytes of message ID register as single 32-bit unsigned value - using this pointer to access

  // acquire a transmit register - this function will return error if no buffers available
  CANTBSEL = CANTFLG;   // get copy of flag register with available buffers and write to CANTBSEL to acquire one
  CAN_BufferFlg = CANTBSEL;  // if a buffer was available, CANTBSEL will indicate it with a set bit - save in CAN_BufferFlg to use later to mark buffer for xmit

  //standard identifier
  CAN_IdBuffer = ((CANmsg.id & 0x07ff) << (32-11));

  CAN_TxIdPtr = (uint_32 *)(&CANTXIDR0);
  *CAN_TxIdPtr = CAN_IdBuffer;

  // copy data to transmit data buffer registers
  CAN_TxDataPtr = &CANTXDSR0;
  for (i = 0; i < CANmsg.length; i++)
    *CAN_TxDataPtr++ = CANmsg.data[i];
  // load the data buffer size into tx size register
  CANTXDLR = CANmsg.length;
  // load the local message priority into tx local priority register
  CANTXTBPR = priority;

  // mark buffer for transmission
  CANTFLG = CAN_BufferFlg;
  return(i);
}

void MSCAN_ListenForMsg(uint_32 id) {
  uint_32 CAN_IdBuffer;	// local copy of 32 bit message ID - msg ID built here and then copied to acceptance register
  uint_32 *CAN_IdRegPtr;

  MSCAN_SetInitMode();

  // set up acceptance filter registers
  CANIDAC = 0;	// setup for 2 32-bit filters - all bits significant
  // filter 1 - standard identifier
  CAN_IdBuffer = ((id & 0x07ff) << (32-11));
  

  CAN_IdRegPtr = (uint_32 *)(&CANIDAR0);
  *CAN_IdRegPtr = CAN_IdBuffer;

  MSCAN_SetNormMode();
}

void MSCAN_SetInitMode(void){
  // Force module into init mode via sleep mode (to ensure xmit/rcv complete
  SET_BITS( CANCTL0, CANCTL0_SLPRQ_MASK );		// put module into sleep mode
  while( BIT_IS_CLR( CANCTL1, CANCTL1_SLPAK_MASK ) );	// wait for module to fall asleep
  SET_BITS( CANCTL0, CANCTL0_INITRQ_MASK );		// put module into init mode
  while( BIT_IS_CLR( CANCTL1, CANCTL1_INITAK_MASK ) );	// wait for module to enter init mode
}

void MSCAN_SetNormMode(void){
  CLR_BITS( CANCTL0, CANCTL0_INITRQ_MASK );		// turn off the init mode request flag
  while( BIT_IS_SET( CANCTL1, CANCTL1_INITAK_MASK ) );	// wait for module to exit init mode
  CLR_BITS( CANCTL0, CANCTL0_SLPRQ_MASK );		// turn off sleep mode flag
  while( BIT_IS_SET( CANCTL1, CANCTL1_SLPAK_MASK ) );	// wait for module to wake up
}

int MSCAN_GotMsg(void) {
  return ( BIT_IS_SET( CANRFLG, CANRFLG_RXF_MASK ) );
}

CANmsg_t MSCAN_Getd(void) {
  CANmsg_t CANmsg;
  //uint_16 index;
  uint_8 *dataPtr;
  uint_16 i = 0;

  CANmsg.id = *((uint_16 *)(&CANRXIDR0))>>5;

  CANmsg.length = CANRXDLR;
  dataPtr = (uint_8 *)(&CANRXDSR0);

  for (i = 0; i < CANmsg.length; i++)
    CANmsg.data[i] = *dataPtr++;

  SET_BITS( CANRFLG, CANRFLG_RXF_MASK );
  return(CANmsg);
}
