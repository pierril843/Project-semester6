from PCANBasic import *
import _mysql

In_transit = 0
Last_Floor = 1
Door = 2
Timestamp = 3
Last_Updated_By = 4


class Elevator:
    """docstring for Elevator."""

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
        self.DESTINATION = 0
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
        self.db = _mysql.connect("127.0.0.1","root","","elevatorDB")
        self.db.query("SELECT * FROM State")
        result = self.db.store_result()
        self.State = result.fetch_row()[0]

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
                if ((~self.receiveBuffer[Elevator.CAN_MSG].DATA[0]) & (Elevator.DOOR_CLOSED<<2)):
                    print("WARNING: freight Car Door open detected. this is not safe! but continuen ;p")
                    self.db.query("UPDATE State SET DOOR='1' ,Timestamp= NOW() ,Last_Updated_By='L'")
                else:
                    self.db.query("UPDATE State SET DOOR='0' ,Timestamp= NOW() ,Last_Updated_By='L'")
                self.pcb.Write(PCAN_USBBUS1,self.sendBuffer)
                return(self.receiveBuffer[Elevator.CAN_MSG].DATA[0] & Elevator.FLOOR_MASK)
        else:
            return(0)

    def AnalyzeCanMes(self):
        #no error.
        if (self.receiveBuffer[Elevator.CAN_MSG].ID == Elevator.EC_ID):#elevator update
            #if is is the elevator then we can compare if we are at the floor yet
            if ((self.receiveBuffer[Elevator.CAN_MSG].DATA[0] & Elevator.FLOOR_MASK) == self.DESTINATION):
                if (self.State[In_transit] == TRUE):
                    print("UPDATE:The freight elevator has reached its destination" )
                    self.db.query("UPDATE State SET Last_Floor=\'" + str(self.receiveBuffer[Elevator.CAN_MSG].DATA[0] & Elevator.FLOOR_MASK) + "\' ,In_Transit=\'0\',Timestamp= NOW() ,Last_Updated_By=\'L\'")
                    #send message to logger
            else:
                if ((self.receiveBuffer[Elevator.CAN_MSG].DATA[0] & Elevator.FLOOR_MASK != int(self.State[Last_Floor])) & (self.receiveBuffer[Elevator.CAN_MSG].DATA[0] & Elevator.FLOOR_MASK != 0)):
                    print ("UPDATE:The freight elevator has passed floor " + str(self.receiveBuffer[Elevator.CAN_MSG].DATA[0] & Elevator.FLOOR_MASK) )
                    self.db.query("UPDATE State SET Last_Floor=\'" + str(self.receiveBuffer[Elevator.CAN_MSG].DATA[0] & Elevator.FLOOR_MASK) + "\' ,Timestamp= NOW()")
                    #send message to logger
        if ((self.receiveBuffer[Elevator.CAN_MSG].ID == Elevator.F1_ID) or \
            (self.receiveBuffer[Elevator.CAN_MSG].ID == Elevator.F2_ID) or \
            (self.receiveBuffer[Elevator.CAN_MSG].ID == Elevator.F3_ID) or \
            (self.receiveBuffer[Elevator.CAN_MSG].ID == Elevator.CC_ID)):#request update
            if (self.State[In_transit] == FALSE):
                req = self.FloorRequest()
                if (req != 0):#meaning it send a req
                    self.db.query("UPDATE State SET In_Transit=\'1\' ,Timestamp= NOW(),Last_Updated_By=\'L\'")
                    #send message to logger
                    print("UPDATE:The freight elevator is in transit to floor " + str(req))
                    self.DESTINATION = req

    def GetState(self):
        self.db.query("SELECT * FROM State")
        result = self.db.store_result()
        self.State = result.fetch_row()[0]


def AnalyzeMysql (SV, WebRequests):
    if (SV.State[In_transit] == FALSE):
        if (WebRequests[0] == "Floor 1 requested the elevator"):
            SV.sendBuffer.DATA[0] = ((Elevator.ENABLE_ELEVATOR<<2) | Elevator.REQUEST_F_1)
            SV.pcb.Write(PCAN_USBBUS1,SV.sendBuffer)
            SV.db.query("UPDATE State SET In_Transit=\'1\' ,Timestamp= NOW(),Last_Updated_By=\'L\'")
            print("UPDATE:The freight elevator is in transit to floor 1")
            SV.DESTINATION = 1
        elif (WebRequests[0] == "Floor 2 requested the elevator"):
            SV.sendBuffer.DATA[0] = ((Elevator.ENABLE_ELEVATOR<<2) | Elevator.REQUEST_F_2)
            SV.pcb.Write(PCAN_USBBUS1,SV.sendBuffer)
            SV.db.query("UPDATE State SET In_Transit=\'1\' ,Timestamp= NOW(),Last_Updated_By=\'L\'")
            print("UPDATE:The freight elevator is in transit to floor 2")
            SV.DESTINATION = 2
        elif (WebRequests[0] == "Floor 3 requested the elevator"):
            SV.sendBuffer.DATA[0] = ((Elevator.ENABLE_ELEVATOR<<2) | Elevator.REQUEST_F_3)
            SV.pcb.Write(PCAN_USBBUS1,SV.sendBuffer)
            SV.db.query("UPDATE State SET In_Transit=\'1\' ,Timestamp= NOW(),Last_Updated_By=\'L\'")
            print("UPDATE:The freight elevator is in transit to floor 3")
            SV.DESTINATION = 3
        elif (WebRequests[0] == "Door open request"):
            print("WARNING: freight Car Door open detected.")
            SV.db.query("UPDATE State SET DOOR='1' ,Timestamp= NOW() ,Last_Updated_By='L'")
        elif (WebRequests[0] == "Door close request"):
            print("WARNING: freight Car Door close detected.")
            SV.db.query("UPDATE State SET DOOR='1' ,Timestamp= NOW() ,Last_Updated_By='L'")            
        return(1)
    return(0)


TRUE        =   '1'
FALSE       =   '0'

SV = Supervisor()
SV.GetState()

SV.DESTINATION = int(SV.State[Last_Floor])
while(TRUE):
    #Read messages
    SV.ReadData()
    SV.GetState()    
    if (SV.receiveBuffer[SV.CAN_ERROR] == PCAN_ERROR_OK):
        SV.AnalyzeCanMes()
    SV.db.query("SELECT * FROM Logger WHERE (Timestamp>\'"+ SV.State[Timestamp] +"\'  AND CAN_sender=\'Web\') ORDER BY Timestamp DESC")
    result = SV.db.store_result()
    WebRequests = result.fetch_row()
    if (WebRequests):
        AnalyzeMysql(SV, WebRequests[0])

pcb.Uninitialize(PCAN_USBBUS1)
