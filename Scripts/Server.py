from socket import *
#import RPi.GPIO as GPIO

socket = socket(AF_INET, SOCK_DGRAM)
socket.bind(('', 2000))
# GPIO.setmode(GPIO.BCM)
# GPIO.setup(2, GPIO.OUT)

while True:
    message, address = socket.recvfrom(1024)
    print ("recieved packet: ")
    print (message)
    print ("message")
    print (message)
    print ("add")
    print (address)
    #reply with ack
    socket.sendto("ACK", address)
    ## if alarm recieved set alarm pin 2  to high
    #GPIO.output(2, 1)
    ## IF Alarm reset recieved set pin to low
    # GPIO.output(2, 0)