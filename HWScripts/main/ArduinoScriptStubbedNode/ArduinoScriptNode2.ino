
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

void setup()
{ // initialize serial port, pins, variables, and sensors
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
  delay(60000);
}
void loop()
{
  // Read sensors every 30 seconds
  if (READSENSORS){
    getFlame();
    getSmoke();
    getWater();
    getTempAndHum();
    Alarm  = checkDataThresholds();
    createAndFormatData();
    if (Alarm != ""){
      sendAlarm();
      buzz(true);
      }
    else {
      if (!GlobalAlarm){
        buzz(false);}
      }
    sendData();
  }
  delay(1000); //Sleep for 30 SECONDS
  LoopCount++;
  if (digitalRead(AlarmPin) == HIGH){
   // Serial.println("Alarm from pin");
    buzz(true);
    GlobalAlarm=true;
    }
  else if (digitalRead(AlarmPin) == LOW){
    //Serial.println("Alarm gone");
    GlobalAlarm=false;
    }
  if (LoopCount>=12){ // CHECK SENSORS EVERY  10 sec
    LoopCount=0;
    READSENSORS= true;
    Alarm = "";
  }
  else {READSENSORS=false;}
}
void callibrateSmokeSensor(){
  return;
}
float getSmokeRs(){
  return 100;
}
String checkDataThresholds(){
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
  FlameValue = random(0,99);
}

void getSmoke(){
  Smoke = random(0,180);;
}
void getWater(){
  WaterPresent = LOW;
}
void getTempAndHum(){ // need a dealy of 2 seconds between calls
    Hum = random(0,69);
    Temp= random(1,40);
}
void sendData(){
  Serial.println(DATA);
}
void sendAlarm(){
  Serial.println(String("{\"alarm\": \""+ Alarm + "\" }"));
}
void createAndFormatData(){
  String garageStr = "NA";
  DATA = String("{\"temp\":" + String(Temp) + ", \"humid\":" + String(Hum) +  ", \"flame\":"+ String(FlameValue) +", \"water\":"+ String(WaterPresent) +", \"smoke\" :" +String(Smoke)+ ", \"garage\":"+ "\"" + garageStr + "\"" +", \"ultrasonic\":" +String(Cm)+ "}");
}
void buzz(boolean Buzz){
    if (Buzz){
    //tone(BuzzOut,5000,2000);
    digitalWrite(BuzzOut,HIGH);
    digitalWrite(LED_pin, HIGH);}
    else{
    digitalWrite(BuzzOut,0);
    digitalWrite(LED_pin, LOW);}
}
void openCloseGarage(boolean GarageOpen){
  return;
}

int readUltraSonic(){
  return Cm;
}
