#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "CAN.h"
#include "Macros.h"
#include "Elevator.h"

#define LED_BITS_MASK         (PTS_PTS2_MASK | PTS_PTS3_MASK)       // LED bits on port S
#define LED_DDR_MASK          (DDRS_DDRS2_MASK | DDRS_DDRS3_MASK)   // DDR for LED bits on port S
#define LED_BITS_INIT         PTS_PTS2_MASK     // initial value written to port S LED bits
#define BUTTON_IS_PRESSED 0b10000000
#define BUTTON2_IS_PRESSED 0b01000000
#define FLOOR3_LED_MASK 0b00001000
#define FLOOR2_LED_MASK 0b00000100
#define FLOOR1_LED_MASK 0b00000010
#define LEDS_PORT_MASK 0b00001110
#define FLOOR_REQUESTED_LED_MASK 0b00000001  
#define OPEN 0
#define CLOSED 1


void main(void) {
uint_8 floor1ButtonLock = 0;
uint_8 floor2ButtonLock = 0;
uint_8 floor3ButtonLock = 0;
uint_16 i = 0;
CANmsg_t msg;
uint_8 volatile floorLocation;
uint_8 DoorState = 1;

DDRAD = 0b00000000;
DDRA = 0b11111111;
DDRB = 0b11111111;
DDRS  |= LED_DDR_MASK;
DDRJ = 0b00000000;

PTS = LED_BITS_INIT;
ATDDIEN = 0b11111111;  

MSCAN_Init();
MSCAN_ListenForMsg(0x101); //listen to EC messages

for (;;)
  {
    if (PTAD == 0b10000001 && floor1ButtonLock == 0)
      {
        PORTB |= 0b00000001;
        floor1ButtonLock = 1;
        i = CC_Status(DoorState, 0x01);
        PTS ^= LED_BITS_MASK;
      }
    if (PTAD == 0b10000010 && floor2ButtonLock == 0)
      {
        PORTB |= 0b00000010;
        floor2ButtonLock = 1;
        i = CC_Status(DoorState, 0x02);
        PTS ^= LED_BITS_MASK;
      }
    if (PTAD == 0b10000100 && floor3ButtonLock == 0)
      {
        PORTB |= 0b00000100;
        floor3ButtonLock = 1;
        i = CC_Status(DoorState, 0x03);
        PTS ^= LED_BITS_MASK;
      }
      
    if (PTJ == BUTTON_IS_PRESSED)
      {
        DoorState = OPEN;
        SET_BITS(PORTB, 0b00001000);    
      }
      
    if (PTJ == BUTTON2_IS_PRESSED)
      {
        DoorState = CLOSED;
        CLR_BITS(PORTB, 0b00001000);   
      }  
      
  
  if (MSCAN_GotMsg())
      {
      //PTS ^= LED_BITS_MASK;    
      msg = MSCAN_Getd();
      floorLocation = msg.data[0];
      
      if ((floorLocation & 0b00000011) == 0x01)
        {
          FORCE_BITS(PORTA, LEDS_PORT_MASK, FLOOR1_LED_MASK); 
          PORTA &= ~FLOOR_REQUESTED_LED_MASK;
          CLR_BITS(PORTB, 0b00000001);
          floor1ButtonLock = 0;
          PTS ^= LED_BITS_MASK;          
        }
        else if ((floorLocation & 0b00000011) == 0x02)
        {
          FORCE_BITS(PORTA, LEDS_PORT_MASK, FLOOR2_LED_MASK);          
          PORTA &= ~0b00000010;
          CLR_BITS(PORTB, 0b00000010);
          floor2ButtonLock = 0;
          PTS ^= LED_BITS_MASK;          
        }
        else if ((floorLocation & 0b00000011) == 0x03)
        {
          FORCE_BITS(PORTA, LEDS_PORT_MASK, FLOOR3_LED_MASK);
          PORTA &= ~0b00000100;
          CLR_BITS(PORTB, 0b00000100);
          floor3ButtonLock = 0;
          PTS ^= LED_BITS_MASK;          
        }          
     }
  }
  


	//EnableInterrupts;


  
}
