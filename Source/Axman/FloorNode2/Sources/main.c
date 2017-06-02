#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "CAN.h"
#include "Macros.h"
#include "Elevator.h"


#define LED_BITS_MASK         (PTS_PTS2_MASK | PTS_PTS3_MASK)       // LED bits on port S
#define LED_DDR_MASK          (DDRS_DDRS2_MASK | DDRS_DDRS3_MASK)   // DDR for LED bits on port S
#define LED_BITS_INIT         PTS_PTS2_MASK     // initial value written to port S LED bits
#define FLOOR_UP_REQUESTED_LED_MASK 0b00000001 //red LED location 
#define FLOOR_DOWN_REQUESTED_LED_MASK 0b00010000 //yellow LED location
#define FLOOR3_LED_MASK 0b00001000 //floor 3 position in car LED
#define FLOOR2_LED_MASK 0b00000100 //floor 2 position in car LED
#define FLOOR1_LED_MASK 0b00000010 //floor 1 position in car LED
#define BUTTON_IS_PRESSED 0b10000000 //Switch 1 on Axman
#define BUTTON2_IS_PRESSED 0b01000000 //Switch 2 on Axman
#define LEDS_PORT_MASK 0b00001110 //PORTA LED locations 
#define FLOOR_LOCATION_MASK 0b00000011 //mask to seperate enable bit from ID in CAN messages 

uint_8 volatile floorLocation;

void main(void) {
   unsigned int volatile testVar = 0;
	int volatile i;
	CANmsg_t msg;
	uint_8 buttonLock = 0;	
	
  DDRS  |= LED_DDR_MASK; //Port S is output  
  DDRJ = 0b00000000; //PortJ is input
  DDRAD = 0b00000000; //PORTAD is input 
	DDRA = 0b11111111; //PORTA is output 
  
  ATDDIEN = 0b11111111; //enable PORTAD digital input
  PTS = LED_BITS_INIT;  //init PORTS to turn on LED1
  
  MSCAN_Init(); //initialize CAN for 125kbps
  MSCAN_ListenForMsg(EC_ID); //listen to EC messages  
  
  for (;;)
  {
    if ((PTJ == BUTTON_IS_PRESSED) && buttonLock == 0) //floor 2 up button pressed
      {
        PORTA |= FLOOR_UP_REQUESTED_LED_MASK;
        i = F2_Status(FLOOR_2);
        buttonLock = 1;
      }
    if ((PTJ == BUTTON2_IS_PRESSED) && buttonLock == 0) //floor 2 down button pressed
      {
        PORTA |= FLOOR_DOWN_REQUESTED_LED_MASK;
        i = F2_Status(REQUEST_CAR);
        buttonLock = 1;
      }
          
    if (MSCAN_GotMsg()) //received message from EC
      {
      PTS ^= LED_BITS_MASK;    
      msg = MSCAN_Getd();
      floorLocation = msg.data[0];
      
      if ((floorLocation & FLOOR_LOCATION_MASK) == FLOOR_2) //car is at floor two
        {
          FORCE_BITS(PORTA, LEDS_PORT_MASK, FLOOR2_LED_MASK); 
          PORTA &= ~(FLOOR_UP_REQUESTED_LED_MASK | FLOOR_DOWN_REQUESTED_LED_MASK);
          buttonLock = 0;          
        }
        else if ((floorLocation & FLOOR_LOCATION_MASK) == FLOOR_1) //car is at floor one
        {
          FORCE_BITS(PORTA, LEDS_PORT_MASK, FLOOR1_LED_MASK);          
        }
        else if ((floorLocation & FLOOR_LOCATION_MASK) == FLOOR_3) //car is at floor three
        {
          FORCE_BITS(PORTA, LEDS_PORT_MASK, FLOOR3_LED_MASK);          
        }          
     }
  }  
}



