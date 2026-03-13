#include <ESP8266WiFi.h>
#include <PubSubClient.h>

/* WIFI */
const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASSWORD";

/* MQTT */
const char* mqttServer = "mqtt3.thingspeak.com";
const int mqttPort = 1883;
const char* mqttUser = "MQTT_USERNAME";
const char* mqttPass = "MQTT_PASSWORD";

const char* topic = "traffic/next_signal";

/* TRAFFIC LIGHTS */

int red = D1;
int green = D2;

WiFiClient espClient;
PubSubClient client(espClient);


/* MQTT CALLBACK */

void callback(char* topic, byte* payload, unsigned int length) {

  String message = "";

  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.println("Incoming Direction: " + message);

  digitalWrite(red, HIGH);
  digitalWrite(green, LOW);

  delay(10000);

  digitalWrite(red, LOW);
  digitalWrite(green, HIGH);
}


/* WIFI */

void connectWiFi() {

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
}


/* MQTT */

void connectMQTT() {

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected()) {

    if (client.connect("NodeMCU_2", mqttUser, mqttPass)) {

      client.subscribe(topic);

    } else {

      delay(2000);

    }
  }
}


/* SETUP */

void setup() {

  Serial.begin(115200);

  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);

  connectWiFi();

  connectMQTT();
}


/* LOOP */

void loop() {

  if (!client.connected()) {
    connectMQTT();
  }

  client.loop();
}