#include "RTClib.h"
RTC_DS1307 rtc;


#include <SD.h>
#include <Wire.h>
/**** Constants for SD card ****/
const int chipSelect = 10;  
File myFile;  

#define pushswitch 7       // push switch for trigger file writing 
#define LED 6              // LED ON when push switch is triggered

void setup () {
  Serial.begin(9600);
  rtc.begin();

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  DateTime now = rtc.now();
  //Debug rtc update
  Serial.println();
  Serial.println(String("\t\tToday: ")+now.timestamp(DateTime::TIMESTAMP_DATE));

/*For SD card & RTC */
  Wire.begin();
  // Initialise SD card
  pinMode(10, OUTPUT);
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    return;
  }
  else{
    Serial.println("initialization of SD card done!");
  }
  //Create a new file and write on to SD card     ///////////////////////////////// This is where code gets stuck
  myFile = SD.open("DateTime.txt",O_CREAT | O_APPEND | O_WRITE); // File created

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

/*For Push Switch */
pinMode(pushswitch, INPUT_PULLUP); // switch bridged to GND
/*For LED */
digitalWrite(LED, LOW);
}

void loop() {
 DateTime now = rtc.now();
 Serial.print("Time Stamp: ");Serial.println(now.timestamp(DateTime::TIMESTAMP_TIME));
 Serial.println("<-After 2 Seconds->");
 //Full Timestamp //Serial.println(String("DateTime::TIMESTAMP_FULL:\t")+time.timestamp(DateTime::TIMESTAMP_FULL));
 //Delay 5s
 delay(2000);}