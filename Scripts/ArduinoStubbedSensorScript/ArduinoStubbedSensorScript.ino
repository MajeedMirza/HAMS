float hum;  
float temp; 
float cm; 
int smoke;
String DATA;
int waterPresent = 0 ;
int flameValue=0;

void testDataThresholdsNormal(
    getFlame();
    getSmoke();
    getWater();
    getTempAndHum();
    String result = checkDataThresholds();
    if (result != expected_result)
      #throw an exception
  )
String checkDataThresholds(){
  }
void getFlame(int val){
  flameValue = val;
}
void getSmoke(int val){
  smoke = val);
}
void getWater(int val){
  waterPresent = val);   
}
void getTempAndHum(int tempval, int humval){ // need a dealy of 2 seconds between calls 
    hum = humval
    temp= tempval
}
void getUltrasonic(int value){
    ultrasonic = value
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
