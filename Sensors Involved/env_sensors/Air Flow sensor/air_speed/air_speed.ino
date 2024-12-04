/* Sensing AIr flow D6F0 analogue pin and display air flow rate per minute */

// assign pins to sensors
#define sensorPin A0       // i/p pin for the ana sensor
#define outputPin 13     // o/p iff required
// variable to store the value coming from the sensor
float air_volt,air_sensor_value = 0.0;          
double air_flow,air_flow_sec,air_speed=0.0;

// necessary vars //
int sampling_delay=2*1000; // (in ms) Sampling rate=60S
int baudeRate=9600;

// sensor dimention 
const float pi=3.14159;
const float r=0.35; // in mm 
double cross_sec=(pi*sq(r)); //.38mm2
  

void setup() {
  
  Serial.begin(1*baudeRate); // baude rate 
  // declare the i/p o/p pins:
  pinMode(sensorPin, INPUT);
  pinMode(outputPin, OUTPUT);

}

void loop() {
  // read the value from the sensor:
  air_sensor_value = analogRead(sensorPin);
  float air_volt=air_sensor_value*(5.0/1023.0);
  // output operation if requitred
  //digitalWrite(LEDPin, HIGH);
  
  if (air_volt<1){
    air_flow=0;  // unit : L/min
  }
  else if (air_volt >=1 and air_volt<=1.5){
    air_flow=0.1;//graph up
  }
  else if (air_volt >1.5 and air_volt<=2){
    air_flow=0.17;//graph up
  }
  else if (air_volt >2.0 and air_volt<=2.5){
    air_flow=0.25;//graph up
  }
  else if (air_volt >2.5 and air_volt<=3.02){
    air_flow=0.4;
  }
  else if (air_volt >3.02 and air_volt<=3.5){
    air_flow=0.6;
  }
  else if (air_volt >3.5 and air_volt<=3.95){
    air_flow=0.8;
  }
  else if (air_volt >3.95 and air_volt<=4.47){
    air_flow=1.2;
  }
  else if (air_volt >4.47 and air_volt<=4.79){
    air_flow=1.6;
  }
  else if (air_volt >4.79 and air_volt<=5.0){
    air_flow=2.0;
  }
  else{
    Serial.print("Out of bound data(0-5V)");
  }
 
  
  // show air flow on screen
  Serial.print("Air sensor voltage (volt): ");Serial.print(air_volt);Serial.println("");
  Serial.print("Air flow rate (L/min): ");Serial.println(air_flow);

  /***** Convertion into m/sec from L/min ******/
  //Serial.println(air_flow);
  //float t_factor=float((1/60)*1000);
  double t_factor=16.66;
  Serial.print("time factor: ");
  Serial.print(t_factor);
  double conv_factor= (t_factor/cross_sec);
  Serial.print("  :conversion factor: ");
  Serial.println(conv_factor);
  double air_speed=(air_flow*conv_factor); // convert into speed using conv_factor
  /*
  air_flow_sec= (air_flow/60);     // L per sec
  Serial.print("air_flow/sec : ");  
  Serial.println(air_flow_sec);
  air_flow=air_flow_sec*(0.001);   // m3/sec
  //Serial.println(air_flow);
  cross_sec=(cross_sec);   // cross section in m2 from mm2
  Serial.print("cross_sec (mm2): ");
  Serial.println(cross_sec);
  air_speed=(air_flow/ cross_sec); // m/sec
  */
  Serial.print("Air velocity (m/sec): ");Serial.print(air_speed);Serial.println("");Serial.println("");
  
  // delay to fetch new value
  delay(sampling_delay);
}

/*Select Tools -> Manage Libraries...
In the "Filter your search..." box put LSM9DS1.
Scroll down to "Arduino LSM9DS1 Library" and click on the "INSTALL" button.
*/
