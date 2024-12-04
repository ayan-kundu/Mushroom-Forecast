#include <SD.h>
#include <Wire.h>
#include "RTClib.h"

const int chipSelect = 10;  // for the data logging shield, we use digital pin 10 for the SD cs line
File myFile;                // Declare logFile as File
RTC_DS1307 rtc;             // RTC???

#define pushswitch 7       // push switch for trigger file writing 

void setup(){
  Serial.begin(9600);
  Serial.println("Set up mode");


  /**** Check-point for RTC ****/
  Wire.begin();
  rtc.begin(); 
  //rtc.adjust(DateTime(2024,5,31,15,25,0));
  //rtc.adjust(DateTime(__DATE__,__TIME__));
  
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    //rtc.adjust(DateTime(2024,5,31,15,25,0));
    rtc.adjust(DateTime(__DATE__,__TIME__));
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
  myFile = SD.open("AndyFile.txt",FILE_WRITE); // File created
  if (myFile) {
    Serial.print("logFile created and opened");//Serial.println(filename);
  
    /**** Writing onto SD card ****/
    Serial.print("\twriting to text file");
    myFile.println("");
    myFile.println();myFile.println("\t\t\t\t--- Data logging --- ");myFile.println("\t\tSENSOR DATA FRAME");
    myFile.close();
    Serial.println("\tFile closed");
  }
  else{
    Serial.println("Error opening text file");
  }


  /**** Push Switch ****/
  pinMode(7, INPUT_PULLUP); // switch bridged to GND



}

void loop(){
  /**** rtc object initialisation ****/
  DateTime now = rtc.now();
  //Date Only //Full Timestamp
  //Serial.println(String("DateTime::TIMESTAMP_DATE:\t")+now.timestamp(DateTime::TIMESTAMP_DATE));
  Serial.println(String("TIMESTAMP:")+now.timestamp(DateTime::TIMESTAMP_TIME));
  
  /**** Senasor data collection through variables ****/
  int RH=20;



  /**** Writing Senasor data on SD ****/
  //myFile = SD.open("myFile.txt",FILE_WRITE);
  myFile = SD.open("AndyFile.txt",O_CREAT | O_APPEND | O_WRITE); // New way !!! quicker


  /**** Push switch operation ****/
  int pushSwitchState=digitalRead(pushswitch); // pin 7 
  if (pushSwitchState == LOW){
    delay (10); // switch debouncing
    // Write sesnor data on SD card
    myFile.println("");
    myFile.print(now.timestamp(DateTime::TIMESTAMP_TIME));
    myFile.print("->\t");
    myFile.print(String("RH:")+RH+String("%"));myFile.print(String("  Air temperature:")+RH+String("*C"));myFile.print(String(" CO2: ")+RH+String("ppm"));
    // Display that data is written down the SD
    Serial.println("Push Switch pressed, file write triggered");
    Serial.print(String("-> RH:")+RH+String("%"));Serial.print(String("   Air temperature:")+RH+String("*C"));Serial.println(String("  CO2: ")+RH+String("ppm"));
    // Turn LED on while pressed or Circuit it hardwarely

    
  }

  myFile.close();
  //Delay 1s
  delay(500); // depending on switch press time  Can use attach interupt 
}