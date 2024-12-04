#include <SD.h>
#include <Wire.h>
#include "RTClib.h"

const int chipSelect = 10;  // for the data logging shield, we use digital pin 10 for the SD cs line
File myFile;                // Declare logFile as File
RTC_DS1307 rtc;

#define pushswitch 7       // push switch for trigger file writing 

void setup(){
  Serial.begin(9600);
  Serial.println("Set up mode");


  /**** Check-point for RTC ****/
  Wire.begin();
  rtc.adjust(DateTime(2024,5,31,15,25,0));
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(2024,5,31,15,25,0));
  }
  Serial.println("RTC Running");


  /**** SD card initialisation check ****/
  // make sure that the default chip select pin is set to output-10 
  pinMode(10, OUTPUT);
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    return;
  }
  else{
    Serial.println("initialization of SD card done!");
  }
  /**** create a new file and write on to SD card ****/
  myFile = SD.open("myFile.txt",FILE_WRITE); // File created
  if (myFile) {
    Serial.print("logFile created and opened");//Serial.println(filename);
  
    /**** Writing onto SD card ****/
    Serial.print("\twriting to text file");
    myFile.println(" --- Data logging --- ");
    myFile.close();
    Serial.println("\tFile closed");
  }
  else{
    Serial.println("Error opening text file");
  }


  /**** Push Switch ****/
  //pinMode(7, INPUT_PULLUP); // switch bridged to GND



}

void loop(){
  /**** rtc object initialisation ****/
  DateTime time = rtc.now();
  //Date Only //Full Timestamp
  Serial.println(String("DateTime::TIMESTAMP_DATE:\t")+time.timestamp(DateTime::TIMESTAMP_DATE));
  //Serial.println(String("DateTime::TIMESTAMP_TIME:\t")+time.timestamp(DateTime::TIMESTAMP_TIME));
  

  myFile = SD.open("myFile.txt",FILE_WRITE);
  myFile.println(String("Time stamp: ")+time.timestamp(DateTime::TIMESTAMP_TIME));
  myFile.print("->");


  /**** Push switch operation ****/
  // int pushSwitchState=digitalRead(pushswitch); // pin 7 
  // if (pushSwitchState == LOW){
  //   Serial.println("Push Switch pressed file write triggered");
  //   delay (75); // switch debouncing
  //   myFile.println("This is triggered by push switch");
  // }

  myFile.close();
  //Delay 1s
  delay(1000);
}