#include <WiFi101.h> 
#include <ArduinoJson.h> 
#include <ArduinoHttpClient.h> 
#include <SPI.h>

// ARTIK Cloud REST endpoint 
char server[] = "api.artik.cloud";   
int port = 443; // We're using HTTPS 
// Device ID tokens
String deviceToken = "28561f1cee314f8bb81466a19b764062"; 
String deviceId = "be8e32edb937405a9558eb53b67e2266"; 
// Your wifi network 
char ssid[] = "Your wifi name";      
char pass[] = "Your wifi Password"; 
float doorsensor = 0.0; 
// boolean onFire = false; 
char buf[200]; // buffer to store the JSON to be sent to the ARTIK cloud 
const int LED = 6;
int ledState = 0;
WiFiSSLClient wifi; 
HttpClient client = HttpClient(wifi, server, port); 
int status = WL_IDLE_STATUS;

void setup() { 
  pinMode(LED,OUTPUT); 
  Serial.begin(9600); 
  while ( status != WL_CONNECTED) { // Keep trying until connected 
    Serial.print("Attempting to connect to Network named: "); 
    Serial.println(ssid);                   
    // Connect to WPA/WPA2 network: 
    status = WiFi.begin(ssid, pass); 
  } 
}
 
void loop() { 
  Serial.println("loop"); 
  ledToggle(); 
  Serial.println("making POST request"); 
  String contentType = "application/json"; 
  String AuthorizationData = "Bearer " + deviceToken; //Device Token 
  doorsensor = analogRead(A0);
  // onFire = false; 
  int len = loadBuffer(doorsensor); //,onFire);   
  Serial.println("Sending data "+ String(doorsensor));  
  // push the data to the ARTIK Cloud 
  client.beginRequest();
  client.post("/v1.1/messages"); //, contentType, buf 
  client.sendHeader("Authorization", AuthorizationData); 
  client.sendHeader("Content-Type", "application/json"); 
  client.sendHeader("Content-Length", len); 
  client.endRequest(); 
  client.print(buf); 
  // read the status code and body of the response 
  int statusCode = client.responseStatusCode(); 
  String response = client.responseBody(); 
  Serial.print("Status code: "); 
  Serial.println(statusCode); 
  Serial.print("Response: "); 
  Serial.println(response); 
  Serial.println("Wait a bit"); 
  delay(30000); // delay 5 min 
}

int loadBuffer(float dvalue) {  //, boolean onFire ) {
  StaticJsonBuffer<200> jsonBuffer; // reserve spot in memory
  JsonObject& root = jsonBuffer.createObject(); // create root objects
  root["sdid"] =  deviceId;
  root["type"] = "message";
  JsonObject& dataPair = root.createNestedObject("data"); // create nested objects
  dataPair["doorsensor"] = dvalue;
  // dataPair["onFire"] = onFire;
  root.printTo(buf, sizeof(buf)); // JSON-print to buffer
  return (root.measureLength()); // also return length
}

void ledToggle(){ 
  if (ledState == 0){ 
    digitalWrite(LED,LOW); 
    ledState = 1; 
  } else { 
    digitalWrite(LED,HIGH); 
    ledState = 0; 
  } 
} 
