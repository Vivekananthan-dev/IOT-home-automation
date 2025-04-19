import cv2
from ultralytics import YOLO
import paho.mqtt.publish as mqtt

# Load YOLOv8 model
model = YOLO("yolov8n.pt")

# Capture video stream
cap = cv2.VideoCapture(0)  # 0 for default camera, or provide a video file path

while True:
    ret, frame = cap.read()
    if not ret:
        break

    # Perform object detection
    results = model(frame)
    person_detected = False

    for result in results:
        boxes = result.boxes  # Boxes object for this result
        for box in boxes:
            if box.cls == 0:  # Class 0 is 'person' in YOLOv8 COCO dataset
                person_detected = True
                break

    # Render results on the frame
    for result in results:
        #print("hello")
        frame = result

    # Publish detection result to MQTT broker
    if person_detected:
        mqtt.single("check", str("0"), hostname="91.121.93.94")
    else:
        mqtt.single("check", str("1"), hostname="91.121.93.94")

    cv2.imshow('YOLOv8 Person Detection', frame)
    if cv2.waitKey(1) == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()