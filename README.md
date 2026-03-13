<p align="center">
<img src="https://readme-typing-svg.herokuapp.com?color=00FFAA&size=30&center=true&vCenter=true&width=800&lines=AI+Powered+Ambulance+Detection;Smart+Traffic+Signal+Control;Computer+Vision+%2B+IoT+Project;Built+by+Pramod+M" />
</p>
# 🚑 Smart Ambulance Detection & Traffic Signal Control System

![GitHub stars](https://img.shields.io/github/stars/Pramodbm18/Ambulance-Detection-Using-Image-Processing?style=social)
![GitHub forks](https://img.shields.io/github/forks/Pramodbm18/Ambulance-Detection-Using-Image-Processing?style=social)
![Python](https://img.shields.io/badge/Python-3.10-blue)
![OpenCV](https://img.shields.io/badge/OpenCV-ComputerVision-green)
![YOLOv5](https://img.shields.io/badge/YOLOv5-ObjectDetection-red)
![NodeMCU](https://img.shields.io/badge/ESP8266-IoT-orange)
![License](https://img.shields.io/badge/License-MIT-yellow)

---

## 🌍 Project Overview

Traffic congestion can delay emergency vehicles like **ambulances**, risking lives.

This project introduces an **AI-powered smart traffic management system** that detects ambulances in real-time using **computer vision** and automatically changes traffic signals to give them priority.

The system integrates **Artificial Intelligence + IoT + Smart Traffic Control** to reduce ambulance response time.

---

## 🚀 Features

✔ Real-time ambulance detection using **YOLOv5**
✔ Live camera video processing
✔ Automatic traffic signal control
✔ IoT communication using **NodeMCU (ESP8266)**
✔ Smart city traffic management concept
✔ Expandable for multi-intersection systems

---

## 🧠 Technologies Used

| Technology      | Purpose                    |
| --------------- | -------------------------- |
| Python          | Image processing           |
| OpenCV          | Video frame processing     |
| YOLOv5          | Ambulance object detection |
| NodeMCU ESP8266 | IoT communication          |
| MQTT / HTTP     | Data transmission          |
| Relay Module    | Traffic signal control     |

---

## 🏗 System Architecture

```
Camera
   │
   ▼
Python + OpenCV
   │
   ▼
YOLOv5 Ambulance Detection
   │
   ▼
Detection Result
   │
   ▼
MQTT / WiFi Communication
   │
   ▼
NodeMCU ESP8266
   │
   ▼
Relay Module
   │
   ▼
Traffic Lights Control
```

---

## ⚙️ Installation

### 1️⃣ Clone Repository

```
git clone https://github.com/Pramodbm18/Ambulance-Detection-Using-Image-Processing.git
cd Ambulance-Detection-Using-Image-Processing
```

### 2️⃣ Install Dependencies

```
pip install -r requirements.txt
```

### 3️⃣ Run Detection

```
python detect.py
```

---

## 📸 Demo

Ambulance detected in traffic video:

```
[ Camera Feed ]
        ↓
[ YOLO Detection ]
        ↓
[ Ambulance Identified ]
        ↓
[ Traffic Signal Turns Green ]
```

---

## 📂 Project Structure

```
Ambulance-Detection-Using-Image-Processing
│
├── dataset
├── models
├── detection.py
├── requirements.txt
├── README.md
```

---

## System Architecture Diagram (Professional)

                ┌──────────────┐
                │   Camera     │
                └──────┬───────┘
                       │
                       ▼
              ┌────────────────┐
              │ Python + OpenCV│
              └──────┬─────────┘
                     │
                     ▼
              ┌────────────────┐
              │  YOLOv5 Model  │
              │ Ambulance Det. │
              └──────┬─────────┘
                     │
                     ▼
              ┌────────────────┐
              │ MQTT / WiFi    │
              │ Communication  │
              └──────┬─────────┘
                     │
                     ▼
              ┌────────────────┐
              │ NodeMCU ESP8266│
              └──────┬─────────┘
                     │
                     ▼
              ┌────────────────┐
              │ Relay Module   │
              └──────┬─────────┘
                     │
                     ▼
              ┌────────────────┐
              │ Traffic Lights │
              └────────────────┘

⚙️ Setup Guide

1️⃣ Install Arduino IDE

Download and install Arduino IDE from the official website.

Download: https://www.arduino.cc/en/software

Steps

1) Install Arduino IDE
2) Open Arduino IDE
3) Go to File → Preferences
4) Add the following URL in Additional Boards Manager URLs
  http://arduino.esp8266.com/stable/package_esp8266com_index.json
5) Click OK

2️⃣ Install ESP8266 Board (NodeMCU)

1) Go to Tools → Board → Boards Manager
2) Search for:
  esp8266
3) Install
  esp8266 by ESP8266 Community

3️⃣ Select NodeMCU Board

Go to:
Tools → Board → NodeMCU 1.0 (ESP-12E Module)
Set the following:
| Setting      | Value       |
| ------------ | ----------- |
| Board        | NodeMCU 1.0 |
| Upload Speed | 115200      |
| Port         | COM Port    |

☁️ ThingSpeak Setup
1️⃣ Create ThingSpeak Account

Go to:
https://thingspeak.com
Sign up and log in.

2️⃣ Create New Channel

1) Click New Channel
2) Enter channel details
  Example:
| Field   | Value               |
| ------- | ------------------- |
| Field 1 | Ambulance Detection |
| Field 2 | Traffic Signal      |

🔌 Hardware Required

| Component          | Quantity |
| ------------------ | -------- |
| NodeMCU ESP8266    | 1        |
| Relay Module       | 1        |
| LED Traffic Lights | 3        |
| Resistors          | 3        |
| Breadboard         | 1        |
| Jumper Wires       | Several  |

📡 Data Flow
-------------
Camera
   ↓
YOLO Detection (Python)
   ↓
Send Data → ThingSpeak
   ↓
NodeMCU Reads Data
   ↓
Traffic Signal Changes

----------

## 🌟 Future Improvements

• Multi-camera traffic monitoring
• Integration with **Smart City infrastructure**
• Cloud-based ambulance tracking
• Edge AI deployment using **NVIDIA Jetson**

---

## 👨‍💻 Author

**Pramod B M**

AI + IoT Developer
Smart Traffic & Computer Vision Projects

GitHub
https://github.com/Pramodbm18

---

⭐ If you like this project, give it a **star**!
