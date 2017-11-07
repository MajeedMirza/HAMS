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
    def getTestSensorValues(self, goodFormat):
        if not goodFormat:
            return '{"temp":"int" , "humid":"int", "flame": "int" , "water" : int , "smoke" : int, "garage" :" + ", "ultrasonic": "int" }'
        else:
            return "{‘temp’:25, ‘hjmid’:50, ‘flame’: 0, ‘water’: 0, ‘smoke’: 0, ‘garage’: TRUE, ‘ultrasonic’: 15}"
    def getTestAlarm(self, goodFormat, alarmString):
        if not goodFormat:
            return "{‘adlaarm’: "+ alarmString + "}"
        else:
            return "{‘alarm’: "+ alarmString + "}"
