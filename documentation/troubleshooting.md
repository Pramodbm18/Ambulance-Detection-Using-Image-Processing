# Troubleshooting Guide

Common issues and solutions.

## Camera Not Detected

Error:

Camera could not be opened

Solution:

Check camera index.

cap = cv2.VideoCapture(0)

Try:

cap = cv2.VideoCapture(1)

---

## YOLO Model Not Loading

Error:

FileNotFoundError: best.pt

Solution:

Ensure the model exists in:

models/best.pt

---

## ThingSpeak Not Updating

Check:

- Internet connection
- Correct API key
- Channel field numbers

---

## NodeMCU Not Connecting to WiFi

Check:

- Correct SSID
- Correct password
- Router distance

---

## MQTT Not Working

Verify:

- MQTT server address
- MQTT port
- username and password

Example:

mqtt3.thingspeak.com
Port: 1883

---

## Low Detection Accuracy

Improve by:

- Training with more images
- Using higher resolution
- Training longer epochs