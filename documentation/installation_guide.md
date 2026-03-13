# Installation Guide

This guide explains how to set up the Ambulance Detection and Traffic Control System.

## 1. Install Python

Download Python from:

https://www.python.org/downloads/

Recommended version:

Python 3.9+

---

## 2. Clone the Repository

git clone https://github.com/Pramodbm18/Ambulance-Detection-Using-Image-Processing.git

cd ambulance-detection

---

## 3. Install Python Dependencies

pip install -r ai_detection/requirements.txt

---

## 4. Install Arduino IDE

Download from:

https://www.arduino.cc/en/software

---

## 5. Install ESP8266 Board

In Arduino IDE:

File → Preferences

Add:

http://arduino.esp8266.com/stable/package_esp8266com_index.json

Then go to:

Tools → Board → Boards Manager

Search:

esp8266

Install **ESP8266 by ESP8266 Community**

---

## 6. Upload NodeMCU Code

Open:

nodemcu_controller/MobileControl.ino

Upload to NodeMCU.

Repeat for:

nodemcu_next_signal/NextSignalController.ino

---

## 7. Configure ThingSpeak

1. Create a ThingSpeak account
2. Create a new channel
3. Add fields:

Field1 → North  
Field2 → South  
Field3 → East  
Field4 → West  

Copy the API keys.

---

## 8. Run AI Detection

python ai_detection/detect.py