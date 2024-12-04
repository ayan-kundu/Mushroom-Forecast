#include <SoftwareSerial.h>    //we have to include the SoftwareSerial library, or else we can't use it

/* UART Communication */
//#define tx 2                   // white sensor wire in    //define what pin rx is going to be
//#define rx 3                   // green sensor wire in    //define what pin tx is going to be

/* Software Serial using A5 A4 pins */
#define tx A5                 // white wire SCL 
#define rx A4                 // green wire SDA

SoftwareSerial myserial(rx, tx);                      //define how the soft serial port is going to work

String inputstring = "";                              //a string to hold incoming data from the PC
String sensorstring = "";                             //a string to hold the data from the Atlas Scientific products
boolean input_string_complete = false;                //have we received all the data from the PC
boolean sensor_string_complete = false;               //have we received all the data from the Atlas Scientific product
int Co2;                                              //used to hold a integer number that is the Co2

int sampling_delay=2.5*1000; // (in ms) Sampling rate=60S // sampling sensor reading =>2.5*4=10s per data pack

/* setting up comm */

void setup() {                                        
  Serial.begin(9600);                                 
  myserial.begin(9600);                               
  inputstring.reserve(10);                            //set aside some bytes for receiving data from the PC
  sensorstring.reserve(30);                           //set aside some bytes for receiving data from Atlas Scientific product
  Serial.println('CO2 sensor getting started soon ...');
}
void serialEvent() {                                  //if the hardware serial port_0 receives a char
  inputstring = Serial.readStringUntil(13);           //read the string until we see a <CR>
  input_string_complete = true;                       //set the flag used to tell if we have received a completed string from the PC
}


/* logic to comm with the senosr */

void loop() {                                         
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
       /* CO2 level logic */
          if (Co2 <400) {                               
             Serial.println("  which is Low");                       
             }
          else if(Co2 >=400 && Co2 <=1200) {                               
             Serial.println("  which is Normal");                       
             }
          else if(Co2 > 1200) {                               
             Serial.println("  which is High");                        
             }
          else{Serial.println("  out bound logic co2 val");}
       Serial.println("/n");
       }
       
       sensor_string_complete = false;                   //reset the flag used to tell if we have received a completed string from the Atlas Scientific product
    
    }
    input_string_complete=false;
    // delay to fetch new value
    //delay(sampling_delay);// 5s sampling rate is commanded   
  }
  
