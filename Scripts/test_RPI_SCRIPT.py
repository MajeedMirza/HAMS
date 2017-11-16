import serial
from socket import *
import json
import requests
import datetime
from threading import Thread
from socketIO.socketIO_client import SocketIO, LoggingNamespace
from ArduinoStub import ArduinoStub
from RPI_SCRIPT import RPI_Handler
RPI_Handler = RPI_Handler()
ArduinoStub = ArduinoStub(24,30,0,0,0,0,True)

def main():
    print ("Starting Test 1/4: testcreateJSONnormal")
    if testcreateJSONnormal(ArduinoStub.getTestSensorValues(True)):
        print ("PASS")
        print ("Starting Test 2/4: testcreateJSONnormalAlarm")
    else:
        print "FAIL"
        return
    if testcreateJSONnormalAlarm(ArduinoStub.getTestAlarm(True,"testAlarm")):
        print ("PASS")
        print ("Starting Test 3/4: testcreateJSONBadValues")
    else:
        print "FAIL"
        return
    if testcreateJSONBadValues(ArduinoStub.getTestSensorValues(False)):
        print ("PASS")
        print ("Starting Test 4/4: testcreateJSONBadAlarm")
    else:
        print "FAIL"
        return
    if testcreateJSONBadAlarm(ArduinoStub.getTestAlarm(False,"testAlarm")):
        print ("PASS")
        print ("TEST DONE, All test passed")
    else:
        print "FAIL"
        return

def testcreateJSONnormal(data):
    print (data)
    json = RPI_Handler.createJSON(data)
    print (json)

    if json["temp"] == 24 and json["humid"]==30 and json['flame'] == 0 and json['water'] == 0 and json['smoke'] == 0 and json['garage'] == "True" and json['ultrasonic'] == 0 and json['id'] and json['time']:
        return True
    else:
        return False
def testcreateJSONnormalAlarm(data):

    json = RPI_Handler.createJSON(data)
    if json['alarm'] == "testAlarm":
        return True
    else:
        return False

def testcreateJSONBadValues(data):
    try:
        json = RPI_Handler.createJSON(data)
        print json
    except:
        return True
    return False

def testcreateJSONBadAlarm(data):
    try:
        json = RPI_Handler.createJSON(data)
    except:
        return True
    return False
if __name__== "__main__":
    main()