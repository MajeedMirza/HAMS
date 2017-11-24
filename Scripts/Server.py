from socket import *
import RPi.GPIO as GPIO

socket = socket(AF_INET, SOCK_DGRAM)
socket.bind(('', 2000))
GPIO.setmode(GPIO.BCM)
GPIO.setup(2, GPIO.OUT)
GPIO.output(2, 0)

while True:
    message, address = socket.recvfrom(1024)
    print ("recieved packet: ")
    print (message)
    print ("message")
    print (message)
    print ("add")
    print (address)
    #reply with ack
    socket.sendto("ACK", address) ## handshake
    if "TriggerAlarm" in message:
        GPIO.output(2, 1)
    elif "ResetAlarm" in message:
        GPIO.output(2, 0)