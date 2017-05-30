// CAN.h
// Header file for HCS12 MSCAN
#ifndef _CAN_H
#define _CAN_H
#include "Macros.h"

#define CANCTL1_INIT		0b11000000	// enable module and use bus as module clock,  3rd bit is loopback enabled
#define CANBTR0_INIT 		0b00000111  // sycn jumpwidth is 1 Tq clock cycle, prescaler of 4
#define CANBTR1_INIT    0b00110010  // sampling  is set to 1 sample per, tseg1 is 3 and tseg2 4

typedef struct {
	uint_32 id;
	uint_8 length;
	uint_8 data[8];
} CANmsg_t;

void MSCAN_Init(void);
int MSCAN_Putd(CANmsg_t CANmsg, uint_8 priority);
CANmsg_t MSCAN_Getd(void);
void MSCAN_ListenForMsg( uint_32 id);
int MSCAN_GotMsg(void);
void MSCAN_SetNormMode(void);
void MSCAN_SetInitMode(void);


#endif
