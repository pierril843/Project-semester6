#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "CAN.h"
#include "Macros.h"
#include "Elevator.h"

#define LED_BITS_MASK         (PTS_PTS2_MASK | PTS_PTS3_MASK)       // LED bits on port S
#define LED_DDR_MASK          (DDRS_DDRS2_MASK | DDRS_DDRS3_MASK)   // DDR for LED bits on port S
#define LED_BITS_INIT         PTS_PTS2_MASK     // initial value written to port S LED bits
#define FLOOR1_BUTTON_IS_PRESSED 0b10000001 //floor 1 in car button
#define FLOOR2_BUTTON_IS_PRESSED 0b10000010 //floor 2 in car button
#define FLOOR3_BUTTON_IS_PRESSED 0b10000100 //floor 3 in car button
#define BUTTON_IS_PRESSED 0b10000000 //Switch 2 on Axman 
#define BUTTON2_IS_PRESSED 0b01000000 //Switch 2 on Axman
#define FLOOR3_LED_MASK 0b00001000 //floor 3 position in car LED
#define FLOOR2_LED_MASK 0b00000100 //floor 2 position in car LED
#define FLOOR1_LED_MASK 0b00000010 //floor 1 position in car LED
#define FLOOR1_REQUEST_LED_MASK 0b00000001  //floor 1 button in car pressed LED
#define FLOOR2_REQUEST_LED_MASK 0b00000010  //floor 2 button in car pressed LED
#define FLOOR3_REQUEST_LED_MASK 0b00000100  //floor 3 button in car pressed LED
#define LEDS_PORT_MASK 0b00001110 //PORTA LED locations  
#define OPEN 0 //door state open
#define CLOSED 1  //door state closed
#define DOOR_OPEN_LED_MASK 0b00001000 //door open LED location
#define DOOR_CLOSED_LED_MASK 0b00001000 //door closed LED location
#define FLOOR_LOCATION_MASK 0b00000011  //mask to seperate enable bit from ID in CAN messages  


void main(void) {
uint_8 floor1ButtonLock = 0; //prevent floor 1 button from sending more than one message when pressed
uint_8 floor2ButtonLock = 0; //prevent floor 2 button from sending more than one message when pressed
uint_8 floor3ButtonLock = 0; //prevent floor 3 button from sending more than one message when pressed
uint_16 retCode = 0;  //return code to check function returns
CANmsg_t msg; //CAN message structure
uint_8 floorLocation;
uint_8 DoorState = 1;

DDRAD = 0b00000000; //PORTAD is input
DDRA = 0b11111111;  //PORTA is output
DDRB = 0b11111111;  //PORTB is output
DDRS  |= LED_DDR_MASK; //Port S is output
DDRJ = 0b00000000;     //PortJ is input

PTS = LED_BITS_INIT; //init PORTS to turn on LED1
ATDDIEN = 0b11111111;//enable PORTAD digital input  

MSCAN_Init(); //initialize CAN for 125kbps
MSCAN_ListenForMsg(EC_ID); //listen to EC messages

for (;;)
  {
    if (PTAD == FLOOR1_BUTTON_IS_PRESSED && floor1ButtonLock == 0) //floor 1 button pressed
      {
        PORTB |= FLOOR1_REQUEST_LED_MASK;
        floor1ButtonLock = 1;
        retCode = CC_Status(DoorState, FLOOR_1);
        PTS ^= LED_BITS_MASK;
      }
    if (PTAD == FLOOR2_BUTTON_IS_PRESSED && floor2ButtonLock == 0) //floor 2 button pressed
      {
        PORTB |= FLOOR2_REQUEST_LED_MASK;
        floor2ButtonLock = 1;
        retCode = CC_Status(DoorState, FLOOR_2);
        PTS ^= LED_BITS_MASK;
      }
    if (PTAD == FLOOR3_BUTTON_IS_PRESSED && floor3ButtonLock == 0) //floor 3 button pressed
      {
        PORTB |= FLOOR3_REQUEST_LED_MASK;
        floor3ButtonLock = 1;
        retCode = CC_Status(DoorState, FLOOR_3);
        PTS ^= LED_BITS_MASK;
      }
      
    if (PTJ == BUTTON_IS_PRESSED) //door open button pressed
      {
        DoorState = OPEN;
        SET_BITS(PORTB, DOOR_OPEN_LED_MASK);    
      }
      
    if (PTJ == BUTTON2_IS_PRESSED) //door close button pressed
      {
        DoorState = CLOSED;
        CLR_BITS(PORTB, DOOR_CLOSED_LED_MASK);   
      }  
      
  
  if (MSCAN_GotMsg()) //received message from EC
      {          
      msg = MSCAN_Getd();
      floorLocation = msg.data[0];
      
      if ((floorLocation & FLOOR_LOCATION_MASK) == FLOOR_1) //car is at floor one
        {
          FORCE_BITS(PORTA, LEDS_PORT_MASK, FLOOR1_LED_MASK); 
          PORTA &= ~FLOOR1_REQUEST_LED_MASK;
          CLR_BITS(PORTB, FLOOR1_REQUEST_LED_MASK);
          floor1ButtonLock = 0;
          PTS ^= LED_BITS_MASK;          
        }
        else if ((floorLocation & FLOOR_LOCATION_MASK) == FLOOR_2) //car is at floor two
        {
          FORCE_BITS(PORTA, LEDS_PORT_MASK, FLOOR2_LED_MASK);          
          PORTA &= ~FLOOR2_REQUEST_LED_MASK;
          CLR_BITS(PORTB, FLOOR2_REQUEST_LED_MASK);
          floor2ButtonLock = 0;
          PTS ^= LED_BITS_MASK;          
        }
        else if ((floorLocation & FLOOR_LOCATION_MASK) == FLOOR_3) //car is at floor three
        {
          FORCE_BITS(PORTA, LEDS_PORT_MASK, FLOOR3_LED_MASK);
          PORTA &= ~FLOOR3_REQUEST_LED_MASK;
          CLR_BITS(PORTB, FLOOR3_REQUEST_LED_MASK);
          floor3ButtonLock = 0;
          PTS ^= LED_BITS_MASK;          
        }          
     }
  }  
}
