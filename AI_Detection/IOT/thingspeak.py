# This module handles communication with ThingSpeak, an IoT analytics platform. It provides functions to send ambulance detection data (direction and count) to ThingSpeak and to reset the fields when needed. The send_to_thingspeak function constructs a payload with the API key and the relevant data fields, then makes a POST request to the ThingSpeak URL. The reset_fields function resets specific fields to zero by sending a similar POST request. Both functions include error handling to catch and print any exceptions that may occur during the HTTP requests.
# The module relies on the requests library to make HTTP requests and imports configuration variables from a config module for the ThingSpeak API key and URL.

import requests
from config import THINGSPEAK_API_KEY, THINGSPEAK_URL # Import ThingSpeak configuration variables from the config module, including the API key and the URL for the ThingSpeak channel.

# This function sends the ambulance's direction and count data to ThingSpeak.
def send_to_thingspeak(direction, count):

    payload = {
        "api_key": THINGSPEAK_API_KEY,
        "field6": direction,
        "field8": count
    }

    try:

        response = requests.post(
            THINGSPEAK_URL,
            data=payload,
            timeout=5
        )

        print("ThingSpeak Update:", response.status_code)

    except Exception as e:
        print("ThingSpeak Error:", e)

# This function resets the fields in ThingSpeak to zero, which can be useful for clearing previous data or starting fresh for a new detection session.
def reset_fields():
# The payload for resetting fields includes the API key and sets specific fields (field1 to field4) to zero.
    payload = {
        "api_key": THINGSPEAK_API_KEY,
        "field1": 0,
        "field2": 0,
        "field3": 0,
        "field4": 0
    }

    try:
        requests.post(THINGSPEAK_URL, data=payload)
        print("ThingSpeak fields reset")

    except Exception as e:
        print("Reset error:", e)