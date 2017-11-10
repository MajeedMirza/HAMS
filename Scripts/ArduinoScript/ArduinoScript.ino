
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
int buzz_out=12;
// input pin for the smoke sensor
int smokeIn = A0;
// input pins for the ultrasonic sensor
int TrigPin = 6; 
int EchoPin = 7;
// input pin for the water sensor
int WaterInPin = 4; 
// input pin for the flame sensor
int flame_in=A1;
// global variables to store the sensor readings
int waterPresent = 0;
int flameValue=0;
float hum;
float temp;
float cm;
int smoke;
String DATA;
boolean READSENSORS;
int loopCount;
//global variable to monitor garage status
boolean open;

void setup()
{
  Serial.begin(9600);
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);
  pinMode(step_pin_1, OUTPUT);
  pinMode(step_pin_2, OUTPUT);
  pinMode(step_pin_3, OUTPUT);
  pinMode(step_pin_4, OUTPUT);
  pinMode(buzz_out,OUTPUT);//Define Beep as output interface
  pinMode(flame_in,INPUT);//Define flame as input interface
  pinMode(WaterInPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(smokeIn, INPUT);
  READSENSORS= true;
  loopCount=0;
  dht.begin();
}
void loop()
{
  // reached every 30 seconds
  // TODO check if recieved alarm from pi
  // TODO check if recieved open close garage
  // TODO alarm HANDLING Sending AND recieving + BUZZ + LED
  if (READSENSORS){
    getFlame();
    getSmoke();
    getWater();
    getTempAndHum();
    createAndFormatData();
    sendData();
  }
  delay(500); //Sleep for 30 SECONDS
  loopCount++;
  if (loopCount>=6){ // CHECK SENSORS EVERY 3 MINS
    loopCount=0;
    READSENSORS= true;
  }
  else {READSENSORS=false;}
}

void getFlame(){
  flameValue = analogRead(flame_in);
}

void getSmoke(){
  smoke = analogRead(smokeIn);
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
void sendAlarm(String Alarmtype){
  DATA=String("{'alarm':"+ Alarmtype + "}");
}
void createAndFormatData(){
  String garageStr;
  if (open){
    garageStr="OPEN";
  }
  else{garageStr="CLOSED";}
  DATA = String("{\"temp\":" + String(temp) + ", \"humid\":" + String(hum) +  ", \"flame\":"+ String(flameValue) +", \"water\":"+ String(waterPresent) +", \"smoke\" :" +String(smoke)+ ", \"garage\":"+ garageStr +", \"ultrasonic\":" +String(cm)+ "}");
}
void Buzz(boolean buzz){
  if (buzz) {
    digitalWrite(buzz_out,HIGH);
    digitalWrite(LED_pin, HIGH);
    }
  else {
    digitalWrite(buzz_out,LOW);
    digitalWrite(LED_pin, LOW);
    }
}
void openCloseGarage(boolean open){
    if (open) {
      while(readUltraSonic() >= 2){ //TODO Change this value reading, NEED A WAY TO FIGURE OUT WHEN THE GARAGE IS FULLY OPEN
        digitalWrite(LED_pin, HIGH);
        digitalWrite(step_pin_1, HIGH); digitalWrite(step_pin_2, HIGH); digitalWrite(step_pin_3, LOW); digitalWrite(step_pin_4, LOW);
        delay(2.5);
        digitalWrite(LED_pin, LOW);
        digitalWrite(step_pin_1, LOW); digitalWrite(step_pin_2, HIGH); digitalWrite(step_pin_3, HIGH); digitalWrite(step_pin_4, LOW);
        delay(2.5);
        digitalWrite(LED_pin, HIGH);
        digitalWrite(step_pin_1, LOW); digitalWrite(step_pin_2, LOW); digitalWrite(step_pin_3, HIGH); digitalWrite(step_pin_4, HIGH);
        delay(2.5);
        digitalWrite(LED_pin, LOW);
        digitalWrite(step_pin_1, HIGH); digitalWrite(step_pin_2, LOW); digitalWrite(step_pin_3, LOW); digitalWrite(step_pin_4, HIGH);
        delay(2.5);
    }
   }
   else{
     while(readUltraSonic() >= 2){ //TODO Change this value reading
        digitalWrite(LED_pin, HIGH);
        digitalWrite(step_pin_1, LOW); digitalWrite(step_pin_2, LOW); digitalWrite(step_pin_3, HIGH); digitalWrite(step_pin_4, HIGH);
        delay(2.5);
        digitalWrite(LED_pin, LOW);
        digitalWrite(step_pin_1, LOW); digitalWrite(step_pin_2, HIGH); digitalWrite(step_pin_3, HIGH); digitalWrite(step_pin_4, LOW);
        delay(2.5);
        digitalWrite(LED_pin, HIGH);
        digitalWrite(step_pin_1, HIGH); digitalWrite(step_pin_2, HIGH); digitalWrite(step_pin_3, LOW); digitalWrite(step_pin_4, LOW);
        delay(2.5);
        digitalWrite(LED_pin, LOW);
        digitalWrite(step_pin_1, HIGH); digitalWrite(step_pin_2, LOW); digitalWrite(step_pin_3, LOW); digitalWrite(step_pin_4, HIGH);
        delay(2.5);
   }
  }
}
int readUltraSonic(){
  digitalWrite(TrigPin, LOW); //request data go low then high then low
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);

  cm = pulseIn(EchoPin, HIGH) / 58.0; //The echo time is converted into cm
  cm = (int(cm * 100.0)) / 100.0; //Keep two decimal places
  return cm;
}