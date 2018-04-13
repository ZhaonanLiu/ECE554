// Simple sensor test programme
int sensorPin = A0;    // select the input pin for the sensor 
float sensorValue = 0;   // variable to store the value coming from the sensor 

void setup() {
  Serial.begin(9600);
  analogReadResolution(10);
  Serial.println("Hello");
}
void loop() { 
  // read the value from the sensor: 
  sensorValue = analogRead(sensorPin);
  Serial.print("sensorValue = ");
  Serial.println(sensorValue);
  delay(100);
}
