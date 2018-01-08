## Setup
- To run the hardware scripts upload the [arduino script](ArduinoScript/ArduinoScript.ino) onto the arduino after all the hardware connections are made
- Connect the Arduino to the Raspberry Pi and run the [AlarmListener](AlarmListener.py) and [RPI_SCRIPT](RPI_SCRIPT.py) scripts
- Run the [garage Handler](GarageHandler.py) script if the nodes is equipped with a motor (to simulate a garage opening)

## Run
```
python AlarmListener.py
python RPI_SCRIPT.py
python GarageHandler.py
```
or you can add the commands to a bash script in order to run all of them from the same place.
