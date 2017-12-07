#@author Kenan El-Gaouny
#include <DHT.h>
// output pin for the LED
int LED_pin = 13;
// output pins for the Stepper Motor 
int step_pin_1 = 8; 
int step_pin_2 = 9; 
int step_pin_3 = 10; 
int step_pin_4 = 11;
// input pins for the temp/humidity sensor 
#define DHTPIN 3
#define DHTTYPE DHT22 
DHT dht(DHTPIN, DHTTYPE);
// output pin for the buzzer
int buzz_out=5;
// input pin for the smoke sensor
int smokeIn = A0;
// input pins for the ultrasonic sensor
int TrigPin = 6;
int EchoPin = 7;
// input pin for the water sensor
int WaterInPin = 4;
// input pin for the flame sensor
int flame_in=A1;
// Alarm and garage pins 
int AlarmPin = 2; 
int GaragePin = 12;
// global variables to store the sensor readings
int waterPresent = 0;
int flameValue=0;
float hum;
float temp;
float cm;
long smoke;
int previousGaragePinState = LOW;
float SmokeRo;
String DATA;
boolean READSENSORS;
String alarm;
int loopCount;
boolean GlobalAlarm = false;
//global variable to monitor garage status
boolean Garageopen = true;

void setup()
{ // set up serial port and pin modes
  Serial.begin(9600);
  pinMode(GaragePin,INPUT);
  pinMode(AlarmPin,INPUT);
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);
  pinMode(step_pin_1, OUTPUT);
  pinMode(step_pin_2, OUTPUT);
  pinMode(step_pin_3, OUTPUT);
  pinMode(step_pin_4, OUTPUT);
  pinMode(buzz_out,OUTPUT);//Define Beep as output interface
  pinMode(flame_in,INPUT);//Define flame as input interface
  pinMode(WaterInPin, INPUT);
  pinMode(LED_pin, OUTPUT);
  pinMode(smokeIn, INPUT);
  // read the sensor values on first loop
  READSENSORS= true;
  // keep track of loops
  loopCount=0;
  // intitialize the temp + hum sensor
  dht.begin();
  // calibrate the smoke sensor 
  callibrateSmokeSensor();
}
void loop()
{
  if (READSENSORS){
    getFlame();
    getSmoke();
    getWater();
    getTempAndHum();
    readUltraSonic();
    alarm  = checkDataThresholds();
    createAndFormatData();
    if (alarm != ""){
      sendAlarm();
      Buzz(true);
      }
    else {
      if (!GlobalAlarm){
        Buzz(false);}
      }
    sendData(); // send final data to pi
  }
  delay(1000); //Sleep for 1 SECOND
  loopCount++;
  // check if alarm recieved from PI (alarm from other nodes)
  if (digitalRead(AlarmPin) == HIGH){
    Buzz(true);
    GlobalAlarm=true;
    }
  else if (digitalRead(AlarmPin) == LOW){
    GlobalAlarm=false;
    }
  //check if garage open/close signal recieved
  if (digitalRead(GaragePin) == HIGH && previousGaragePinState != HIGH){
    openCloseGarage(Garageopen);
    Garageopen = !Garageopen;
    previousGaragePinState = HIGH;
    }
  else if (digitalRead(GaragePin) == LOW){
      previousGaragePinState = LOW;
    }

  if (loopCount>=10){ // CHECK SENSORS EVERY  10 sec
    loopCount=0;
    READSENSORS= true;
    alarm = "";
  }
  else {READSENSORS=false;}
}

// callibrate the smoke sensor
void callibrateSmokeSensor(){
  float rs = 0;
  // get average data by testing 100 times  
  for(int x = 0 ; x < 100 ; x++)
  { digitalWrite(LED_pin, HIGH);
    rs = rs+getSmokeRs();
    delay(500);
    digitalWrite(LED_pin, LOW);
    delay(500);
  }
  rs = rs/100.0;
  SmokeRo = rs/9.80; // conver rs to ro (resistance in clean air)// equation from datasheet
  
}
// get resistance of smoke sensor
float getSmokeRs(){
  int sensorValue;
  sensorValue = analogRead(smokeIn);
  float sensor_volt=(float)sensorValue/1024*5.0; // 1024 is the max value that can be read from the smoke sensor due to adc
  return  (5.0-sensor_volt)/sensor_volt; //voltage devider formula allows us to omit rl values // this value is our rs
}
// checls data thresholds and returns alarm string in case one is crossed
String checkDataThresholds(){
  String alarm = "";
  if (smoke > 200){
    alarm = alarm +  "SMOKE/GAS";}
  if (flameValue > 100){
    alarm = alarm + " " + "FLAME";}  
  if (waterPresent == HIGH){
    alarm = alarm + " " + "WATER";
    }
  if (temp > 50){
    alarm = alarm + " " + "HIGH TEMP";
    }
  if (temp < 0) {
    alarm = alarm + " " + "LOW TEMP";
    }
  if (hum > 70){
    alarm = alarm + " " + "HUMIDITY";
    }
  return alarm;
  }
void getFlame(){
  flameValue = analogRead(flame_in);
}
// get smoke ppm value from smoke sensot
void getSmoke(){
  float RS = getSmokeRs(); // get resistance from reading value of smoke sensor
  long ppm = pow(10,((log10(RS/SmokeRo)-1.55)/-0.43));
  long rppm= (ppm + 50) / 100 * 100; // round to the nearest 100
  smoke = rppm;
}
// check if water is present
void getWater(){
  waterPresent = digitalRead(WaterInPin);
}
// read temp + hum sensor
void getTempAndHum(){ // need a dealy of 2 seconds between calls
    hum = dht.readHumidity();
    temp= dht.readTemperature();
}
// send data to pi via serial connection bt printing it
void sendData(){
  Serial.println(DATA);
}
// format and send alarm string to pi
void sendAlarm(){
  Serial.println(String("{\"alarm\": \""+ alarm + "\" }"));
}
// create data string to send to pi with sensor readings 
void createAndFormatData(){
  String garageStr;
  if (Garageopen){
    garageStr="OPEN";
  }
  else{garageStr="CLOSED";}
  DATA = String("{\"temp\":" + String(temp) + ", \"humid\":" + String(hum) +  ", \"flame\":"+ String(flameValue) +", \"water\":"+ String(waterPresent) +", \"smoke\" :" +String(smoke)+ ", \"garage\":"+ "\"" + garageStr + "\"" +", \"ultrasonic\":" +String(cm)+ "}");
}
// turn buzzer on or off // also turns on led
void Buzz(boolean buzz){
    if (buzz){
    tone(buzz_out,5000,2000);
    digitalWrite(LED_pin, HIGH);}
    else{
    digitalWrite(LED_pin, LOW);}
}

// opens or closes the garages // runs the motor for 5 seconds
void openCloseGarage(boolean GarageOpen){
    unsigned long  startTime = millis(); // used to time how long the motor is run for
    if (GarageOpen) {
      while (millis()-startTime < 5000){ // run for 5 seconds 
          digitalWrite(step_pin_1, HIGH); digitalWrite(step_pin_2, HIGH); digitalWrite(step_pin_3, LOW); digitalWrite(step_pin_4, LOW);
          delay(2.5);
          digitalWrite(step_pin_1, LOW); digitalWrite(step_pin_2, HIGH); digitalWrite(step_pin_3, HIGH); digitalWrite(step_pin_4, LOW);
          delay(2.5);
          digitalWrite(step_pin_1, LOW); digitalWrite(step_pin_2, LOW); digitalWrite(step_pin_3, HIGH); digitalWrite(step_pin_4, HIGH);
          delay(2.5);
          digitalWrite(step_pin_1, HIGH); digitalWrite(step_pin_2, LOW); digitalWrite(step_pin_3, LOW); digitalWrite(step_pin_4, HIGH);
          delay(2.5);
        }
   }
   else{
      while (millis()-startTime < 5000){ // run for 5seconds
        digitalWrite(step_pin_1, LOW); digitalWrite(step_pin_2, LOW); digitalWrite(step_pin_3, HIGH); digitalWrite(step_pin_4, HIGH);
        delay(2.5);
        digitalWrite(step_pin_1, LOW); digitalWrite(step_pin_2, HIGH); digitalWrite(step_pin_3, HIGH); digitalWrite(step_pin_4, LOW);
        delay(2.5);
        digitalWrite(step_pin_1, HIGH); digitalWrite(step_pin_2, HIGH); digitalWrite(step_pin_3, LOW); digitalWrite(step_pin_4, LOW);
        delay(2.5);
        digitalWrite(step_pin_1, HIGH); digitalWrite(step_pin_2, LOW); digitalWrite(step_pin_3, LOW); digitalWrite(step_pin_4, HIGH);
        delay(2.5);
    }
  }
  //Turn off all pins when done
  digitalWrite(step_pin_1, LOW); digitalWrite(step_pin_2, LOW); digitalWrite(step_pin_3, LOW); digitalWrite(step_pin_4, LOW);
}
//get values from ultrasonic sensor and convert to cm
void readUltraSonic(){
  digitalWrite(TrigPin, LOW); //request data go low then high then low to read the sensor
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);

  cm = pulseIn(EchoPin, HIGH) / 58.0; //The echo time is converted into cm
  cm = (int(cm * 100.0)) / 100.0; // keep only 2 decimal points 
  
}
