#include "DHT.h"
#define DHT_PIN 3 // The Arduino Nano pin connected to DHT11 sensor
#define DHT_TYPE DHT11

DHT dht11(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(9600);
  dht11.begin(); // initialize the sensor
}

void loop() {

  // Call DHT sesnor for Air RH and Temperature 
  // float humi = DHT()[0];float temp_C = DHT()[1];
  DHT();

  // Call CO2 sensor for CO2 concentration



  // sampling rate 
  delay(2000);
}

float DHT(){
  //uint8_t DHTvalues[2];// function is float return would be float 

  // read humidity
  float humi  = dht11.readHumidity();
  
  // read temperature as Celsius
  float temp_C = dht11.readTemperature();
  
  // check if any reads failed
  if (isnan(humi) || isnan(temp_C)) {
    Serial.println("Failed to read from DHT sensor!");
  } 
  else {
    Serial.print("Humidity: ");
    Serial.print(humi);
    Serial.print("%");

    Serial.print("  |  "); 

    Serial.print("Temperature: ");
    Serial.print(temp_C);
    Serial.println("°C");
  }
  //DHTvalues[0]= humi; //DHTvalues[1]= temp_C;
  return humi,temp_C;
}
