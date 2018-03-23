//#include <Adafruit_MQTT.h>
//#include <Adafruit_MQTT_Client.h>
//#include <Adafruit_MQTT_FONA.h>

#include <MQTTClient.h>
#include <ArduinoJson.h>
//#include <SPI.h> 
#include <WiFi101.h>

const char* _SSID     = "ATT3ngY4T9 2.4G"; //Wi-Fi SSID  
const char* _PASSWORD = "james1234"; // Wi-Fi Password  
// MQTT - Artik Cloud Server params 
char Artik_Cloud_Server[]     = "api.artik.cloud"; // Server  
int  Artik_Cloud_Port         = 8883; // MQTT Port 
char Client_Name[] = "spark.iot.mkr1000-mrk1000"; // Any Name  
char Device_ID[]   = "8849802baf534dc595f2bd8e2a49e39e"; // DEVICE ID 
char Device_TOKEN[]   = "bf9079f0660e43db9e43f657850edb69"; //  DEVICE TOKEN 
char MQTT_Publish[]      = "/v1.1/messages/8849802baf534dc595f2bd8e2a49e39e"; // (/v1.1/messages/"DEVICE ID") 
char MQTT_Subscription[] = "/v1.1/actions/8849802baf534dc595f2bd8e2a49e39e"; // (/v1.1/actions/"DEVICE ID") 
char buf[50]; // Json Data to Artik Cloud  
WiFiSSLClient client; 
MQTTClient MQTT_Artik_Client; // MQTT Protocol 


//led SET 
const int LEDPin = 0; 

boolean toggle; 
int LED_state = 0;

void messageReceived(String topic, String payload, char * bytes, unsigned int length) { 
Serial.print("incoming: "); 
Serial.print(topic); 
Serial.print(" - "); 
Serial.print(payload); 
Serial.println(); 
}

void digitalWriteuser(int Pin, boolean state) 
{ 
  digitalWrite(Pin, state); 
  LED_state=state; 
}

void setup() { 
  Serial.begin(57600);   
  // Wifi Setting 
  WiFi.begin(_SSID, _PASSWORD); 
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500); 
    Serial.print("."); 
  } 
  Serial.println(""); 
  Serial.println("WiFi connected"); 
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP()); 
  MQTT_Artik_Client.begin(Artik_Cloud_Server, Artik_Cloud_Port, client); // Connect to Artik Server 
  
  while (!MQTT_Artik_Client.connect(Client_Name, Device_ID, Device_TOKEN)) { // Connect to Artik IOT Device  
    Serial.print("*"); 
    delay(1000); 
  }
  
  MQTT_Artik_Client.subscribe(MQTT_Subscription); 
  pinMode(LEDPin, OUTPUT); 

  digitalWriteuser(LEDPin, LOW);

} 


void loadBuffer(void) { 
  StaticJsonBuffer<200> jsonBuffer; 
  JsonObject& dataPair0 = jsonBuffer.createObject(); 
  //JsonObject& dataPair1 = jsonBuffer.createObject(); 
  dataPair0["enabked"] = LED_state; 

  //dataPair0["Digital_I_O"]=dataPair1; 
  dataPair0.printTo(buf, sizeof(buf)); 
  Serial.println(buf); 
} 

void loop() { 
  loadBuffer(); 
  MQTT_Artik_Client.publish(MQTT_Publish, buf);// Publishing data to the Artik Cloud  
  Serial.println("Publishing..");
  
  for (int i =0; i<100;i++){ // delay 10 Mnts ( as per Artik Free package specifications)  
    toggle = !toggle; 
    digitalWrite(LEDPin, toggle);
    delay(6000);
  }
    
} 
