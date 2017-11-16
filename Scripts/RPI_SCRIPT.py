import serial
from socket import *
import json
import requests
import datetime
from threading import Thread
from socketIO.socketIO_client import SocketIO, LoggingNamespace

class RPI_Handler:
    def __init__(self):
       # self.ser=serial.Serial("COM3",9600)
        self.nodeAddrList=[] ## input node ips in here
       # self.SERVERAPI = "http://172.17.79.27:3001/api"
        self.NODE_ID = '1'
       # self.socketIO = SocketIO('172.17.79.27', 3001, LoggingNamespace)
        #self.socketIO.on('garage', self.GarageHandler)


    def _receive_events_thread(self):
        self.socketIO.wait()

    def main(self):
        ## start garage monitor listner thread
        receive_events_thread = Thread(target=self._receive_events_thread())
        receive_events_thread.daemon = True
        receive_events_thread.start()
        while True:
            values=self.getSensorData() ## get the sensor data
            if "alarm" in values:
                self.sendAlarm(values)
            else:
                try:
                    data=self.createJSON(values)
                    self.sendJSON(data)
                except ValueError:
                    print("BAD JSON IGNORING PACKET")

    def getSensorData(self):
        complete = ""
        while (True):
            data = self.ser.read()
            complete = complete + data
            if (data == "\n"):
                ## ADD CHECK TO MAKE SURE DATA IS VALID< IF NOt WAIT FOR NEXT PACKET, STARTED LIStening AT THW worng time
                return (complete)

    def createJSON(self,data):
        try:
            dataDict = json.loads(data)
            dataDict['time'] = self.timestamp()
            dataDict['id'] = self.NODE_ID
            return  dataDict
        except:
            raise BaseException("BAD JSON")
    def sendJSON(self, data):
        response = requests.post(self.SERVERAPI, json=data)
        ## maybe need to add error handling
    def GarageHandler(self ,*args):
        ## TODO IF below values is rrue sound an alarm set a global variable
        args[0]["open"]
        return

    def opencloseGarage(self, b):
        ##TODO
        return

    def sendAlarm(self,data):
        ## TODO add method to send to server
        ## TODO error handling for response
        for addr in self.nodeAddrList:
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

    def resetAlarm(self):
        for addr in self.nodeAddrList:
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

    def checkAlarm(self):
        ##TODO
        return

    def timestamp(self):
        return datetime.datetime.now().isoformat()

    if __name__== "__main__":
      main()