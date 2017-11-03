
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
// global variables
int waterPresent = 0 ;
int flameValue=0;
float hum;  
float temp; 
float cm; 
int k = 0;
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
  dht.begin();
}

void loop() 

{

// Test Motor and LED
  if (k!=1){
    for (int i=0; i < 525; i++){
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
   k=1;
  }
// Test buzzer and Flame sensor
  flameValue = analogRead(flame_in);
  Serial.println("Flame Reading: ");
  Serial.println(flameValue);
  if(flameValue>=600)// set threshold for alarm 
  { digitalWrite(buzz_out,HIGH);
    digitalWrite(LED_pin, HIGH);
  }
 else { 
  digitalWrite(buzz_out,LOW); 
  digitalWrite(LED_pin, LOW);} 
 delay(500);
  
// test Ultrasonic sensor **look at the values in serials monitors
  digitalWrite(TrigPin, LOW); //request data go low then high then low
  delayMicroseconds(2); 
  digitalWrite(TrigPin, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(TrigPin, LOW); 

  cm = pulseIn(EchoPin, HIGH) / 58.0; //The echo time is converted into cm
  cm = (int(cm * 100.0)) / 100.0; //Keep two decimal places
  Serial.println("UltraSonic Reading:"); 
  Serial.print(cm); 
  Serial.println(" cm"); 
  
// test smoke sensor //look at serial
  int smoke = analogRead(smokeIn);

  Serial.println("Smoke Value: ");
  Serial.println(smoke);

// test water sensor //touch pins and look at serial
  waterPresent = digitalRead(WaterInPin);
  if (waterPresent == HIGH) {
    digitalWrite(LED_pin, HIGH);
    delay(500);
    Serial.println("Water Detected");
  }

 
// test temp and humidity sensor
    hum = dht.readHumidity();
    temp= dht.readTemperature();
    Serial.println("Humidity reading: ");
    Serial.println(hum);
    Serial.println("Temp reading in C: ");
    Serial.println(temp);
    delay(2000);
}

  

