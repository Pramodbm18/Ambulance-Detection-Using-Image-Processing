import cv2
import time
import torch

from config import MODEL_PATH, CAMERA_INDEX, UPDATE_INTERVAL
from utils.direction import get_direction
from iot.thingspeak import send_to_thingspeak, reset_fields
from iot.mqtt import notify_next_signal


print("Loading YOLO model...")

model = torch.hub.load(
    "ultralytics/yolov5",
    "custom",
    path=MODEL_PATH
)

model.conf = 0.5


cap = cv2.VideoCapture(CAMERA_INDEX)

if not cap.isOpened():
    raise Exception("Camera not detected")


last_sent_time = {}
active_directions = set()


while True:

    ret, frame = cap.read()

    if not ret:
        break

    frame = cv2.resize(frame, (640, 480))

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

        now = time.time()

        if direction not in last_sent_time or now - last_sent_time[direction] > UPDATE_INTERVAL:

            send_to_thingspeak(direction, ambulance_count)

            notify_next_signal(direction)

            last_sent_time[direction] = now

        cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)

        cv2.putText(
            frame,
            f"Ambulance - {direction}",
            (x1, y1 - 10),
            cv2.FONT_HERSHEY_SIMPLEX,
            0.6,
            (0, 255, 255),
            2
        )


    now = time.time()

    inactive = [
        d for d in last_sent_time
        if d not in active_directions and now - last_sent_time[d] > UPDATE_INTERVAL
    ]

    if inactive:

        reset_fields()

        for d in inactive:
            del last_sent_time[d]


    cv2.imshow("Ambulance Detection", frame)

    if cv2.waitKey(1) & 0xFF == ord("q"):
        break


cap.release()
cv2.destroyAllWindows()