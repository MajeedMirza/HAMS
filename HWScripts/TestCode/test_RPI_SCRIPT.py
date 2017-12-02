from ArduinoStub import ArduinoStub
from RPI_SCRIPT import RPI_Handler
RPI_Handler = RPI_Handler() ## intiate object for rpi script
ArduinoStub = ArduinoStub(24,30,0,0,0,0,True) ## initialize arduino stub class
## main function to run the tests
def main():
    print ("Starting Test 1/4: testcreateJSONnormal")
    if testcreateJSONnormal(ArduinoStub.getTestSensorValues(True)):
        print "PASS"
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
        print ("TEST DONE, All tests passed")
    else:
        print "FAIL"
        return

## fucntion to test behavior of createJson when value strings are provided should convert string to proper json
def testcreateJSONnormal(data):
    json = RPI_Handler.createJSON(data)

    if json["temp"] == 24 and json["humid"]==30 and json['flame'] == 0 and json['water'] == 0 and json['smoke'] == 0 and json['garage'] == "True" and json['ultrasonic'] == 0 and json['id'] and json['time']:
        return True
    else:
        return False
## fucntion to test behavior of createJson when alarm strings are provided should convert string to proper json
def testcreateJSONnormalAlarm(data):

    json = RPI_Handler.createJSON(data)
    if json['alarm'] == "testAlarm":
        return True
    else:
        return False
## fucntion to test behavior of createJson when bad values are provided i.e, badly formatted json string
def testcreateJSONBadValues(data):
    try:
        json = RPI_Handler.createJSON(data)
        print json
    except:
        return True
    return False
## fucntion to test behavior of createJson when bad alarms are provided i.e, badly formatted json string
def testcreateJSONBadAlarm(data):
    try:
        json = RPI_Handler.createJSON(data)
    except:
        return True
    return False
if __name__== "__main__":
    main()