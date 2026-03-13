// MobileControl.ino
// Mobile Control for Traffic Light System
// This code connects to a WiFi network and an MQTT broker to receive control messages for traffic lights. It manages the traffic light states based on the received messages and runs a normal traffic cycle when
// no emergency mode is active. When an emergency message is received, it activates the corresponding traffic light for 10 seconds before resetting to normal operation.  

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ThingSpeak.h>
#include <WiFiClient.h>

// WiFi Credentials
const char* ssid = "Redmi Note 10T 5G"; 
const char* password = "password";

// MQTT Credentials
const char* mqttServer = "mqtt3.thingspeak.com";  // Your MQTT Broker
const int mqttPort = 1883;
const char* clientID = "OSgeFRIJJRMIJhopPAADNDc"; // Unique client ID
const char* mqttUsername = "OSgeFRIJJRMIJhopPAADNDc";
const char* mqttPassword = "L1LPuGT/SU0wNnApha0OmWuh";

// MQTT Topics
const char* controlTopic = "traffic/control/direction";  // Topic for receiving control messages

// Pins for Traffic Lights (Active LOW)
const int redNorth = D1, yellowNorth = D2, greenNorth = D3;
const int redEast = D4, yellowEast = D5, greenEast = D6;
const int redSouth = D7, yellowSouth = D5, greenSouth = D9;
const int redWest = D0, yellowWest = D2, greenWest = D8;

WiFiClient espClient;
PubSubClient client(espClient);

bool emergencyMode = false;

void setup() {
  Serial.begin(115200);
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi Connected!");

  // Connect to MQTT Broker
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  
  while (!client.connected()) {
    if (client.connect(clientID, mqttUsername, mqttPassword)) {
      Serial.println("Connected to MQTT Broker");
      client.subscribe(controlTopic);  // Subscribe to the topic for controlling the traffic lights
    } else {
      delay(2000);
      Serial.println("Retrying MQTT connection...");
    }
  }

  setupLights();  // Initialize the lights
}

void loop() {
  client.loop();  // Maintain MQTT connection

  if (!emergencyMode) {
    normalTrafficCycle();  // Run normal traffic light cycle when no emergency mode
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  payload[length] = '\0';  // Null terminate the payload
  String message = String((char*)payload);  // Convert byte array to String

  Serial.print("Received message: ");
  Serial.println(message);  // Print the received message (for debugging)

  // Call function to activate the appropriate traffic light based on the direction
  activateTrafficLight(message);
}

void activateTrafficLight(String direction) {
  emergencyMode = true;  // Set emergency mode to true

  // Reset all traffic lights first to ensure no conflicting states
  resetTrafficLights();

  if (direction == "North") {
    // Activate green light for North and red for others
    digitalWrite(greenNorth, LOW);  // Green for North
    digitalWrite(redEast, LOW);     // Red for East
    digitalWrite(redSouth, LOW);    // Red for South
    digitalWrite(redWest, LOW);     // Red for West

    // Wait for 10 seconds with North green
    delay(10000);

    // Reset the lights after the 10 seconds
    resetTrafficLights();
  }
  else if (direction == "South") {
    // Activate green light for South and red for others
    digitalWrite(greenSouth, LOW);  // Green for South
    digitalWrite(redNorth, LOW);    // Red for North
    digitalWrite(redEast, LOW);     // Red for East
    digitalWrite(redWest, LOW);     // Red for West

    // Wait for 10 seconds with South green
    delay(10000);

    // Reset the lights after the 10 seconds
    resetTrafficLights();
  }
  else if (direction == "East") {
    // Activate green light for East and red for others
    digitalWrite(greenEast, LOW);   // Green for East
    digitalWrite(redNorth, LOW);    // Red for North
    digitalWrite(redSouth, LOW);    // Red for South
    digitalWrite(redWest, LOW);     // Red for West

    // Wait for 10 seconds with East green
    delay(10000);

    // Reset the lights after the 10 seconds
    resetTrafficLights();
  }
  else if (direction == "West") {
    // Activate green light for West and red for others
    digitalWrite(greenWest, LOW);   // Green for West
    digitalWrite(redNorth, LOW);    // Red for North
    digitalWrite(redSouth, LOW);    // Red for South
    digitalWrite(redEast, LOW);     // Red for East

    // Wait for 10 seconds with West green
    delay(10000);

    // Reset the lights after the 10 seconds
    resetTrafficLights();
  }

  emergencyMode = false;  // Reset emergency mode
}

void resetTrafficLights() {
  // Set all lights to red and ensure yellow is off
  digitalWrite(redNorth, HIGH); digitalWrite(greenNorth, HIGH); digitalWrite(yellowNorth, LOW);
  digitalWrite(redSouth, HIGH); digitalWrite(greenSouth, HIGH); digitalWrite(yellowSouth, LOW);
  digitalWrite(redEast, HIGH);  digitalWrite(greenEast, HIGH);  digitalWrite(yellowEast, LOW);
  digitalWrite(redWest, HIGH);  digitalWrite(greenWest, HIGH);  digitalWrite(yellowWest, LOW);
}

void normalTrafficCycle() {
  Serial.println("Normal Traffic Cycle Started");

  resetTrafficLights();
  
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
// Function to initialize the traffic light pins and set them to their default state (red on, green and yellow off)
void setupLights() {
  pinMode(redNorth, OUTPUT); pinMode(greenNorth, OUTPUT); pinMode(yellowNorth, OUTPUT);
  pinMode(redSouth, OUTPUT); pinMode(greenSouth, OUTPUT); pinMode(yellowSouth, OUTPUT);
  pinMode(redEast, OUTPUT); pinMode(greenEast, OUTPUT); pinMode(yellowEast, OUTPUT);
  pinMode(redWest, OUTPUT); pinMode(greenWest, OUTPUT); pinMode(yellowWest, OUTPUT);
  resetTrafficLights();
}
