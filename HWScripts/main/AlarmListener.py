from socket import *
import RPi.GPIO as GPIO
## this script keeps listening for UDP DATA indicating an alarm and triggers the alarm
socket = socket(AF_INET, SOCK_DGRAM)
socket.bind(('', 2000))
GPIO.setmode(GPIO.BCM)
GPIO.setup(3, GPIO.OUT)

while True:
    print "waiting for alarm message"
    message, address = socket.recvfrom(1024)
    print ("recieved message: ")
    print (message)
    print ("from add")
    print (address)
    #reply with ack
    socket.sendto("ACK", address)
    ## if alarm recieved set alarm pin 2  to high
    if "TriggerAlarm" in message:
        print "setting gpio high"
        GPIO.output(3, 1)
        print "gpio 3 high"
    ## IF Alarm reset recieved set pin to low
    elif "ResetAlarm" in message:
         print "reset alarm message recieved"
         GPIO.output(3, 0)
         print "gpio 3 low"