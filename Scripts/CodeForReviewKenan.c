// include library for smoke sensor
#include <DHT.h>

// output pin for the LED
int LED_pin = 13;
// output pins for the Stepper Motor 
int Step_pin_1 = 8; 
int Step_pin_2 = 9; 
int Step_pin_3 = 10; 
int Step_pin_4 = 11;
// input pins for the Temp/Humidity sensor 
#define DHTPIN 3
#define DHTTYPE DHT22 
DHT dht(DHTPIN, DHTTYPE);
// output pin for the buzzer
int BuzzOut=5;
// input pin for the Smoke sensor
int SmokeIn = A0;
// input pins for the ultrasonic sensor
int TrigPin = 6;
int EchoPin = 7;
// input pin for the water sensor
int WaterInPin = 4;
// input pin for the flame sensor
int FlameIn=A1;
// Alarm and garage pins 
int AlarmPin = 2; 
int GaragePin = 12;

// global variables to store the sensor readings
int WaterPresent = 0;
int FlameValue=0;
float Hum;
float Temp;
float Cm;
int Smoke;
// store previous garage pin state to avoid opening and closing on same signal
int PreviousGaragePinState = LOW;
// store callibration ro value of smoke sensor
float SmokeRo;
// store data string to be sent to serial port
String DATA;
// used to indicate wether to read sensors at current loop or not
boolean READSENSORS;
// store alarm string to be sent via serial port
String Alarm;
// keep count of number of loops
int LoopCount;
// true if alarm recieved from another node 
boolean GlobalAlarm = false;
//global variable to monitor garage status
boolean Garageopen = true;

void setup(){
  // initialize serial port, pins, variables, and sensors
  Serial.begin(9600);
  pinMode(GaragePin,INPUT);
  pinMode(AlarmPin,INPUT);
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);
  pinMode(Step_pin_1, OUTPUT);
  pinMode(Step_pin_2, OUTPUT);
  pinMode(Step_pin_3, OUTPUT);
  pinMode(Step_pin_4, OUTPUT);
  pinMode(BuzzOut,OUTPUT);//Define Beep as output interface
  pinMode(FlameIn,INPUT);//Define flame as input interface
  pinMode(WaterInPin, INPUT);
  pinMode(LED_pin, OUTPUT);
  pinMode(SmokeIn, INPUT);
  READSENSORS= true;
  LoopCount=0;
  dht.begin(); // start temperature sensor
  callibrateSmokeSensor(); // callibrate the smoke sensor to obtain R0 value
}

void loop(){
  // Read sensors every 30 seconds
  if (READSENSORS){
	// get sensor readings
    getFlame();
    getSmoke();
    getWater();
    getTempAndHum();
	// check the thresholds and generate alarm if thresholds crossed
    Alarm  = checkDataThresholds();
	// create data stirng to be sent to arduino
    createAndFormatData();
    if (Alarm != ""){ // if alarm detected turn on the buzzer and send alarm string
      sendAlarm();
      buzz(true);
      }
    else {
      if (!GlobalAlarm){ // if no alarm present from self or other node turn off the buzzer
        buzz(false);}
      }
    sendData(); // send the data to the other node
  }
  delay(1000); //Sleep for 1 SECONDS
  LoopCount++;
  if (digitalRead(AlarmPin) == HIGH){ // check if alarm recieved from other nodes
   // Serial.println("Alarm from pin");
    buzz(true); 
    GlobalAlarm=true;
    }
  else if (digitalRead(AlarmPin) == LOW){ // global alarm disabled 
    //Serial.println("Alarm gone");
    GlobalAlarm=false;
    }
 // Serial.println("checking garage, prevous signal");
 // Serial.println(PreviousGaragePinState);
  if (digitalRead(GaragePin) == HIGH && PreviousGaragePinState != HIGH){ // check if open/close garage signal is recieved
    openCloseGarage(Garageopen); // open or close garage 
    Garageopen = !Garageopen; // change garage status
    PreviousGaragePinState = HIGH; // store previous garage pin status
    }
  else if (digitalRead(GaragePin) == LOW){ // set previous garage pin state
      PreviousGaragePinState = LOW; 
    }

  if (LoopCount>=10){ // CHECK SENSORS EVERY  10 sec
    LoopCount=0;
    READSENSORS= true;
    Alarm = "";
  }
  else {READSENSORS=false;}
}

void callibrateSmokeSensor(){ 
  // used to vallibrate smoke sensor in clean air when node is setup
  float rs = 0;
  // get average data by testing 100 times, with 1 sec delay between readings
  // Serial.println("CALLIBRATING");
  for(int x = 0 ; x < 100 ; x++)
  { digitalWrite(LED_pin, HIGH);
    rs = rs+getSmokeRs();
    delay(500);
    digitalWrite(LED_pin, LOW);
    delay(500); // blink LED to display that node is calibrating 
  }
  rs = rs/100.0; // find average 
  SmokeRo = rs/9.80; // devide by ratio for clean air to find ro since rs/r0 = 9.80 from the sensors data sheet
}

float getSmokeRs(){
  // find smoke sensor resistance 
  int sensorValue;
  sensorValue = analogRead(SmokeIn); // read smoke sensor input
  // conver the sensor reading to voltage value
  float sensor_volt=(float)sensorValue/1024*5.0; // 1024 is the max value that can be read from the Smoke sensor due to adc
  return  (5.0-sensor_volt)/sensor_volt; //voltage devider formula allows us to omit rl values // this value is our rs
}

String checkDataThresholds(){
  // checks global variables to thresholds and creates alarm string with the type of alarms detected
  // returns empty string if no alarms are detected
  String Alarm = "";
  if (Smoke > 200){
    Alarm = Alarm +  "SMOKE/GAS";}
  if (FlameValue > 100){
    Alarm = Alarm + " " + "FLAME";}  
  if (WaterPresent == HIGH){
    Alarm = Alarm + " " + "WATER";
    }
  if (Temp > 42){
    Alarm = Alarm + " " + "HIGH TEMP";
    }
  if (Temp < 0) {
    Alarm = Alarm + " " + "LOW TEMP";
    }
  if (Hum > 70){
    Alarm = Alarm + " " + "HUMIDITY";
    }
  return Alarm;
  }
  
void getFlame(){
  // get flame sensor reading
  FlameValue = analogRead(FlameIn);
}

void getSmoke(){
  // get smoke sensor reading and convert it to ppm value
  float RS = getSmokeRs(); // get resistance from reading value of Smoke sensor
  long ppm = pow(10,((log10(RS/SmokeRo)-1.55)/-0.43)); // eqaution found from datasheet to conver rs/ro to ppm
  long rppm= (ppm + 50) / 100 * 100; // round to the nearest 100
  Smoke = rppm;
}

void getWater(){
  //get water sensor reading
  WaterPresent = digitalRead(WaterInPin);
}

void getTempAndHum(){ 
    // need a delay of 2 seconds between calls
	// get temperature and humidity reading from the dht22 sensor
    Hum = dht.readHumidity();
	Temp= dht.readTemperature();
}

void sendData(){
	// send data to Raspberry pi by printing to serial console
	Serial.println(DATA);
}

void sendAlarm(){
	// send alarm to Raspberry pi by printing to serial console
	Serial.println(String("{\"alarm\": \""+ Alarm + "\" }"));
}

void createAndFormatData(){
	// format the data as a json string before sending it to RPI
	String garageStr;
	if (Garageopen){
		garageStr="OPEN";
	}
	else{garageStr="CLOSED";}
	DATA = String("{\"temp\":" + String(Temp) + ", \"humid\":" + String(Hum) +  ", \"flame\":"+ String(FlameValue) +", \"water\":"+ String(WaterPresent) +", \"smoke\" :" +String(Smoke)+ ", \"garage\":"+ "\"" + garageStr + "\"" +", \"ultrasonic\":" +String(Cm)+ "}");
}

void buzz(boolean Buzz){
	// enable or disable buzzers 
    if (Buzz){
    tone(BuzzOut,5000,2000);
    digitalWrite(BuzzOut,HIGH);
    digitalWrite(LED_pin, HIGH);}
    else{
    digitalWrite(BuzzOut,0);
    digitalWrite(LED_pin, LOW);}
}

void openCloseGarage(boolean GarageOpen){ 
    // opens or closes the garage depending on the input
    unsigned long  startTime = millis();
    if (GarageOpen) { // check which direction to run motor in (close/open)
      while (millis()-startTime < 10000){ // run for 10 seconds 
          digitalWrite(Step_pin_1, HIGH); digitalWrite(Step_pin_2, HIGH); digitalWrite(Step_pin_3, LOW); digitalWrite(Step_pin_4, LOW);
          delay(2.5);
          digitalWrite(Step_pin_1, LOW); digitalWrite(Step_pin_2, HIGH); digitalWrite(Step_pin_3, HIGH); digitalWrite(Step_pin_4, LOW);
          delay(2.5);
          digitalWrite(Step_pin_1, LOW); digitalWrite(Step_pin_2, LOW); digitalWrite(Step_pin_3, HIGH); digitalWrite(Step_pin_4, HIGH);
          delay(2.5);
          digitalWrite(Step_pin_1, HIGH); digitalWrite(Step_pin_2, LOW); digitalWrite(Step_pin_3, LOW); digitalWrite(Step_pin_4, HIGH);
          delay(2.5);
        }
   }
   else{
      while (millis()-startTime < 10000){ // run for 10 seconds in opposite direction
        digitalWrite(Step_pin_1, LOW); digitalWrite(Step_pin_2, LOW); digitalWrite(Step_pin_3, HIGH); digitalWrite(Step_pin_4, HIGH);
        delay(2.5);
        digitalWrite(Step_pin_1, LOW); digitalWrite(Step_pin_2, HIGH); digitalWrite(Step_pin_3, HIGH); digitalWrite(Step_pin_4, LOW);
        delay(2.5);
        digitalWrite(Step_pin_1, HIGH); digitalWrite(Step_pin_2, HIGH); digitalWrite(Step_pin_3, LOW); digitalWrite(Step_pin_4, LOW);
        delay(2.5);
        digitalWrite(Step_pin_1, HIGH); digitalWrite(Step_pin_2, LOW); digitalWrite(Step_pin_3, LOW); digitalWrite(Step_pin_4, HIGH);
        delay(2.5);
    }
  }
  //Turn off all pins when done
  digitalWrite(Step_pin_1, LOW); digitalWrite(Step_pin_2, LOW); digitalWrite(Step_pin_3, LOW); digitalWrite(Step_pin_4, LOW);
}

void readUltraSonic(){ 
  // get the ultrasonic reading and convert it to cm
  digitalWrite(TrigPin, LOW); //request data go low then high then low beofore obtaining reading
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);

  Cm = pulseIn(EchoPin, HIGH) / 58.0; //The echo time reading is converted into Cm
  Cm = (int(Cm * 100.0)) / 100.0; //Keep two decimal places and store it in global Cm variable
}