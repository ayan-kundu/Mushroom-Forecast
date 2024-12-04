#include <SoftwareSerial.h>

// Define the pins for RS485 communication
#define RX 5
#define TX 2
// enable pins 
#define RE 4
#define DE 3
 
// Request frame for the soil sensor
const byte humiditySensorRequest[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x02, 0xC4, 0x0B};
byte humiditySensorResponse[9];                  // should i define it 9 or 5 or 2             //  ? 
SoftwareSerial modbus(RX, TX); // Software serial for RS485 communication
 
void setup()
{
  Serial.begin(9600);  // Initialize serial communication for debugging
  modbus.begin(9600);  // Initialize software serial communication with 9600
  pinMode(RE, OUTPUT); // Set RE pin as output
  pinMode(DE, OUTPUT); // Set DE pin as output

}
 
 
void loop()
{
  /*Start the transmission mode for RS485 */
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);delay(10);
 
  // Send the request frame to the soil sensor
  modbus.write(humiditySensorRequest, sizeof(humiditySensorRequest));
 
  /*End the transmission mode and set to receive mode for RS485*/
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);
  delay(10);
 
  // Wait for the response from the sensor or timeout after 1 second
  unsigned long startTime = millis();
  while (modbus.available() < 9 && millis() - startTime < 1000)
  {
    delay(1); // help continue the loop 
  }
  Serial.println("Waited for response");
  //Serial.flush();                              // is it required as suggested // ?
  /**** Reading response frame and store it ****/
  if (modbus.available() >= 9) // If valid response received
  {
    // Read the response from the sensor
    byte index = 0;
    while (modbus.available() && index < 9)
    {
      humiditySensorResponse[index] = modbus.read();
      Serial.print(humiditySensorResponse[index], HEX); // Print the received byte in HEX format
      Serial.print(" ");
      index++;
    }
    Serial.println("Humidity sensor Response Read");
    /*******************************************/
    /* Parse and calculate the Moisture value */
    int Moisture_Int = int(humiditySensorResponse[5] << 8 | humiditySensorResponse[6]);
    float Moisture_Percent = Moisture_Int / 10.0;
 
    Serial.print("Moisture content: ");
    Serial.print(Moisture_Percent);
    Serial.println(" %");
    /*******************************************/
    /* Parse and calculate the Temperature value */
    int Temperature_Int = int(humiditySensorResponse[3] << 8 | humiditySensorResponse[4]);
    float Temperature_Celsius = Temperature_Int / 10.0;
 
    // Check if temperature is negative and convert accordingly
    if (Temperature_Int > 0x7FFF)
    {
      Temperature_Celsius = 0x10000 - Temperature_Int;
      Temperature_Celsius = -Temperature_Celsius / 10.0;
    }
 
    Serial.print("Temperature: ");
    Serial.print(Temperature_Celsius);
    Serial.println(" °C");
  }
  /*******************************************/
  delay(1000); // Wait for a second before the next loop iteration min 1000 is required
}