from PCANBasic import *

class Elevator:
    """docstring for Elevator."""
    DOOR        =   0
    BUSY        =   0
    DESTINATION =   0

    DISABLE_ELEVATOR    =   0x00
    ENABLE_ELEVATOR     =   0x01

    REQUEST_NONE    =   0x00
    REQUEST_F_1     =   0x01
    REQUEST_F_2     =   0x02
    REQUEST_F_3     =   0x03

    CAR_MOVING      =   0x00
    FLOOR_1         =   0x01
    FLOOR_2         =   0x02
    FLOOR_3         =   0x03

    FLOOR_MASK      =   (FLOOR_3 | FLOOR_2 | FLOOR_3)

    DOOR_OPEN       =   0x00
    DOOR_CLOSED     =   0x01

    REQUEST_CAR     =   0x01

    SC_ID       =   0x100
    EC_ID       =   0x101
    CC_ID       =   0x200
    F1_ID       =   0x201
    F2_ID       =   0x202
    F3_ID       =   0x203

    CAN_ERROR   =   0
    CAN_MSG     =   1
    CAN_TSTAMP  =   3



class Supervisor(Elevator):
    """docstring for Supervisor."""
    def __init__(self):
        self.pcb = PCANBasic()
        self.pcb.Initialize(PCAN_USBBUS1, PCAN_BAUD_125K)
        print("PCAN_DEVICE_NUMBER = " + str(self.pcb.GetValue(PCAN_USBBUS1, PCAN_DEVICE_NUMBER)))
        # enable pcanbasic verbose mode
        # pcb._PCANBasic__m_dllBasic.pcanlog_set(4, 0, 0)
        #init
        self.sendBuffer = TPCANMsg()
        self.sendBuffer.ID = Elevator.SC_ID
        self.sendBuffer.LEN = 1
        self.receiveBuffer = [TPCANStatus(),TPCANMsg(),TPCANTimestamp()]#i think
        #apply filter

    def ReadData(self):
        self.receiveBuffer = self.pcb.Read(PCAN_USBBUS1)


    def FloorRequest(self):
        if (self.receiveBuffer[Elevator.CAN_MSG].ID == Elevator.F1_ID):
            if (self.receiveBuffer[Elevator.CAN_MSG].DATA[0] == Elevator.REQUEST_CAR):
                self.sendBuffer.DATA[0] = ((Elevator.ENABLE_ELEVATOR<<2) | Elevator.REQUEST_F_1)
                self.pcb.Write(PCAN_USBBUS1,self.sendBuffer)
                return(Elevator.FLOOR_1)
        elif (self.receiveBuffer[Elevator.CAN_MSG].ID == Elevator.F2_ID):
            if (self.receiveBuffer[Elevator.CAN_MSG].DATA[0] == Elevator.REQUEST_CAR):
                self.sendBuffer.DATA[0] = ((Elevator.ENABLE_ELEVATOR<<2) | Elevator.REQUEST_F_2)
                self.pcb.Write(PCAN_USBBUS1,self.sendBuffer)
                return(Elevator.FLOOR_2)
        elif (self.receiveBuffer[Elevator.CAN_MSG].ID == Elevator.F3_ID):
            if (self.receiveBuffer[Elevator.CAN_MSG].DATA[0] == Elevator.REQUEST_CAR):
                self.sendBuffer.DATA[0] = ((Elevator.ENABLE_ELEVATOR<<2) | Elevator.REQUEST_F_3)
                self.pcb.Write(PCAN_USBBUS1,self.sendBuffer)
                return(Elevator.FLOOR_3)
        elif (self.receiveBuffer[Elevator.CAN_MSG].ID == Elevator.CC_ID):
            if ((self.receiveBuffer[Elevator.CAN_MSG].DATA[0] & Elevator.FLOOR_MASK)):
                self.sendBuffer.DATA[0] = ((Elevator.ENABLE_ELEVATOR<<2) | (self.receiveBuffer[1].DATA[0] & Elevator.FLOOR_MASK))
                if ((~self.receiveBuffer[CAN_MSG].DATA[0]) & (Elevator.DOOR_CLOSED<<2)):
                    print("WARNING: freight Car Door open detected. this is not safe! but continuen ;p")
                    self.pcb.Write(PCAN_USBBUS1,self.sendBuffer)
                    return(self.receiveBuffer[CAN_MSG].DATA[0] & Elevator.FLOOR_MASK)
        else:
            return(0)
    def AnalyzeData(self):
        if (self.receiveBuffer[self.CAN_ERROR] == PCAN_ERROR_OK):
            #no error.
            if (self.receiveBuffer[Elevator.CAN_MSG].ID == Elevator.EC_ID):#elevator update
                #if is is the elevator then we can compare if we are at the floor yet
                if ((self.receiveBuffer[Elevator.CAN_MSG].DATA[0] & Elevator.FLOOR_MASK) == self.DESTINATION):
                    if (self.BUSY == TRUE):
                        print("UPDATE:The freight elevator has reached its destination" )
                        self.BUSY = FALSE
                else:
                    print ("UPDATE:The freight elevator has passed floor " + str(self.receiveBuffer[Elevator.CAN_MSG].DATA[0] & Elevator.FLOOR_MASK) )
            if ((self.receiveBuffer[Elevator.CAN_MSG].ID == Elevator.F1_ID) or \
                (self.receiveBuffer[Elevator.CAN_MSG].ID == Elevator.F2_ID) or \
                (self.receiveBuffer[Elevator.CAN_MSG].ID == Elevator.F3_ID) or \
                (self.receiveBuffer[Elevator.CAN_MSG].ID == Elevator.CC_ID)):#request update
                if (self.BUSY == FALSE):
                    req = self.FloorRequest()
                    if (req != 0):#meaning it send a req
                        self.BUSY = TRUE
                        print("UPDATE:The freight elevator is in transit to floor " + str(req))
                        self.DESTINATION = req

TRUE        =   1
FALSE       =   0

SV = Supervisor()
while(TRUE):
    #Read messages
   
    SV.ReadData()
    # receiveBuffer will be a 3 touple. 0 being the error, 1 being the canmsg, and the third is the timestamp
    SV.AnalyzeData()

pcb.Uninitialize(PCAN_USBBUS1)
