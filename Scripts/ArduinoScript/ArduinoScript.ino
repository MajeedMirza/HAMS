
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
{
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
  READSENSORS= true;
  loopCount=0;
  dht.begin();
  callibrateSmokeSensor();
}
void loop()
{
  // reached every 30 seconds
  // TODO check if recieved alarm from pi
  // TODO alarm HANDLING Sending AND recieving + BUZZ + LED
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
    sendData();
  }
  delay(1000); //Sleep for 30 SECONDS
  loopCount++;
  if (digitalRead(AlarmPin) == HIGH){
   // Serial.println("alarm from pin");
    Buzz(true);
    GlobalAlarm=true;
    }
  else if (digitalRead(AlarmPin) == LOW){
    //Serial.println("alarm gone");
    GlobalAlarm=false;
    }
 // Serial.println("checking garage, prevous signal");
 // Serial.println(previousGaragePinState);
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
void callibrateSmokeSensor(){
  float rs = 0;
  // get average data by testing 100 times  
 // Serial.println("CALLIBRATING");
  for(int x = 0 ; x < 100 ; x++)
  { digitalWrite(LED_pin, HIGH);
    rs = rs+getSmokeRs();
    delay(500);
    digitalWrite(LED_pin, LOW);
    delay(500);
  }
  rs = rs/100.0;
  SmokeRo = rs/9.80;
  
}
float getSmokeRs(){
  int sensorValue;
  sensorValue = analogRead(smokeIn);
  float sensor_volt=(float)sensorValue/1024*5.0; // 1024 is the max value that can be read from the smoke sensor due to adc
  return  (5.0-sensor_volt)/sensor_volt; //voltage devider formula allows us to omit rl values // this value is our rs
}
String checkDataThresholds(){
  String alarm = "";
  if (smoke > 200){
    alarm = alarm +  "SMOKE/GAS";}
  if (flameValue > 100){
    alarm = alarm + " " + "FLAME";}  
  if (waterPresent == HIGH){
    alarm = alarm + " " + "WATER";
    }
  if (temp > 42){
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

void getSmoke(){
  float RS = getSmokeRs(); // get resistance from reading value of smoke sensor
  long ppm = pow(10,((log10(RS/SmokeRo)-1.55)/-0.43));
  long rppm= (ppm + 50) / 100 * 100; // round to the nearest 100
  smoke = rppm;
}
void getWater(){
  waterPresent = digitalRead(WaterInPin);
}
void getTempAndHum(){ // need a dealy of 2 seconds between calls
    hum = dht.readHumidity();
    temp= dht.readTemperature();
}
void sendData(){
  Serial.println(DATA);
}
void sendAlarm(){
  Serial.println(String("{\"alarm\": \""+ alarm + "\" }"));
}
void createAndFormatData(){
  String garageStr;
  if (Garageopen){
    garageStr="OPEN";
  }
  else{garageStr="CLOSED";}
  DATA = String("{\"temp\":" + String(temp) + ", \"humid\":" + String(hum) +  ", \"flame\":"+ String(flameValue) +", \"water\":"+ String(waterPresent) +", \"smoke\" :" +String(smoke)+ ", \"garage\":"+ "\"" + garageStr + "\"" +", \"ultrasonic\":" +String(cm)+ "}");
}
void Buzz(boolean buzz){
    if (buzz){
    //tone(buzz_out,5000,2000);
    digitalWrite(buzz_out,HIGH);
    digitalWrite(LED_pin, HIGH);}
    else{
    digitalWrite(buzz_out,0);
    digitalWrite(LED_pin, LOW);}
}
void openCloseGarage(boolean GarageOpen){
    unsigned long  startTime = millis();
    if (GarageOpen) {
      while (millis()-startTime < 5000){ // run for one minute 
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
      while (millis()-startTime < 5000){ // run for one minute
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

void readUltraSonic(){
  digitalWrite(TrigPin, LOW); //request data go low then high then low
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);

  cm = pulseIn(EchoPin, HIGH) / 58.0; //The echo time is converted into cm
  cm = (int(cm * 100.0)) / 100.0; //Keep two decimal places
  
}
