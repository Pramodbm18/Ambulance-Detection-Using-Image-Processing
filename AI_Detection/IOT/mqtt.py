# This module handles MQTT communication to notify traffic signals about the ambulance's approach.
# It uses the paho-mqtt library to publish messages to a specified MQTT topic, which can be subscribed to by traffic signal controllers.
# The notify_next_signal function takes the direction of the ambulance as an argument and sends a message to the MQTT broker, indicating the ambulance's approach from that direction.

import paho.mqtt.publish as publish # Import the publish function from the paho-mqtt library to send MQTT messages.
from config import MQTT_BROKER, MQTT_PORT, MQTT_USERNAME, MQTT_PASSWORD, MQTT_TOPIC # Import MQTT configuration variables from the config module.

# This function sends an MQTT message to notify the next traffic signal about the ambulance's approach.
def notify_next_signal(direction):

# The function takes a single argument 'direction', which indicates the direction from which the ambulance is approaching (e.g., "NORTH", "SOUTH", "EAST", "WEST").
    try:

        publish.single(
            MQTT_TOPIC,
            f"AMBULANCE_{direction}",
            hostname=MQTT_BROKER,
            port=MQTT_PORT,
            auth={
                "username": MQTT_USERNAME,
                "password": MQTT_PASSWORD
            }
        )

        print("MQTT sent:", direction)

    except Exception as e: # If there is an error while sending the MQTT message, it will be caught here and printed to the console.
        print("MQTT error:", e)