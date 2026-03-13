# System Design

This project implements an AI-powered smart traffic control system for emergency vehicles.

## Objective

Detect ambulances using computer vision and automatically clear traffic signals.

## System Components

1. Camera
2. YOLO Detection System
3. ThingSpeak Cloud
4. NodeMCU Traffic Controller
5. MQTT Communication
6. Traffic Light System

## System Architecture

Camera
↓
YOLO Detection (Python)
↓
ThingSpeak Cloud
↓
NodeMCU Controller
↓
Traffic Light Control
↓
MQTT Message
↓
Next Intersection Controller

## Technologies Used

| Technology | Purpose |
|-----------|---------|
| Python | AI detection |
| YOLOv5 | Object detection |
| OpenCV | Image processing |
| NodeMCU | IoT controller |
| MQTT | Communication |
| ThingSpeak | Cloud data exchange |

## Advantages

- Reduces ambulance delay
- Improves traffic efficiency
- Enables smart city infrastructure
- Scalable to multiple intersections