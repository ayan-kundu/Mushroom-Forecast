#include <ModbusMaster.h>


// instantiate ModbusMaster object
ModbusMaster node;


void setup()
{
  // use Serial (port 0); initialize Modbus communication baud rate
  Serial.begin(9600);

  // communicate with Modbus slave ID 2 over Serial (port 0)
  Serial.print("Address is being checked... ");
  uint8_t result;
  for (int i; i<=255;i++){
    node.begin(i, Serial); // Try all address from 0 to 255 through for loop
    delay(10);
    Serial.print("  ");Serial.print(i);//Serial.print("  ");
    result = node.readHoldingRegisters(i, 2);
    if (result ==node.ku8MBSuccess){
    Serial.println(">>>");Serial.print("Correct Address is  ");Serial.print(i);
    break;
    }
  }
}


void loop()
{
  static uint32_t i;
  uint8_t j, result;
  uint16_t data[6];
  

  // i++;
  // set word 0 of TX buffer to least-significant word of counter (bits 15..0)
  // node.setTransmitBuffer(0, lowWord(i));
  
  // // set word 1 of TX buffer to most-significant word of counter (bits 31..16)
  // node.setTransmitBuffer(1, highWord(i));
  
  // // slave: write TX buffer to (2) 16-bit registers starting at register 0
  // result = node.writeMultipleRegisters(0, 2);
  
  
  // slave: read (6) 16-bit registers starting at register 2 to RX buffer
  result = node.readHoldingRegisters(0, 2); // Start address=0 ; 2 parameters are expected e.g. temp and humidity
  
  // do something with data if read is successful
    if (result == node.ku8MBSuccess) // flag to check if buffer read is success
    {
      for (j = 0; j < 6; j++)
      {
        data[j] = node.getResponseBuffer(j); // contains data 
      }
    }
}
