#include <Wire.h>
#include <SparkFun_FS3000_Arduino_Library.h> 
FS3000 fs;


const int OpenAirReading = 578;   //calibration data 1
const int WaterReading = 270;     //calibration data 2
int soilMoistureLevel = 0;
int SoilMoisturePercentage = 0;
 
void setup() {
  Wire.begin();
  fs.setRange(AIRFLOW_RANGE_15_MPS);

  Serial.begin(115200); // open serial port, set the baud rate to 9600 bps
}
 
void loop() {

  Serial.print("FS3000 Readings- ");
  Serial.print("\tRaw:");
  Serial.print(fs.readRaw()); // note, this returns an int from 0-3686

  Serial.print("\tm/s: ");
  Serial.print(fs.readMetersPerSecond()); // note, this returns a float from 0-7.23 for the FS3000-1005, and 0-15 for the FS3000-1015 
  Serial.println("");


  //int SoilMoisture(OpenAirReading, WaterReading){
  soilMoistureLevel = analogRead(A0);  //update based on the analog Pin selected
  //Serial.println(soilMoistureLevel);
  // map received value into percentage (0-100)
  SoilMoisturePercentage = map(soilMoistureLevel, OpenAirReading, WaterReading, 0, 100);
  // Display value in logic 
  if (SoilMoisturePercentage >= 100)
  {
    Serial.println("\n Maximum - 100 %");
  }
  else if (SoilMoisturePercentage <= 0)
  {
    Serial.println("\n Minimum - 0 %");
  }
  else if (SoilMoisturePercentage > 0 && SoilMoisturePercentage < 100)
  {
    Serial.print("Soil Moisture: ");
    Serial.print(SoilMoisturePercentage);
    Serial.println("%");
  }
  //return SoilMoisturePercentage;
  //}


  /**************  sense parameters   *************/
  //SoilMoisturePercentage=SoilMoisture(OpenAirReading, WaterReading)


  // Sampling timer
  delay(1000);
}