#include <SD.h>
#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 rtc;

//#include "RTClib.h"        // define the Real Time Clock object
const int chipSelect = 10;   // for the data logging shield, we use digital pin 10 for the SD cs line
File logFile;                // Declare logFile as File

void setup(){
  Serial.begin(9600);
  Serial.println("Set up mode");

  /**** SD card initialisation check ****/
  // make sure that the default chip select pin is set to output-10 
  pinMode(10, OUTPUT);
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
  }
  Serial.println("card initialized!");
  
  /**** create a new file on to SD card ****/
  logFile = SD.open("test.txt",FILE_WRITE); // File created
  if (logFile) {
    Serial.print("\t File created ");//Serial.println(filename);
  }
  /**** Writing onto SD card ****/
  Wire.begin();
  logFile.println(" Writen on to SD card ");
  Serial.println("\t Written on to SD card");

  /**** Check-point for RTC ****/
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(2024,5,31,10,0,0)); // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  Serial.println("RTC Running, All set!");



}

void loop(){
  // rtc object initialisation
  DateTime time = rtc.now();
  
  //Date Only
  Serial.println(String("DateTime::TIMESTAMP_DATE:\t")+time.timestamp(DateTime::TIMESTAMP_DATE));
  //Full Timestamp
  Serial.println(String("DateTime::TIMESTAMP_TIME:\t")+time.timestamp(DateTime::TIMESTAMP_TIME));
  Serial.println("\n");


  //Delay 5s
  delay(5000);
}