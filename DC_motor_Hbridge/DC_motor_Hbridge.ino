
int open_pin = 9;
int close_pin = 3;
int analogPin = A0;
float sensorState = 0;
boolean val = 0;

void Motor_control()
// 1 controls door open; 0 controls door close//
{ 
  if (val == 1)
  {
    digitalWrite(close_pin,LOW);
    analogWrite(open_pin,200);
  }
  else 
  {
    digitalWrite(open_pin,LOW);
    analogWrite(close_pin,200);
  }
}
void setup() {
  // put your setup code here, to run once:
//   pinMode(SENSORPIN,INPUT);
//   digitalWrite(SENSORPIN, HIGH);
    Serial.begin(9600);
    pinMode(open_pin,OUTPUT);
    pinMode(close_pin,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  sensorState = analogRead(analogPin);
//  int sensorState = digitalRead(SENSORPIN);
  Serial.print(sensorState);
  if (sensorState == 0)
  {
    digitalWrite(open_pin,LOW);
    digitalWrite(close_pin,LOW);
  }
  else
  {
    Motor_control();
  }
}
