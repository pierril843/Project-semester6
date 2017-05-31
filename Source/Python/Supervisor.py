from PCANBasic import *

pcb = PCANBasic()
# enable pcanbasic verbose mode
pcb._PCANBasic__m_dllBasic.pcanlog_set(4, 0, 0)

pcb.Initialize(PCAN_USBBUS1, PCAN_BAUD_125K)
print("PCAN_DEVICE_NUMBER = " + str(pcb.GetValue(PCAN_USBBUS1, PCAN_DEVICE_NUMBER)))

buff = TPCANMsg()

buff.ID = 0x07
buff.LEN = 1
buff.DATA[0] = 0xA5

pcb.Write(PCAN_USBBUS1, buff)

pcb.Uninitialize(PCAN_USBBUS1)
