import string
class ArduinoStub:
    def __init__(self, temp, humid, smoke, water, flame, ultrasonic, garage):
      self.temp = temp
      self.humid = humid
      self.smoke = smoke
      self.water = water
      self.flame = flame
      self.garage = garage
      self.ultrasonic = ultrasonic

    ## meant to replace get sensorValues in main script
    def getTestSensorValues(self,goodFormat):
        if not goodFormat:
            return '"temp":'+ str(self.temp)+' , "humid"'+ str(self.humid) +', "flame"'+str(self.flame)+' , "water" ' + str(self.water) +' , "smoke" :'+ str(self.smoke)+', "garage" :' + str(self.garage)+', "ultrasonic": '+str(self.ultrasonic)+' }'
        else:
            return '{"temp":'+ str(self.temp)+' , "humid":'+ str(self.humid) +', "flame":'+str(self.flame)+' , "water" :' + str(self.water) +' , "smoke" :'+ str(self.smoke)+', "garage" :' + '"' + str(self.garage) + '"'+', "ultrasonic": '+str(self.ultrasonic)+' }'

    def getTestAlarm(self, goodFormat, alarmString):
        if not goodFormat:
            return '"ADlarm"' + alarmString + "}"
        else:
            return '{"alarm": "'+ alarmString + '"}'
