float hum;  
float temp; 
float cm; 
int smoke;
String DATA;
int waterPresent = 0 ;
int flameValue=0;
int ultrasonic;
boolean open;
boolean test;
void setup()
{
  open = true;
  test = true;
}
void loop()
{
  while (test){
    Serial.println("Starting test 1/5: testcheckDataThresholdsNormal");
    if (testDataThresholdsNormal()){
      Serial.println("PASS");
      }
     else {Serial.println("FAIL"); test=false; break;}
     Serial.println("Starting test 2/5: testcreateAndFormatData");
    if (testcreateAndFormatData()){
      Serial.println("PASS");
      }
     else {Serial.println("FAIL"); test=false; break;}
     Serial.println("Starting test 3/5: testcheckDataThresholdsHighTemp");
    if (testcheckDataThresholdsHighTemp()){
      Serial.println("PASS");
      }
     else {Serial.println("FAIL"); test=false; break;}
     Serial.println("Starting test 4/5: testsendAlarm");
     String alarmType = checkDataThresholds();  
     if (testsendAlarm(alarmType)){
      Serial.println("PASS");
      }
     else {Serial.println("FAIL"); test=false; break;}
     Serial.println("Starting test 5/5: testcheckDataThresholdLowTemp");
     if (testcheckDataThresholdLowTemp()){
      Serial.println("PASS");
      Serial.println("ALL TESTS PASSED");
      }
     else {Serial.println("FAIL"); test=false; break;}
}
}
boolean testDataThresholdsNormal(){
    getFlame(0);
    getSmoke(0);
    getWater(0);
    getTempAndHum(24,20);
    getUltrasonic(20);
    String result = checkDataThresholds();
    if (result != "" ){
      return false;
      }
    return true;
}
boolean testcheckDataThresholdsHighTemp(){
    getFlame(0);
    getSmoke(0);
    getWater(0);
    getTempAndHum(80,20);
    getUltrasonic(20);
    String result = checkDataThresholds();
    if (result != "HIGHTEMP " ){
      return false;
      }
    return true;
}
boolean testcheckDataThresholdLowTemp(){
    getFlame(0);
    getSmoke(0);
    getWater(0);
    getTempAndHum(-10,20);
    getUltrasonic(20);
    String result = checkDataThresholds();
    if (result != "LOWTEMP " ){
      return false;
      }
    return true;
  
  
  }
boolean testcreateAndFormatData(){
  if (DATA == String("{‘temp’:" + String(temp) + ", ‘humid’:" + String(hum) +  ", ‘flame’:"+ String(flameValue) +", ‘water’:"+ String(waterPresent) +", ‘smoke’ :" +String(smoke)+ ", ‘garage’:"+ "OPEN" +", ‘ultrasonic’:" +String(cm)+ "}")){
      return true ;
    }
    return false;
  }
String checkDataThresholds(){
  String alarm = "";
  if (smoke > 350){
    alarm = alarm +  "SMOKE ";}
  if (flameValue > 100){
    alarm = alarm  + "FLAME ";}  
  if (waterPresent > 0){
    alarm = alarm + " " + "WATER ";
    }
  if (temp > 42){
    alarm = alarm  + "HIGHTEMP ";
    }
  if (temp < 0) {
    alarm = alarm +  "LOWTEMP ";
    }
  if (hum > 80){
    alarm = alarm +  "HUMIDITY ";
    }
  return alarm;
  }
boolean testsendAlarm(String Alarmtype){
  sendAlarm(Alarmtype);
  if (DATA == String("{'alarm':"+ Alarmtype + "}")){
    return true;
    }
  else{return false;}
}
void getFlame(int val){
  flameValue = val;
}
void getSmoke(int val){
  smoke = val;
}
void getWater(int val){
  waterPresent = val;  
}
void getTempAndHum(int tempval, int humval){ // need a dealy of 2 seconds between calls 
    hum = humval;
    temp= tempval;
}
void getUltrasonic(int value){
    ultrasonic = value;
  }

void sendData(){
  Serial.println(DATA);
}
void sendAlarm(String Alarmtype){
  DATA=String("{'alarm':"+ Alarmtype + "}");
  Serial.println(DATA);
}
void createAndFormatData(){
  String garageStr; 
  if (open){
    garageStr="OPEN";
  }
  else{garageStr="CLOSED";}
  DATA = String("{‘temp’:" + String(temp) + ", ‘humid’:" + String(hum) +  ", ‘flame’:"+ String(flameValue) +", ‘water’:"+ String(waterPresent) +", ‘smoke’ :" +String(smoke)+ ", ‘garage’:"+ garageStr +", ‘ultrasonic’:" +String(cm)+ "}"); 
}
