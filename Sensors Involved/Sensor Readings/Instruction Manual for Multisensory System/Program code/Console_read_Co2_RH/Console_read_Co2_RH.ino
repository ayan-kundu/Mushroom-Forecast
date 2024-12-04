#include <SoftwareSerial.h>
#include <AltSoftSerial.h>

/**** For RH sensor ****/
/**** SoftwareSerial ****/

// Define the pins for RS485 communication
#define RX 5
#define TX 2
// enable pins 
#define RE 4
#define DE 3
 
// Request frame for the soil sensor
const byte RHSensorRequest[] = {0x02, 0x03, 0x00, 0x00, 0x00, 0x02, 0xC4, 0x38};
byte RHSensorResponse[9];                  // should i define it 9 or 5 or 2             //  ? 
SoftwareSerial modbus(RX, TX); // Software serial for RS485 communication

/**** For CO2 sensor ****/
/**** AltSoftwareSerial ****/

//define how the soft serial port is going to work
AltSoftSerial myserial; // Arduino pin 9 :: Tx; pin 8 :: Rx

String inputstring = "";                              //a string to hold incoming data from the PC
String sensorstring = "";                             //a string to hold the data from the Atlas Scientific products
boolean input_string_complete = false;                //have we received all the data from the PC
boolean sensor_string_complete = false;               //have we received all the data from the Atlas Scientific product

int Co2_value;                                              //used to hold a integer number that is the Co2
//float soil_temp;
//float moisture_percent;

void setup()
{
/*For soil sensor*/
  Serial.begin(9600);  // Initialize serial communication for debugging
  modbus.begin(9600);  // Initialize software serial communication with 9600
  
  pinMode(RE, OUTPUT); // Set RE pin as output
  pinMode(DE, OUTPUT); // Set DE pin as output

/*For CO2 sensor*/   
  myserial.begin(9600);
  inputstring.reserve(10);                            //set aside some bytes for receiving data from the PC
  sensorstring.reserve(30);                           //set aside some bytes for receiving data from Atlas Scientific product

}
/*For CO2 sensor*/
  void serialEvent() {                                  //if the hardware serial port_0 receives a char
    inputstring = Serial.readStringUntil(13);           //read the string until we see a <CR>
    input_string_complete = true;                       //set the flag used to tell if we have received a completed string from the PC
  }



void loop()

{
  // Call functions/sensors
  //float temperature_Celsius;
  //float soil_temp=temperature_Celsius;
  //float moisture_percent = soilSensor(&temperature_Celsius);
  String air_profile=RHSensor();
  //soil_moisture=moisture_percent;
  //delay(1100);
  Co2_value=CO2();

  Serial.println(String("->Co2: ")+Co2_value+String("ppm | ")+air_profile);
  //+String("% | ")+soil_temp+String("*C"));

  /*******************************************/
  delay(5000); // Wait for a second before the next loop iteration min 1000 is required
}

String RHSensor(){//float *temp
  //myserial.end();
  //modbus.begin(9600);
  float Temperature_Celsius;float Moisture_Percent;
  /*Start the transmission mode for RS485 */
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);delay(10);
 
  // Send the request frame to the soil sensor
  modbus.write(RHSensorRequest, sizeof(RHSensorRequest));
 
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
      RHSensorResponse[index] = modbus.read();
      Serial.print(RHSensorResponse[index], HEX); // Print the received byte in HEX format
      Serial.print(" ");
      index++;
    }
    Serial.println("RH sensor Response Read");
    /*******************************************/
    /* Parse and calculate the Moisture value */

    int Moisture_Int = int(RHSensorResponse[5] << 8 | RHSensorResponse[6]); // code worked with this 
    //int Moisture_Int = int(humiditySensorResponse[5] << 8 | humiditySensorResponse[6]);
    Moisture_Percent = Moisture_Int / 10.0;
 
    Serial.print("Moisture content: ");
    Serial.print(Moisture_Percent);
    Serial.println(" %");
    /*******************************************/
    /* Parse and calculate the Temperature value */

    int Temperature_Int = int(RHSensorResponse[3] << 8 | RHSensorResponse[4]); // code worked with this 
    //int Temperature_Int = int(humiditySensorResponse[3] << 8 | humiditySensorResponse[4]);
    Temperature_Celsius = Temperature_Int / 10.0;
 
    // Check if temperature is negative and convert accordingly
    if (Temperature_Int > 0x7FFF)
    {
      Temperature_Celsius = 0x10000 - Temperature_Int;
      Temperature_Celsius = -Temperature_Celsius / 10.0;
      //*temp = Temperature_Celsius;
    }
 
    Serial.print("Temperature: ");
    Serial.print(Temperature_Celsius);
    Serial.println(" °C");
    //myserial.listen();
  }
  String air_profile;
  air_profile=(String("Air Moisture: ")+String(Moisture_Percent) + String("%  ")+ String("Air temperature: ")+String(Temperature_Celsius)+String("°C | "));
  return air_profile;
  
}// value &Temperature_Celsius


int CO2(){
  //modbus.end();
  //myserial.begin(9600);
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
      Co2_value=Co2;
      Serial.println("Incoming sensorstring complete!");            //send that string to the PC's serial monitor
      Serial.print("CO2 in ppm:");Serial.print(Co2_value); 
      Serial.println();
    }
       
    sensor_string_complete = false;                   //reset the flag used to tell if we have received a completed string from the Atlas Scientific product
    
    }
  // delay to fetch new value
  //delay(sampling_delay);
  input_string_complete=false;
  //modbus.listen();
  return Co2_value;
}
