#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "CAN.h"
#include "Macros.h"
#include "Elevator.h"


#define LED_BITS_MASK         (PTS_PTS2_MASK | PTS_PTS3_MASK)       // LED bits on port S
#define LED_DDR_MASK          (DDRS_DDRS2_MASK | DDRS_DDRS3_MASK)   // DDR for LED bits on port S
#define LED_BITS_INIT         PTS_PTS2_MASK     // initial value written to port S LED bits


void main(void) {
   unsigned int volatile testVar = 0;
	int volatile i;
	CANmsg_t msg;
	
	msg.data[0] = 0xA5;
	msg.id = 0x30;
	msg.length = 0x01;
	
  MSCAN_Init();
  
  
  i = MSCAN_Putd(msg,0x05);
  
  i = F2_Status(0x03);
  
  if (i != 0)
  { 
  testVar++;
  i++;
  }
  for(;;)
  {
  }
}
