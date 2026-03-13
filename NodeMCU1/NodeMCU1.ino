// MobileControl.ino
// Mobile Control for Traffic Light System
// This code connects to a WiFi network and an MQTT broker to receive control messages for traffic
// lights. It manages the traffic light states based on the received messages and runs a normal traffic cycle when no emergency mode is active. When an emergency message is received, it activates the corresponding traffic light for 10 seconds before resetting to normal operation.  

#include <ESP8266WiFi.h> // WiFi library for ESP8266
#include <PubSubClient.h> // MQTT client library for ESP8266
#include <ThingSpeak.h> // ThingSpeak library for ESP8266
#include <WiFiClient.h> // WiFi client library for ESP8266

// WiFi Credentials
const char* ssid = "Redmi Note 10T 5G"; // Your WiFi SSID
const char* password = "password"; // Your WiFi Password

// ThingSpeak MQTT Credentials
const char* mqttServer = "mqtt3.thingspeak.com"; // Your MQTT Broker
const int mqttPort = 1883;  // MQTT Broker Port
const char* clientID = "OSgeFRIJJRMIJhopPAADNDc";
const char* mqttUsername = "OSgeFRIJJRMIJhopPAADNDc";
const char* mqttPassword = "L1LPuGT/SU0wNnApha0OmWuh";

// MQTT Topics
const char* publishTopic = "channels/2906713/publish";  // MQTT publish base
const char* nextControllerTopic = "next/traffic/controller";  // MQTT to NodeMCU 2

// ThingSpeak Settings
unsigned long channelID = 2906713;
const char* readAPIKey = "2OCLY33SOB45Y1Y6";
const char* writeAPIKey = "EUNG2OJAFA7SYCUF";  // Add correct write key here

// Lights (Active LOW)
const int redNorth = D1, yellowNorth = D2, greenNorth = D3;
const int redEast = D4, yellowEast = D5, greenEast = D6;
const int redSouth = D7, yellowSouth = D5, greenSouth = D9;
const int redWest = D0, yellowWest = D2, greenWest = D8;

WiFiClient espClient;
PubSubClient client(espClient);

int ambulanceCount = 0;
bool emergencyMode = false;
// Variable to store the current ambulance direction
String ambulanceDirection = "";
// Function prototypes
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi Connected!");

  ThingSpeak.begin(espClient);
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
// Connect to MQTT Broker
  while (!client.connected()) {
    if (client.connect(clientID, mqttUsername, mqttPassword)) {
      Serial.println("Connected to ThingSpeak MQTT");
      client.subscribe("channels/2906713/subscribe/fields/field1");
      client.subscribe("channels/2906713/subscribe/fields/field2");
      client.subscribe("channels/2906713/subscribe/fields/field3");
      client.subscribe("channels/2906713/subscribe/fields/field4");
    } else {
      delay(2000);
      Serial.println("Retrying MQTT connection...");
    }
  }

  setupLights();
}

void loop() {
  client.loop();
// Check for ambulance direction from ThingSpeak fields
  String newDirection = checkAmbulanceDirection();
  if (newDirection != "") {
    if (!emergencyMode) {
      emergencyMode = true;
      ambulanceDirection = newDirection;
      ambulanceCount++;
      Serial.println("Ambulance from: " + ambulanceDirection); 

      sendThingSpeakUpdate();
      sendToNextTrafficController(ambulanceDirection);

      activateEmergencyMode(ambulanceDirection);
      delay(10000);

      emergencyMode = false;
      Serial.println("Returning to normal traffic...");
    }
  }
// If no ambulance is detected, run normal traffic cycle
  if (!emergencyMode) {
    normalTrafficCycle();
  }
}

void sendThingSpeakUpdate() {
  String updatePayload = "field1=" + ambulanceDirectionToCode(ambulanceDirection);
  updatePayload += "&field6=All Directions OK";
  updatePayload += "&field8=" + String(ambulanceCount);
  client.publish(publishTopic, updatePayload.c_str());
}

String ambulanceDirectionToCode(String dir) {
  if (dir == "North") return "1";
  if (dir == "South") return "1";
  if (dir == "East") return "1";
  if (dir == "West") return "1";
  return "0";
}
// Activate the corresponding traffic light for 10 seconds based on the direction
void sendToNextTrafficController(String direction) {
  client.publish(nextControllerTopic, direction.c_str());
  Serial.println("Sent to NodeMCU 2: " + direction);
}

String checkAmbulanceDirection() {
  int field1 = ThingSpeak.readIntField(channelID, 1, readAPIKey);  // North
  int field2 = ThingSpeak.readIntField(channelID, 2, readAPIKey);  // South
  int field3 = ThingSpeak.readIntField(channelID, 3, readAPIKey);  // East
  int field4 = ThingSpeak.readIntField(channelID, 4, readAPIKey);  // West

  Serial.print("North (Field1): "); Serial.println(field1);
  Serial.print("South (Field2): "); Serial.println(field2);
  Serial.print("East (Field3): "); Serial.println(field3);
  Serial.print("West (Field4): "); Serial.println(field4);

  if (field1 == 1) return "North";
  if (field2 == 1) return "South";
  if (field3 == 1) return "East";
  if (field4 == 1) return "West";
// If no direction is active, return an empty string
  return "";
}
// Activate the corresponding traffic light for 10 seconds based on the direction

void activateEmergencyMode(String direction) {
  Serial.println("Emergency Mode for: " + direction);
  resetTrafficLights();
  if (direction == "North")  digitalWrite(greenNorth, LOW);
  else if (direction == "South") digitalWrite(greenSouth, LOW);
  else if (direction == "East") digitalWrite(greenEast, LOW);
  else if (direction == "West") digitalWrite(greenWest, LOW);
  delay(10000);
  resetTrafficLights();
}
// Activate the corresponding traffic light for 10 seconds based on the direction
void normalTrafficCycle() {
  Serial.println("Normal Traffic Cycle Started");

  resetTrafficLights();
  // North-South green, East-West red
  digitalWrite(yellowNorth, HIGH);
  digitalWrite(yellowWest, HIGH);
  // North-South green, East-West red
  digitalWrite(redNorth, HIGH);
  digitalWrite(greenWest, HIGH);
  digitalWrite(greenNorth, LOW);
  digitalWrite(redSouth, LOW);
  digitalWrite(redEast, LOW);
  digitalWrite(redWest, LOW);
  delay(5000);

  // North-South yellow
  digitalWrite(yellowNorth, LOW);
  digitalWrite(yellowEast, LOW);
  delay(2000);

  // North-South red, East-West green
  digitalWrite(yellowNorth, HIGH);
  digitalWrite(yellowEast, HIGH);
  digitalWrite(redNorth, LOW);
  digitalWrite(greenEast, LOW);
  digitalWrite(greenNorth, HIGH);
  digitalWrite(redEast, HIGH);
  delay(5000);

  digitalWrite(yellowSouth, LOW);
  digitalWrite(yellowEast, LOW);
  delay(2000);

  // North-South red, East-West green
  digitalWrite(yellowSouth, HIGH);
  digitalWrite(yellowEast, HIGH);
  digitalWrite(redSouth, HIGH);
  digitalWrite(greenSouth, LOW);
  digitalWrite(greenEast, HIGH);
  digitalWrite(redEast, LOW);
  delay(5000);

  digitalWrite(yellowSouth, LOW);
  digitalWrite(yellowWest, LOW);
  delay(2000);

  // North-South red, East-West green
  digitalWrite(yellowSouth, HIGH);
  digitalWrite(yellowWest, HIGH);
  digitalWrite(redSouth, LOW);
  digitalWrite(redWest, HIGH);
  digitalWrite(greenSouth, HIGH);
  digitalWrite(greenWest, LOW);
  delay(5000);

  digitalWrite(yellowWest, LOW);
  digitalWrite(yellowNorth, LOW);
  delay(2000);
}
// Reset all traffic lights to red
void resetTrafficLights() {
  digitalWrite(redNorth, HIGH); digitalWrite(greenNorth, HIGH); digitalWrite(yellowNorth, LOW);
  digitalWrite(redSouth, HIGH); digitalWrite(greenSouth, HIGH); digitalWrite(yellowSouth, LOW);
  digitalWrite(redEast, HIGH); digitalWrite(greenEast, HIGH); digitalWrite(yellowEast, LOW);
  digitalWrite(redWest, HIGH); digitalWrite(greenWest, HIGH); digitalWrite(yellowWest, LOW);
}
// Reset all traffic lights to red
void setupLights() {
  pinMode(redNorth, OUTPUT); pinMode(greenNorth, OUTPUT); pinMode(yellowNorth, OUTPUT);
  pinMode(redSouth, OUTPUT); pinMode(greenSouth, OUTPUT); pinMode(yellowSouth, OUTPUT);
  pinMode(redEast, OUTPUT); pinMode(greenEast, OUTPUT); pinMode(yellowEast, OUTPUT);
  pinMode(redWest, OUTPUT); pinMode(greenWest, OUTPUT); pinMode(yellowWest, OUTPUT);
  resetTrafficLights();
}
// MQTT callback function to handle incoming messages
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("MQTT: ");
  for (int i = 0; i < length; i++) Serial.print((char)payload[i]);
  Serial.println();
}
