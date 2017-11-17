#include <DHT.h>
#define DHTPIN 3
#define DHTTYPE DHT22 
DHT dht(DHTPIN, DHTTYPE);
float temp;
boolean test;
void setup() {
  Serial.begin(9600);
  dht.begin();
  test = true;
}

void loop() {
  while (test){
    Serial.println("Test Started");
    testgetTemp();
    if (temp < 30 && temp > 15){
      Serial.println(temp);
      Serial.println("PASS: temp reading valid");
      test =false;
      }
    else {Serial.println("FAIL: temp not in valid range trying again in 2s");}
  delay(2000);
  }
}

void testgetTemp(){
    temp= dht.readTemperature();
}

