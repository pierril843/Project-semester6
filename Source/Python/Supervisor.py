from PCANBasic import *

pcb = PCANBasic()

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


def FloorRequest(ret, buff):
    if (ret[1].ID == Floor1_ID):
        if (ret[1].DATA[0] == 0b00000001):
            buff.DATA[0] = (0b00000100 | 0b00000001)
            pcb.Write(PCAN_USBBUS1,buff)
            return(1)
    elif (ret[1].ID == Floor2_ID):
        if (ret[1].DATA[0] == 0b00000001):
            buff.DATA[0] = (0b00000100 | 0b00000010)
            pcb.Write(PCAN_USBBUS1,buff)
            return(2)
    elif (ret[1].ID == Floor3_ID):
        if (ret[1].DATA[0] == 0b00000001):
            buff.DATA[0] = (0b00000100 | 0b00000011)
            pcb.Write(PCAN_USBBUS1,buff)
            return(3)
    elif (ret[1].ID == Car_ID):
        if ((ret[1].DATA[0] & 0b00000011)):
            buff.DATA[0] = (0b00000100 | (ret[1].DATA[0] & 0b00000011))
            if ((~ret[1].DATA[0]) & 0b0000000100):
                print("WARNING: freight Car Door open detected. this is not safe! but continuen ;p")
            pcb.Write(PCAN_USBBUS1,buff)
            return(ret[1].DATA[0] & 0b00000011)
    else:
        return(0)


# enable pcanbasic verbose mode
# pcb._PCANBasic__m_dllBasic.pcanlog_set(4, 0, 0)

pcb.Initialize(PCAN_USBBUS1, PCAN_BAUD_125K)
print("PCAN_DEVICE_NUMBER = " + str(pcb.GetValue(PCAN_USBBUS1, PCAN_DEVICE_NUMBER)))

buff = TPCANMsg()

#init
buff = TPCANMsg()
buff.ID = 0x100
buff.LEN = 1
#apply filter
while(TRUE):
    #Read messages
    ret = pcb.Read(PCAN_USBBUS1)
    # ret will be a 3 touple. 0 being the error, 1 being the canmsg, and the third is the timestamp
    if (ret[0] == PCAN_ERROR_OK):
        #no error
        if (ret[1].ID == Elevator_ID):#elevator update
            #if is is the elevator then we can compare if we are at the floor yet
            i = (ret[1].DATA[0] &0b00000011)# temp var but this mask removes the en form the message 
            if ((ret[1].DATA[0] &0b00000011) == DESTINATION):
                if (BUSY == TRUE):
                    print("UPDATE:The freight elevator has reached its destination" )
                    BUSY = FALSE
        if ((ret[1].ID == Floor1_ID) or \
            (ret[1].ID == Floor2_ID) or \
            (ret[1].ID == Floor3_ID) or \
            (ret[1].ID == Car_ID)):#request update
            if (BUSY == FALSE):
                req = FloorRequest(ret,buff)
                if (req != 0):#meaning it send a req
                    BUSY = TRUE
                    print("UPDATE:The freight elevator is in transit to floor " + str(req))
                    DESTINATION = req

pcb.Uninitialize(PCAN_USBBUS1)
