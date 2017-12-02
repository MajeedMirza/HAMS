
               
  float RS_gas; // Get value of RS in a GAS
  float R0=0;                     // Get the value of R0 via in H2
  float RsRoRatio; // Get ratio RS_GAS/RS_air
void setup() {
  Serial.begin(9600);
  pinMode(A5, INPUT); 
  float cal = 0;
  float b; 
  // get average data by testing 100 times  
  for(int x = 0 ; x < 100 ; x++)
  {
    b = callibrateSmokeSensor();
    Serial.println(b);
    cal = cal+b;
    //delay(1000);
  }
  Serial.println(cal);
  R0 = cal/100.0;
  Serial.println(R0);
}
 
void loop() {
  RS_gas = getRS();
  RsRoRatio = RS_gas/R0;  // ratio = RS/R0 
  Serial.print("ro = ");
  Serial.println(R0);
  Serial.print("RS_gas = ");
  Serial.println(RS_gas);
  Serial.print("Rs/R0 = ");
  Serial.println(RsRoRatio);
  Serial.print("PPM of smoke = ");
  Serial.println(getPPM(RsRoRatio));
  Serial.print("\n\n");
  Serial.println(analogRead(A5));
  delay(1000);
}
float callibrateSmokeSensor(){

  float RS_air = getRS(); //this is rs of clean air which is ro
  return RS_air/9.80;                       

  Serial.print("R0 = ");
  Serial.println(R0);
  
 }

float getRS(){
  int sensorValue;
    sensorValue = sensorValue + analogRead(A5);
  /*-----------------------------------------------*/
  float sensor_volt=(float)sensorValue/1024*5.0; // 1024 is the max value that can be read
  return  (5.0-sensor_volt)/sensor_volt; //voltage devider formula allows us to omit rl values // this value is our rs
}

long  getPPM(float rs_ro_ratio)
{
 long ppm = pow(10,((log10(rs_ro_ratio)-1.55)/-0.43));
  long rppm= (ppm + 50) / 100 * 100; // round to the nearest 
  return rppm;
}
