#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ThingSpeak.h>

/* WIFI CONFIG */
const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASSWORD";

/* MQTT CONFIG */
const char* mqttServer = "mqtt3.thingspeak.com";
const int mqttPort = 1883;
const char* mqttUser = "MQTT_USERNAME";
const char* mqttPass = "MQTT_PASSWORD";

const char* nextSignalTopic = "traffic/next_signal";

/* THINGSPEAK CONFIG */
unsigned long channelID = 2906713;
const char* readAPIKey = "READ_API_KEY";

/* TRAFFIC PINS */
int redNorth = D1;
int greenNorth = D2;

int redSouth = D3;
int greenSouth = D4;

int redEast = D5;
int greenEast = D6;

int redWest = D7;
int greenWest = D8;

WiFiClient espClient;
PubSubClient client(espClient);


/* CONNECT WIFI */

void connectWiFi() {

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting WiFi...");
  }

  Serial.println("WiFi Connected");
}


/* CONNECT MQTT */

void connectMQTT() {

  client.setServer(mqttServer, mqttPort);

  while (!client.connected()) {

    Serial.println("Connecting MQTT...");

    if (client.connect("NodeMCU_Controller", mqttUser, mqttPass)) {

      Serial.println("MQTT Connected");

    } else {

      delay(2000);

    }
  }
}


/* RESET TRAFFIC */

void resetLights() {

  digitalWrite(redNorth, HIGH);
  digitalWrite(redSouth, HIGH);
  digitalWrite(redEast, HIGH);
  digitalWrite(redWest, HIGH);

  digitalWrite(greenNorth, HIGH);
  digitalWrite(greenSouth, HIGH);
  digitalWrite(greenEast, HIGH);
  digitalWrite(greenWest, HIGH);
}


/* EMERGENCY MODE */

void activateEmergency(String direction) {

  resetLights();

  if (direction == "North") digitalWrite(greenNorth, LOW);
  if (direction == "South") digitalWrite(greenSouth, LOW);
  if (direction == "East") digitalWrite(greenEast, LOW);
  if (direction == "West") digitalWrite(greenWest, LOW);

  Serial.println("Emergency Direction: " + direction);

  client.publish(nextSignalTopic, direction.c_str());

  delay(10000);

  resetLights();
}


/* READ THINGSPEAK */

String checkDirection() {

  int north = ThingSpeak.readIntField(channelID, 1, readAPIKey);
  int south = ThingSpeak.readIntField(channelID, 2, readAPIKey);
  int east  = ThingSpeak.readIntField(channelID, 3, readAPIKey);
  int west  = ThingSpeak.readIntField(channelID, 4, readAPIKey);

  if (north == 1) return "North";
  if (south == 1) return "South";
  if (east == 1) return "East";
  if (west == 1) return "West";

  return "";
}


/* SETUP */

void setup() {

  Serial.begin(115200);

  pinMode(redNorth, OUTPUT);
  pinMode(greenNorth, OUTPUT);

  pinMode(redSouth, OUTPUT);
  pinMode(greenSouth, OUTPUT);

  pinMode(redEast, OUTPUT);
  pinMode(greenEast, OUTPUT);

  pinMode(redWest, OUTPUT);
  pinMode(greenWest, OUTPUT);

  connectWiFi();

  ThingSpeak.begin(espClient);

  connectMQTT();

  resetLights();
}


/* LOOP */

void loop() {

  client.loop();

  String direction = checkDirection();

  if (direction != "") {

    activateEmergency(direction);

  }
}