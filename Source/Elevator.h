// CAN.h
// Header file for HCS12 MSCAN
#ifndef _ELEVATOR_H
#define _ELEVATOR_H
#include "CAN.h"
#include "Elevator.c"

#define DISABLE_ELEVATOR  0x00
#define ENABLE_ELEVATOR   0x01

#define REQUEST_NONE      0x00
#define REQUEST_F_1       0x01
#define REQUEST_F_2       0x02
#define REQUEST_F_3       0x03

#define CAR_MOVING        0x00
#define FLOOR_1           0x01
#define FLOOR_2           0x02
#define FLOOR_3           0x03

#define DOOR_OPEN         0x00
#define DOOR_CLOSED       0x01

#define DOOR_OPEN         0x00
#define REQUEST_CAR       0x01

#define SC_ID         0x100
#define EC_ID         0x101
#define CC_ID         0x200
#define F1_ID         0x201
#define F2_ID         0x202
#define F3_ID         0x203

#define HIGH          0x05
#define MID           0x03
#define LOW           0x01

extern int SC_Cmd_EC  (uint_8 Enable, uint_8 Floor);
extern int EC_Status  (uint_8 Status, uint_8 Position);
extern int CC_Status  (uint_8 DoorState, uint_8 FloorReq);
extern int F1_Status  (uint_8 Status);
extern int F2_Status  (uint_8 Status);
extern int F3_Status  (uint_8 Status);

extern int EC_Init    (void);
extern int CC_Init    (void);
extern int F_Init     (void);
#endif
