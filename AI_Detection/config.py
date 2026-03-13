# This configuration file defines constants and settings for the AI Detection project, including model paths, camera settings, ThingSpeak API details, MQTT broker information, and system parameters such as update intervals and confidence thresholds.
# It centralizes all the configurable parameters in one place, making it easier to manage and modify the settings as needed for the project.
import os

# Model
MODEL_PATH = "models/best.pt" # Path to the trained model file used for object detection.

# Camera
CAMERA_INDEX = 0

# ThingSpeak
THINGSPEAK_API_KEY = os.getenv("THINGSPEAK_API_KEY") # API key for authenticating with the ThingSpeak service, retrieved from environment variables for security.
THINGSPEAK_URL = "https://api.thingspeak.com/update" # URL endpoint for sending data to ThingSpeak, where the detected information will be posted.

# MQTT
MQTT_BROKER = "mqtt3.thingspeak.com" # Address of the MQTT broker used for publishing messages, in this case, the Thingspeak MQTT broker.
MQTT_PORT = 1883
MQTT_USERNAME = os.getenv("MQTT_USERNAME")
MQTT_PASSWORD = os.getenv("MQTT_PASSWORD")

MQTT_TOPIC = "channels/2906713/subscribe/fields/field7"

# System
UPDATE_INTERVAL = 30
CONFIDENCE_THRESHOLD = 0.5