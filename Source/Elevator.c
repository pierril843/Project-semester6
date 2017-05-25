int SC_Cmd_EC  (uint_8 Enable, uint_8 Floor){
  CANmsg_t CANmsg;

  CANmsg.id = SC_ID;
  CANmsg.data[0] = (Enable << 2) | (Floor);
  CANmsg.length = 1;

  if (MSCAN_Putd(CANmsg, MID) != 1){}
    //failed to send
    return(-1)
  }
  return(0)
}

int EC_Status  (uint_8 Status, uint_8 Position){
  CANmsg_t CANmsg;

  CANmsg.id = EC_ID;
  CANmsg.data[0] = (Status << 2) | (Position);
  CANmsg.length = 1;

  if (MSCAN_Putd(CANmsg, MID) != 1){}
    //failed to send
    return(-1)
  }
  return(0)
}

int CC_Status  (uint_8 DoorState, uint_8 FloorReq){
  CANmsg_t CANmsg;

  CANmsg.id = CC_ID;
  CANmsg.data[0] = (DoorState << 2) | (FloorReq);
  CANmsg.length = 1;

  if (MSCAN_Putd(CANmsg, MID) != 1){}
    //failed to send
    return(-1)
  }
  return(0)
}

int F1_Status  (uint_8 Status){
  CANmsg_t CANmsg;

  CANmsg.id = F1_ID;
  CANmsg.data[0] = Status;
  CANmsg.length = 1;

  if (MSCAN_Putd(CANmsg, MID) != 1){}
    //failed to send
    return(-1)
  }
  return(0)
}

int F2_Status  (uint_8 Status){
  CANmsg_t CANmsg;

  CANmsg.id = F2_ID;
  CANmsg.data[0] = Status;
  CANmsg.length = 1;

  if (MSCAN_Putd(CANmsg, MID) != 1){}
    //failed to send
    return(-1)
  }
  return(0)
}

int F3_Status  (uint_8 Status){
  CANmsg_t CANmsg;

  CANmsg.id = F3_ID;
  CANmsg.data[0] = Status;
  CANmsg.length = 1;

  if (MSCAN_Putd(CANmsg, MID) != 1){}
    //failed to send
    return(-1)
  }
  return(0)
}
//listen for SC
int EC_Init    (void){
  MSCAN_ListenForMsg(SC_ID);
}
//listen for EC
int CC_Init    (void){
  MSCAN_ListenForMsg(EC_ID);
}
//listen for EC
int F_Init     (void){
  MSCAN_ListenForMsg(EC_ID);
}
