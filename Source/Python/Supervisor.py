from PCANBasic import *

class Elevator:
    """docstring for Elevator."""
    Floor1_ID   =   0x201
    Floor2_ID   =   0x202
    Floor3_ID   =   0x203
    Car_ID      =   0x200
    Elevator_ID =   0x101

    TRUE        =   1
    FALSE       =   0


    DOOR        =   0
    BUSY        =   0
    DESTINATION =   0



class Supervisor(Elevator):
    """docstring for Supervisor."""
    def __init__(self, arg):
        pcb = PCANBasic()
        pcb.Initialize(PCAN_USBBUS1, PCAN_BAUD_125K)
        print("PCAN_DEVICE_NUMBER = " + str(pcb.GetValue(PCAN_USBBUS1, PCAN_DEVICE_NUMBER)))
        # enable pcanbasic verbose mode
        # pcb._PCANBasic__m_dllBasic.pcanlog_set(4, 0, 0)

        sendBuffer = TPCANMsg()
        #init
        sendBuffer = TPCANMsg()
        sendBuffer.ID = 0x100
        sendBuffer.LEN = 1
        receiveBuffer = [TPCANStatus(),TPCANMsg(),TPCANTimestamp()]#i think
        #apply filter


    def FloorRequest(receiveBuffer, sendBuffer):
        if (receiveBuffer[1].ID == Floor1_ID):
            if (receiveBuffer[1].DATA[0] == 0b00000001):
                sendBuffer.DATA[0] = (0b00000100 | 0b00000001)
                pcb.Write(PCAN_USBBUS1,sendBuffer)
                receiveBufferurn(1)
        elif (receiveBuffer[1].ID == Floor2_ID):
            if (receiveBuffer[1].DATA[0] == 0b00000001):
                sendBuffer.DATA[0] = (0b00000100 | 0b00000010)
                pcb.Write(PCAN_USBBUS1,sendBuffer)
                receiveBufferurn(2)
        elif (receiveBuffer[1].ID == Floor3_ID):
            if (receiveBuffer[1].DATA[0] == 0b00000001):
                sendBuffer.DATA[0] = (0b00000100 | 0b00000011)
                pcb.Write(PCAN_USBBUS1,sendBuffer)
                receiveBufferurn(3)
        elif (receiveBuffer[1].ID == Car_ID):
            if ((receiveBuffer[1].DATA[0] & 0b00000011)):
                sendBuffer.DATA[0] = (0b00000100 | (receiveBuffer[1].DATA[0] & 0b00000011))
                if ((~receiveBuffer[1].DATA[0]) & 0b0000000100):
                    print("WARNING: freight Car Door open detected. this is not safe! but continuen ;p")
                    pcb.Write(PCAN_USBBUS1,sendBuffer)
                    receiveBufferurn(receiveBuffer[1].DATA[0] & 0b00000011)
        else:
            receiveBufferurn(0)

while(TRUE):
    #Read messages
    Supervisor.receiveBuffer = pcb.Read(PCAN_USBBUS1)
    # receiveBuffer will be a 3 touple. 0 being the error, 1 being the canmsg, and the third is the timestamp
    if (Supervisor.receiveBuffer[0] == PCAN_ERROR_OK):
        #no error
        if (Supervisor.receiveBuffer[1].ID == Elevator_ID):#elevator update
            #if is is the elevator then we can compare if we are at the floor yet
            if ((Supervisor.receiveBuffer[1].DATA[0] &0b00000011) == DESTINATION):
                if (BUSY == TRUE):
                    print("UPDATE:The freight elevator has reached its destination" )
                    BUSY = FALSE
        if ((Supervisor.receiveBuffer[1].ID == Floor1_ID) or \
            (Supervisor.receiveBuffer[1].ID == Floor2_ID) or \
            (Supervisor.receiveBuffer[1].ID == Floor3_ID) or \
            (Supervisor.receiveBuffer[1].ID == Car_ID)):#request update
            if (BUSY == FALSE):
                req = Supervisor.FloorRequest(receiveBuffer,sendBuffer)
                if (req != 0):#meaning it send a req
                    BUSY = TRUE
                    print("UPDATE:The freight elevator is in transit to floor " + str(req))
                    DESTINATION = req

pcb.Uninitialize(PCAN_USBBUS1)
