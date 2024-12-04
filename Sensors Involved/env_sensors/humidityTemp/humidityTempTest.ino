#include <SoftwareSerial.h>
#include <ModbusMaster.h>

// Define RX and TX pins for software serial communication
const int RX_PIN = 5;
const int TX_PIN = 2;
#define DE_PIN 3
#define RE_PIN 4

// Define frame for querying the sensor for temperature
const byte TEMP_QUERY_FRAME[] = {0x00, 0x03, 0x00, 0x00, 0x00, 0x02, 0x84, 0x0A};

// Define frame for querying the sensor for humidity
const byte HUMIDITY_QUERY_FRAME[] = {0x00, 0x03, 0x00, 0x01, 0x00, 0x02, 0x85, 0xFA};

// Store sensor response values
byte tempResponseValues[4];                       // should i define it 4 or 5 or 2             //  ? 
byte humidityResponseValues[4];

// Store temperature and humidity variables
int temperature;
int humidity;

// Instantiate software serial communication object
SoftwareSerial rs485(RX_PIN, TX_PIN);

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  rs485.begin(4800);
  Serial.println("Serial comm set up!");
  // Enable pins to instantiate
  digitalWrite(DE_PIN, LOW);  // Disable Sensor Tx
  digitalWrite(RE_PIN, HIGH); // Disable Sensor Rx
}

void loop() {
  /*Transmit*/
  digitalWrite(RE_PIN, HIGH);                        // Disable RS485 receiver
  digitalWrite(DE_PIN, HIGH);                        // Enable RS485 transmitter
  // Send query frame to the sensor for temperature
  sendQueryFrame(TEMP_QUERY_FRAME);
  /*** Read temperature response ***/
  //.available() returns: number of bytes avaialble to read
  Serial.println(rs485.available());
  Serial.flush();// flush before reading
  if (rs485.available()) { // Assuming minimum response length is 5 bytes
    Serial.println("RS485 Available");
    /*Receive*/
    digitalWrite(DE_PIN, LOW);                        // Disable RS485 transmitter
    digitalWrite(RE_PIN, LOW);
    // Read response data
    for (int i = 0; i < 5; i++) {
      tempResponseValues[i] = rs485.read();// = rs485.readBytes();
    }
    // Extract temperature value (assuming it's in the fourth byte of the response frame)
    temperature = tempResponseValues[3];
    // Display temperature value
    Serial.print("Temperature: ");
    Serial.println(temperature);
    /*Disable Receiver*/
    digitalWrite(RE_PIN, HIGH); 
  }

  // Send query frame to the sensor for humidity
  sendQueryFrame(HUMIDITY_QUERY_FRAME);

  /*** Read humidity response ***/
  if (rs485.available() >=5) { // Assuming minimum response length is 5 bytes
    // Read response data
    for (int i = 0; i < 5; i++) {
      humidityResponseValues[i] = rs485.read();
    }
    // Extract humidity value (assuming it's in the fourth byte of the response frame)
    humidity = humidityResponseValues[4];
    // Display humidity value
    Serial.print("Humidity: ");
    Serial.println(humidity);
  }

  delay(2000); // Delay for 1 second before querying again
}

// Send query frame to the sensor
void sendQueryFrame(const byte frame[]) {
  // Calculate CRC checksum
  byte crc = calculateCRC(frame, sizeof(frame) - 1); // Subtracting last two CRC bytes

  // Send frame data
  rs485.write(frame, sizeof(frame));
  rs485.write(crc);
}

// Calculate CRC checksum
byte calculateCRC(const byte data[], byte length) {
  unsigned int crc = 0xFFFF;

  for (byte i = 0; i < length; i++) {
    crc ^= data[i];
    for (byte j = 0; j < 8; j++) {
      if (crc & 0x0001) {
        crc >>= 1;
        crc ^= 0xA001;
      } else {
        crc >>= 1;
      }
    }
  }

  return crc;
}