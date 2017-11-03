import serial
ser=serial.Serial("COM3",9600)
complete=""
while(True):
    data = ser.read()
    complete= complete+data
    if (data == "\n"):
        ## ADD CHECK TO MAKE SURE DATA IS VALID< IF NOt WAIT FOR NEXT PACKET, STARTED LIStening AT THW worng time
        print ("FINAL: "  + complete)
        complete=""
        ## DO SOMETHING WITH DATA HERE
