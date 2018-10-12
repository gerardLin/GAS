/*
  This a simple example of the aREST Library for the ESP8266 WiFi chip.
  This example illustrate the cloud part of aREST that makes the board accessible from anywhere
  See the README file for more details.

  Written in 2015 by Marco Schwartz under a GPL license.
*/

// Import required libraries
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <aREST.h>

// Clients
WiFiClient espClient;
PubSubClient client(espClient);

// Create aREST instance
aREST rest = aREST(client);


char* device_id="test1a2b3";
const char* ssid = "IOTAP";
const char* password = "raspberry";

// Variables to be exposed to the API
int temperature;
int humidity;

const int led = 2;
int GAS = A0;
int swPin = 5;
int led2 = 4;
int gasval;
String swtt,gatt;

// Functions
void callback(char* topic, byte* payload, unsigned int length);

void updateGas() {
  gasval= analogRead(GAS);
  if (gasval > 300)
  {
    digitalWrite(led, LOW);
    gatt="Safety";
  }
  else
    digitalWrite(led, HIGH);
    gatt="abnormal";
}

void swPin1() {
  boolean val;
  val = digitalRead(swPin);
  //Serial.print("status:");

  if (val)
  {
    digitalWrite(4, HIGH);
    //Serial.println("開門警報");
    swtt="abnormal";
  }
  else {
    digitalWrite(4, LOW);
    //Serial.println("關門");
    swtt="Safety";
  }
}

void setup(void)
{
  // Start Serial
  Serial.begin(115200);

  // Set callback
  client.setCallback(callback);

  // Init variables and expose them to REST API
  /*temperature = 24;
  humidity = 40;
  rest.variable("temperature",&temperature);
  rest.variable("humidity",&humidity);*/

  // Give name & ID to the device (ID should be 6 characters long)
  rest.set_id(device_id);
  rest.set_name("esp8266");
  rest.variable("sw",&swtt);
  rest.variable("gasdigital",&gatt);
  rest.variable("gasanalog",&gasval);
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Set output topic
  char* out_topic = rest.get_topic();

}

void loop() {

  // Connect to the cloud
  rest.handle(client);

}

// Handles message arrived on subscribed topic(s)
void callback(char* topic, byte* payload, unsigned int length) {
  updateGas();
  swPin1();
  rest.handle_callback(client, topic, payload, length);

}
