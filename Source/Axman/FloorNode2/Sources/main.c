#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "CAN.h"
#include "Macros.h"
#include "Elevator.h"


#define LED_BITS_MASK         (PTS_PTS2_MASK | PTS_PTS3_MASK)       // LED bits on port S
#define LED_DDR_MASK          (DDRS_DDRS2_MASK | DDRS_DDRS3_MASK)   // DDR for LED bits on port S
#define LED_BITS_INIT         PTS_PTS2_MASK     // initial value written to port S LED bits
#define FLOOR_UP_REQUESTED_LED_MASK 0b00000001
#define FLOOR_DOWN_REQUESTED_LED_MASK 0b00010000
#define FLOOR3_LED_MASK 0b00001000
#define FLOOR2_LED_MASK 0b00000100
#define FLOOR1_LED_MASK 0b00000010
#define BUTTON_IS_PRESSED 0b10000000
#define BUTTON2_IS_PRESSED 0b01000000
#define LEDS_PORT_MASK 0b00001110  

uint_8 volatile floorLocation;

void main(void) {
   unsigned int volatile testVar = 0;
	int volatile i;
	CANmsg_t msg;
	uint_8 buttonLock = 0;	
	
	msg.data[0] = 0xA5;
	msg.id = 0x30;
	msg.length = 0x01;
	
	PTS = LED_BITS_INIT;
  DDRS  |= LED_DDR_MASK;
  
  DDRJ = 0b00000000;
  
	
  MSCAN_Init();
  
  DDRAD = 0b00000000;
  ATDDIEN = 0b11111111;
  DDRA = 0b11111111;
  
  MSCAN_ListenForMsg(0x101);
  
  //CANRIER = 0b00000001;
  
  //EnableInterrupts;  
  
  //i = MSCAN_Putd(msg,0x05);
  for (;;)
  {
    if ((PTJ == BUTTON_IS_PRESSED) && buttonLock == 0)
      {
        PORTA |= FLOOR_UP_REQUESTED_LED_MASK;
        i = F2_Status(0x01);
        buttonLock = 1;
      }
    if ((PTJ == BUTTON2_IS_PRESSED) && buttonLock == 0)
      {
        PORTA |= FLOOR_DOWN_REQUESTED_LED_MASK;
        i = F2_Status(0x01);
        buttonLock = 1;
      }
          
    if (MSCAN_GotMsg())
      {
      PTS ^= LED_BITS_MASK;    
      msg = MSCAN_Getd();
      floorLocation = msg.data[0];
      
      if ((floorLocation & 0b00000011) == 0x02)
        {
          FORCE_BITS(PORTA, LEDS_PORT_MASK, FLOOR2_LED_MASK); 
          PORTA &= ~(FLOOR_UP_REQUESTED_LED_MASK | FLOOR_DOWN_REQUESTED_LED_MASK);
          buttonLock = 0;          
        }
        else if ((floorLocation & 0b00000011) == 0x01)
        {
          FORCE_BITS(PORTA, LEDS_PORT_MASK, FLOOR1_LED_MASK);          
        }
        else if ((floorLocation & 0b00000011) == 0x03)
        {
          FORCE_BITS(PORTA, LEDS_PORT_MASK, FLOOR3_LED_MASK);          
        }          
     }
  }  
}



