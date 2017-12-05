#@author Kenan El-Gaouny
from socketIO.socketIO_client import SocketIO, LoggingNamespace
import RPi.GPIO as GPIO
## this script is responsible for listening on socket io for open/close garage messages
def garageHandler(*args):
    print('garage', args[0]["open"])
    if "true" in args[0]["open"]: ## handling done on arduino to see if garage is open or closed already
        GPIO.output(2, GPIO.HIGH)
        print "updated gpio"
    elif "false" in args[0]["open"]:
        GPIO.output(2, GPIO.HIGH)

GPIO.setmode(GPIO.BCM)
GPIO.setup(2, GPIO.OUT)
socketIO = SocketIO('172.17.82.126', 3001, LoggingNamespace)
socketIO.on('garage', garageHandler)
while True:
    socketIO.wait() ## wait to recieve messafe
