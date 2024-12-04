// Library for RS communication
#include <SoftwareSerial.h>
// Library for SD card
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"


/**** Constants for SD card ****/
const int chipSelect = 10;  // for the data logging shield, we use digital pin 10 for the SD cs line
File myFile;                // Declare logFile as File type
RTC_DS1307 rtc;            
#define pushswitch 7       // push switch for trigger file writing 
#define LED 6              // LED ON when push switch is triggered


/**** Constants for RS sensors ****/
float Moisture_Percent;
float Temperature_Celsius;


/**** Define the pins for RS485 communication ****/
#define RX 5
#define TX 2
// enable pins 
#define RE 4
#define DE 3
// Request frame for the soil sensor
const byte soilSensorRequest[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x02, 0xC4, 0x0B};
byte soilSensorResponse[9];                  // should i define it 9 or 5 or 2             //  ? 
SoftwareSerial modbus(RX, TX); // Software serial for RS485 communication


// Realtime clock initialisation



void setup()
{
  Serial.begin(9600);Serial.flush();
  Serial.println("Set up mode\t");
  /**** Set up for SD card ****/
  Wire.begin();
  rtc.begin(); 
  //rtc.adjust(DateTime(__DATE__,__TIME__));
  DateTime now = rtc.now();
  //Debug rtc update
  Serial.print(String("Today: ")+now.timestamp(DateTime::TIMESTAMP_DATE)+String("\t"));
  Serial.println(String("Time: ")+now.timestamp(DateTime::TIMESTAMP_TIME));

  // Initialise SD card
  pinMode(10, OUTPUT);
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    return;
  }
  else{
    Serial.println("initialization of SD card done!");
  }
  //Create a new file and write on to SD card
  myFile = SD.open("PushData.txt",O_CREAT | O_APPEND | O_WRITE); // File created
  if (myFile) {
    Serial.print("DataFile created and opened");//Serial.println(filename);
    /**** Writing onto SD card ****/
    Serial.print("\twriting to text file...  ");
    myFile.println("");
    myFile.println();myFile.println("\t\t\t\t--- Data logging --- ");myFile.print("\t\tSENSOR DATA FRAME  @  ");myFile.println(now.timestamp(DateTime::TIMESTAMP_DATE));
    myFile.close();
    Serial.println("\tFile closed");
  }
  else{
    Serial.println("Error opening text file");
  }
  
  /**** Set up Push Switch ****/
  pinMode(pushswitch, INPUT_PULLUP); // switch bridged to GND
  

  /**** Set up for RS sesnor ****/
  Serial.begin(9600);  // Initialize serial communication for debugging
  modbus.begin(9600);  // Initialize software serial communication with 9600
  pinMode(RE, OUTPUT); // Set RE pin as output
  pinMode(DE, OUTPUT); // Set DE pin as output

  /**** Setting LED to LOW ****/
  digitalWrite(LED, LOW);
}

 
void loop()
{
  //float Moisture_Percent_global,Temperature_Celsius_global;
  
  /**** activate clock and SD card file system****/
  // Start clock
  DateTime noww = rtc.now(); // It takes time to get initialised and start updating
  Serial.print("--- ");Serial.print(noww.timestamp(DateTime::TIMESTAMP_TIME));Serial.println(" ---");


  /**** Start the transmission mode for RS485 ****/
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);delay(10);
  // Send the request frame to the soil sensor
  modbus.write(soilSensorRequest, sizeof(soilSensorRequest));
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
  Serial.flush();                              // is it required as suggested // ?
  /**** Reading response frame and store it ****/
  if (modbus.available() >= 9) // If valid response received
  {
    // Read the response from the sensor
    byte index = 0;
    while (modbus.available() && index < 9)
    {
      soilSensorResponse[index] = modbus.read();
      Serial.print(soilSensorResponse[index], HEX); // Print the received byte in HEX format
      Serial.print(" ");
      index++;
    }
    Serial.println("\tSoil sensor Response Read");
    /*******************************************/
    /* Parse and calculate the Moisture value */

    //int Moisture_Int = int(soilSensorResponse[3] << 8 | soilSensorResponse[4]); // code worked with this 
    int Moisture_Int = int(soilSensorResponse[5] << 8 | soilSensorResponse[6]);
    Moisture_Percent = Moisture_Int / 10.0;
 
    Serial.print("\tMoisture content: ");
    Serial.print(Moisture_Percent);
    Serial.println(" %");
    /*******************************************/
    /* Parse and calculate the Temperature value */

    //int Temperature_Int = int(soilSensorResponse[5] << 8 | soilSensorResponse[6]); // code worked with this 
    int Temperature_Int = int(soilSensorResponse[3] << 8 | soilSensorResponse[4]);
    Temperature_Celsius = Temperature_Int / 10.0;
 
    // Check if temperature is negative and convert accordingly
    if (Temperature_Int > 0x7FFF)
    {
      Temperature_Celsius = 0x10000 - Temperature_Int;
      Temperature_Celsius = -Temperature_Celsius / 10.0;
    }
 
    Serial.print("\tTemperature: ");
    Serial.print(Temperature_Celsius);
    Serial.println(" °C");
    Serial.println();

    //Moisture_Percent_global=Moisture_Percent;
    //Temperature_Celsius_global=Temperature_Celsius;
  }


  // Debug variables
  //Serial.print(String("-> Soil moisture:")+Moisture_Percent_global+String("%"));Serial.println(String("   Soil temperature:")+Temperature_Celsius_global+String("°C"));delay(1500);
  
  /**** Push switch operation ****/
  int pushSwitchState=digitalRead(pushswitch); // pin 7 
  if (pushSwitchState == LOW){
    delay (10); // switch debouncing
    // Open file 
    myFile = SD.open("PushData.txt",O_CREAT | O_APPEND | O_WRITE);
    // Write sesnor data on SD card
    myFile.println("");
    myFile.print(noww.timestamp(DateTime::TIMESTAMP_TIME));
    myFile.print("->\t");
    myFile.print(String("Soil moisture:")+Moisture_Percent+String("%"));myFile.println(String("  Soil temperature:")+Temperature_Celsius+String("°C"));
    // Display that data is written down the SD
    Serial.println("<-Push Switch pressed, file write triggered");
    //Serial.print(String("->")+now.timestamp(DateTime::TIMESTAMP_TIME));
    Serial.print(String("->Soil moisture:")+Moisture_Percent+String("%"));Serial.println(String("  |  Soil temperature:")+Temperature_Celsius+String("°C"));
    Serial.println();
    myFile.close();

    // Turn LED on while pressed or Circuit it hardwarely
    //digitalWrite(LED,HIGH);
    //delay(1000);
    //digitalWrite(LED,LOW);
  }


  /*******************************************/
  // LED is using 1S delay 
  delay(100); // 100 is suitable syncing debouncing
}