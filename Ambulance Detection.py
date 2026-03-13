# Ambulance Detection and Traffic Light Control System
# This Python script uses a YOLOv5 custom model to detect ambulances in real-time video feed. It determines the direction of the ambulance and sends updates to ThingSpeak and MQTT for traffic light control. The system also resets fields after a period of inactivity.  

import torch # PyTorch for loading the YOLOv5 model
import cv2 # OpenCV for video processing
import time # Time module for managing intervals
import requests # Requests for HTTP communication with ThingSpeak
import paho.mqtt.publish as publish # MQTT publish for sending messages to the broker

# ==== IP Camera or Webcam ====
IP_CAMERA_URL = 'http://192.0.0.4:8080/video' # Uncomment the line below to use an IP camera instead of a webcam
# cap = cv2.VideoCapture(IP_CAMERA_URL)  # Use IP camera
cap = cv2.VideoCapture(0)  # Use 0 for webcam

# ==== Load YOLOv5 Custom Model ====
model = torch.hub.load("ultralytics/yolov5", "custom", path="C:/Users/Pramod B M/Downloads/yolov5win11customobj-main (2)/yolov5win11customobj-main/yolov5win11customobj-main/best.pt") # Load the custom YOLOv5 model (replace with your model path)   
model.conf = 0.5

# ==== ThingSpeak & MQTT Settings ====
thingspeak_api_key = "EUNG2OJAFA7SYCUF" # Replace with your ThingSpeak API key
thingspeak_url = "https://api.thingspeak.com/update" # ThingSpeak API endpoint for updates
mqtt_broker = "mqtt3.thingspeak.com" # MQTT broker address (ThingSpeak MQTT broker)
mqtt_port = 1883 # MQTT broker port
mqtt_username = "OSgeFRIJJRMIJhopPAADNDc" # MQTT username (ThingSpeak MQTT username)
mqtt_password = "L1LPuGT/SU0wNnApha0OmWuh" # MQTT password (ThingSpeak MQTT password)
mqtt_topic_base = "channels/2906713/subscribe/fields/field7" # Base MQTT topic for subscribing to updates

# ==== Timing & State ====
last_sent_time = {} # Dictionary to track last sent time for each direction
thingspeak_interval = 30  # Minimum interval (in seconds) between updates for the same direction
active_directions = set() # Set to track currently active directions

# ==== Field & Direction Mappings ====
direction_field_map = {
    "North": "field1", # Map each direction to its corresponding ThingSpeak field
    "South": "field2", # Map each direction to its corresponding ThingSpeak field
    "East": "field3",  # Map each direction to its corresponding ThingSpeak field
    "West": "field4"   # Map each direction to its corresponding ThingSpeak field
}
ambulance_count_field = "field8"
# Codes for directions and next signal updates
DIRECTION_CODES = {
    "North": 1, # Code for North direction
    "South": 2, # Code for South direction
    "East": 3,  # Code for East direction
    "West": 4,  # Code for West direction
    "Center": 5 # Code for Center direction
}
NEXT_SIGNAL_CODES = {
    "North": 6,      # North-East
    "South": 7,      # South-West
    "East": 8,       # East-South
    "West": 9,       # West-North
    "Center": 10     # Central-Exit
}

# ==== Get Direction Based on Position ====
def get_direction(x_center, y_center, frame_width, frame_height):
    if y_center < frame_height / 3:
        return "North"
    elif y_center > 2 * frame_height / 3:
        return "South"
    elif x_center < frame_width / 3:
        return "West"
    elif x_center > 2 * frame_width / 3:
        return "East"
    else:
        return "Center"

# ==== Send to ThingSpeak ====
def send_to_thingspeak(direction, count):
    payload = {
        "api_key": thingspeak_api_key,
        "field6": DIRECTION_CODES.get(direction, 0),
        ambulance_count_field: count
    }
    for field in direction_field_map.values():
        payload[field] = 0
    if direction in direction_field_map:
        payload[direction_field_map[direction]] = 1

    try:
        response = requests.post(thingspeak_url, data=payload)
        print(f"ThingSpeak | Direction={direction}, Payload={payload}, Success={response.status_code == 200}")
    except Exception as e:
        print(f"ThingSpeak Error: {e}")

# ==== Notify Next Signal ====
def notify_next_signal(direction):
    next_code = NEXT_SIGNAL_CODES.get(direction, 0)
    payload = {
        "api_key": thingspeak_api_key,
        "field7": next_code
    }
    try:
        response = requests.post(thingspeak_url, data=payload)
        print(f"Next Signal Update | Code={next_code} | Status={response.status_code}")
    except Exception as e:
        print(f"Next Signal Error: {e}")

    topic = mqtt_topic_base + direction.lower()
    try:
        publish.single(
            topic, "AMBULANCE_PASSING_TO_NEXT_SIGNAL",
            hostname=mqtt_broker,
            port=mqtt_port,
            auth={"username": mqtt_username, "password": mqtt_password}
        )
    except Exception as e:
        print(f"MQTT Error: {e}")

# ==== Reset Fields ====
def reset_fields():
    payload = {
        "api_key": thingspeak_api_key,
        "field1": 0,
        "field2": 0,
        "field3": 0,
        "field4": 0
    }
    try:
        response = requests.post(thingspeak_url, data=payload)
        print("ThingSpeak fields reset after 30 sec inactivity")
    except Exception as e:
        print(f"Reset Error: {e}")

# ==== Main Loop ====
while True:
    ret, frame = cap.read()
    if not ret:
        break

    height, width, _ = frame.shape
    results = model(frame)
    detections = results.xyxy[0]
    ambulance_boxes = [d for d in detections if int(d[-1]) == 0]
    ambulance_count = len(ambulance_boxes)
    active_directions.clear()

    for *box, conf, cls in ambulance_boxes:
        x1, y1, x2, y2 = map(int, box[:4])
        x_center = (x1 + x2) / 2
        y_center = (y1 + y2) / 2
        direction = get_direction(x_center, y_center, width, height)
        active_directions.add(direction)

        current_time = time.time()
        if direction not in last_sent_time or (current_time - last_sent_time[direction] > thingspeak_interval):
            send_to_thingspeak(direction, ambulance_count)
            notify_next_signal(direction)
            last_sent_time[direction] = current_time

        # Draw Box
        cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
        cv2.putText(frame, f"Ambulance - {direction}", (x1, y1 - 10),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 255), 2)

    # ==== Reset Inactive ====
    now = time.time()
    inactive_directions = [d for d in last_sent_time if d not in active_directions and now - last_sent_time[d] > thingspeak_interval]

    if inactive_directions:
        reset_fields()
        for d in inactive_directions:
            del last_sent_time[d]

    # ==== Show Frame ====
    cv2.imshow("Ambulance Detection", frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# ==== Cleanup ====
cap.release()
cv2.destroyAllWindows()
