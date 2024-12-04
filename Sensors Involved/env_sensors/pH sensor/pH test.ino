#include <SoftwareSerial.h>

// Define the pins for RS485 communication
#define RX 5
#define TX 2
// enable pins 
#define RE 4
#define DE 3
 
// Request frame for the soil sensor
const byte phSensorRequest[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x02, 0xC4, 0x0B};
byte phSensorResponse[9];                  // should i define it 9   ( or 5 or 2 or 11)        //  ? 
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
  modbus.write(phSensorRequest, sizeof(phSensorRequest));
 
  /*End the transmission mode and set to receive mode for RS485*/
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);
  delay(10);
 
  // Wait for the response from the sensor or timeout after 1 second
  unsigned long startTime = millis();
  // while (modbus.available() < 11 && millis() - startTime < 1000)
  // {
  //   delay(1); // help continue the loop 
  // }
  //Serial.println("Waited for response");
  Serial.flush();                              // is it required as suggested // ?
  /**** Reading response frame and store it ****/
  if (modbus.available() >= 9) // If valid response received
  {
    // Read the response from the sensor
    byte index = 0;
    while (modbus.available() && index < 9)
    {
      phSensorResponse[index] = modbus.read();
      Serial.print(phSensorResponse[index], HEX); // Print the received byte in HEX format
      Serial.print(" ");
      index++;
    }
    Serial.println("pH sensor Response Read");
    /*******************************************/
    /* Parse and calculate the Moisture value */
    int pH_raw = float(phSensorResponse[7]);
    float pH = pH_raw / 100.0; // pH_raw / 10.0;
 
    Serial.print("Compost pH: ");
    Serial.print(pH);
    Serial.println("");
  }
  /*******************************************/
  delay(1000); // Wait for a second before the next loop iteration min 1000 is required
}