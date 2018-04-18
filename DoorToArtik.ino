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
  Serial.println("Hello");
  while ( status != WL_CONNECTED) { // Keep trying until connected 
    Serial.print("Attempting to connect to Network named: "); 
    Serial.println(ssid);                   
    // Connect to WPA/WPA2 network: 
    status = WiFi.begin(ssid, pass);
  }
  Serial.println("Connected to wifi");
  printWifiStatus();

  /*Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server, 443)) {
    Serial.println("connected to server");
    // Make a HTTP request:
    client.println("GET /search?q=arduino HTTP/1.1");
    client.println("Host: www.google.com");
    client.println("Connection: close");
    client.println();
  }*/
}
 
void loop() { 
  Serial.println("loop"); 
  ledToggle(); 
  Serial.println("making POST request"); 
  String contentType = "application/json"; 
  String AuthorizationData = "Bearer " + deviceToken; //Device Token 
  doorsensor = analogRead(A0);
  // onFire = false
  Serial.println("check1"); 
  int len = loadBuffer(doorsensor); //,onFire);   
  Serial.println("Sending data "+ String(doorsensor));  
  // push the data to the ARTIK Cloud
  Serial.println("check2");
  client.beginRequest();
  Serial.println("check3");
  client.post("/v1.1/messages"); //, contentType, buf
  Serial.println("check4");
  client.sendHeader("Authorization", AuthorizationData); 
  client.sendHeader("Content-Type", "application/json"); 
  //client.sendHeader("Content-Length", len); 
  client.endRequest();
  Serial.print("buf:");
  Serial.println(buf);
  client.print(buf);
  // read the status code and body of the response
  Serial.println("check5");
  int statusCode = client.responseStatusCode(); 
  String response = client.responseBody(); 
  Serial.print("Status code: "); 
  Serial.println(statusCode); 
  Serial.print("Response: "); 
  Serial.println(response); 
  Serial.println("Wait a bit"); 
  delay(6000); // delay 5 min 
}

int loadBuffer(float dvalue) {  //, boolean onFire ) {
  StaticJsonBuffer<200> jsonBuffer; // reserve spot in memory
  JsonObject& root = jsonBuffer.createObject(); // create root objects
  JsonObject& dataPair = root.createNestedObject("data"); // create nested objects
  dataPair["doorsensor"] = dvalue;
  root["sdid"] = deviceId;
  root["type"] = "message";
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

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
