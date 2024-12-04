#include <AltSoftSerial.h>       //we have to include the SoftwareSerial library, or else we can't use it
#include <SD.h>
#include <Wire.h>
#include <SPI.h>

#include "RTClib.h"
RTC_DS1307 rtc; 

/**** Constants for SD card ****/
const int chipSelect = 10;  
File myFile;  

// AltSoftSerial myserial; // Arduino pin 9 :: Tx; pin 8 :: Rx
AltSoftSerial myserial;                      //define how the soft serial port is going to work

String inputstring = "";                              //a string to hold incoming data from the PC
String sensorstring = "";                             //a string to hold the data from the Atlas Scientific products
boolean input_string_complete = false;                //have we received all the data from the PC
boolean sensor_string_complete = false;               //have we received all the data from the Atlas Scientific product
int Co2_value;                                              //used to hold a integer number that is the Co2

#define pushswitch 7       // push switch for trigger file writing 


void setup() {                                        
  Serial.begin(9600);                                 
  myserial.begin(9600);                               
  inputstring.reserve(10);                            //set aside some bytes for receiving data from the PC
  sensorstring.reserve(30);                           //set aside some bytes for receiving data from Atlas Scientific product
  Serial.println('CO2 sensor getting started soon ...');

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
  
  rtc.begin();
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));//CR1220 Battery
  }
  DateTime now = rtc.now();
  myFile = SD.open("Co2.txt",O_CREAT | O_APPEND | O_WRITE); // File created
  Serial.print("DataFile created and opened\t ");
  Serial.print(now.timestamp(DateTime::TIMESTAMP_DATE));
  myFile.println("");
  myFile.println();myFile.println("\t\t\t\t--- Data logging --- ");myFile.print("\t\tSENSOR DATA FRAME  @  ");myFile.println(now.timestamp(DateTime::TIMESTAMP_DATE));myFile.println();
  myFile.close();
  Serial.println("\t File closed");


  /**** Set up Push Switch ****/
  pinMode(pushswitch, INPUT_PULLUP); // switch bridged to GND

}
void serialEvent() {                                  //if the hardware serial port_0 receives a char
  inputstring = Serial.readStringUntil(13);           //read the string until we see a <CR>
  input_string_complete = true;                       //set the flag used to tell if we have received a completed string from the PC
}


/* logic to comm with the senosr */

void loop() {    
  DateTime now = rtc.now();
  int Co2;
  if (input_string_complete == true) {                //if a string from the PC has been received in its entirety
    Serial.println("I/p cmd gone through");
    //Serial.println(inputstring);
    myserial.print(inputstring);                      //send that string to the Atlas Scientific product
    myserial.print('\r');                              //add a <CR> to the end of the string                                 
    }
  if (myserial.available() > 0) {                     //if we see that the Atlas Scientific product has sent a character
    //Serial.println("Sensor data Available!");
    char inchar = (char)myserial.read();              //get the char we just received
    if(inchar !='\r'){                                // '\r' ends data string and use it to decouple data packs
         sensorstring += inchar;                      //add the char to the var called sensorstring
         if (isdigit(sensorstring[0])){
            Co2 = sensorstring.toInt();
            }
        }
    
    else{                                 
      // decoupling data packs from each other
      sensorstring = "";                              // decouple readings per 
      sensor_string_complete= true;
      }

    if (sensor_string_complete == true) {                            //if a string from the Atlas Scientific product has been received in its entirety
      //Serial.println("Showing data");                  
      Serial.println("Incoming sensorstring complete!");            //send that string to the PC's serial monitor
      Serial.print("CO2 in ppm:");Serial.print(Co2); 
      Co2_value=Co2;
      Serial.println();
    }
    sensor_string_complete = false;                   //reset the flag used to tell if we have received a completed string from the Atlas Scientific product

  }
  input_string_complete=false;
  // delay to fetch new value
  //delay(sampling_delay);// 5s sampling rate is commanded  

  int pushSwitchState=digitalRead(pushswitch); // pin 7 
  if (pushSwitchState == LOW){
    delay(25); // switch debouncing
    // Open file 
    myFile = SD.open("Co2.txt",O_CREAT | O_APPEND | O_WRITE);
    // Write sesnor data on SD card
    myFile.print(now.timestamp(DateTime::TIMESTAMP_TIME));
    myFile.print("->\t");
    myFile.println(String("Co2:")+Co2_value+String("ppm"));
    // Display that data is written down the SD
    Serial.println("<-Push Switch pressed, file write triggered");
    Serial.print(String("->")+now.timestamp(DateTime::TIMESTAMP_TIME)+String("\tCo2:")+Co2_value+String("ppm"));
    Serial.println();
    myFile.close();
  }
 delay(350);
}
  
