#define switchPin 10

void setup()
{
  Serial.begin(9600);        // Initialize serial communication 
  pinMode(switchPin, INPUT); // switch pin 

}
 
 
void loop()
{
// when switch is pushed, display status on serial
if (digitalRead(switchPin) == HIGH){
  Serial.println("Switch pressed, Pulse in!");
}
else {
  Serial.println("Switch relaxed");
  // Delay for serial over load 
  delay(1000);
}

}