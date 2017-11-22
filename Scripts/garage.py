from threading import Thread
from socketIO.socketIO_client import SocketIO, LoggingNamespace
import RPi.GPIO as GPIO

def garageHandler(*args):
    print('garage', args[0]["open"])
    print "WORKING ON GARAGE"
    if "true" in args[0]["open"]: ## handling done in app to check if open or closed already
        GPIO.output(2, GPIO.HIGH)
        print "updated gpio"
    elif "false" in args[0]["open"]:
        GPIO.output(2, GPIO.HIGH)
        print "updated gpio"

##def _receive_events_thread(): ## not needed with single thread
    ##socketIO.wait()
GPIO.setmode(GPIO.BCM)
GPIO.setup(2, GPIO.OUT)
socketIO = SocketIO('172.17.82.126', 3001, LoggingNamespace)
socketIO.on('garage', garageHandler)
while True:
    socketIO.wait()
##receive_events_thread = Thread(target=_receive_events_thread)
##receive_events_thread.daemon = True
##receive_events_thread.start()
