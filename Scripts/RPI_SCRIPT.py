import serial
from socket import *
import json
import requests
import datetime
from threading import Thread
from socketIO.socketIO_client import SocketIO, LoggingNamespace


ser=serial.Serial("COM3",9600)
nodeAddrList=[] ## input node ips in here
SERVERAPI = "http://172.17.79.27:3001/api"
NODE_ID = '1'
socketIO = SocketIO('172.17.79.27', 3001, LoggingNamespace)
socketIO.on('garage', GarageHandler)


def _receive_events_thread():
    socketIO.wait()

def main():
    ## start garage monitor listner thread
    receive_events_thread = Thread(target=_receive_events_thread)
    receive_events_thread.daemon = True
    receive_events_thread.start()
    while True:
        values=getSensorData() ## get the sensor data
        if "alarm" in values:
            sendAlarm(values)
        else:
            try:
                data=createJSON(values)
                sendJSON(data)
            except ValueError:
                print("BAD JSON IGNORING PACKET")

def getSensorData():
    complete = ""
    while (True):
        data = ser.read()
        complete = complete + data
        if (data == "\n"):
            ## ADD CHECK TO MAKE SURE DATA IS VALID< IF NOt WAIT FOR NEXT PACKET, STARTED LIStening AT THW worng time
            return (complete)

def createJSON(data):
    dataDict = json.loads(data)
    dataDict['time'] = timestamp()
    dataDict['id'] = NODE_ID

def sendJSON(data):
    response = requests.post(SERVERAPI, json=data)
    ## maybe need to add error handling
def GarageHandler(*args):
    ## TODO IF below values is rrue sound an alarm set a global variable
    args[0]["open"]
    return

def opencloseGarage(Boolean):
    ##TODO
    return

def sendAlarm(data):
    ## TODO add method to send to server
    ## TODO error handling for response
    for addr in nodeAddrList:
        socket = socket(AF_INET, SOCK_DGRAM)
        socket.settimeout(1)
        i = 0
        while True:
          socket.sendto("TriggerAlarm", addr)
          ## WAIT FOR SEVER ACK HERE if not try again
          message, address = socket.recvfrom(1024)
          if "ACK" in message:
              break
          if i >= 2: ## try three times before giving up on node
              break
          i = i+1

def resetAlarm():
    for addr in nodeAddrList:
        socket = socket(AF_INET, SOCK_DGRAM)
        socket.settimeout(1)
        i=0
        while True:
          socket.sendto("ResetAlarm", addr)
          ## WAIT FOR SEVER ACK HERE if not try again
          message, address = socket.recvfrom(1024)
          if "ACK" in message:
              break
          if i >= 2: ## try three times before giving up on node
              break
          i = i+1

def checkAlarm():
    ##TODO
    return

def timestamp():
    return datetime.datetime.now().isoformat()

if __name__== "__main__":
  main()