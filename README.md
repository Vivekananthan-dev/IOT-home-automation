# IOT Home Automation

A smart home automation project that combines ESP32 hardware, Firebase cloud services, a Flutter mobile app, and computer vision to control lighting, fan systems, door access, and sensor monitoring from anywhere.

## Overview

This project demonstrates a complete IoT-based smart home system. The ESP32 reads data from sensors and controls relays and servo-based door access. A Firebase Realtime Database stores device state and sensor values, while a Flutter mobile app provides an easy dashboard for users to control appliances and monitor readings. Additionally, a Python-based YOLOv8 object detection system detects human presence and sends detection data using MQTT.

The repository includes:
- Embedded ESP32 firmware for smart home control
- Flutter mobile app for device and sensor management
- YOLOv8-based human detection
- Firebase integration for real-time data sync
- Hardware design and project documentation

## Features

- Smart control of lights and fan via Firebase
- Motion-based automation using PIR sensors
- Door control using a servo motor
- Temperature monitoring
- Current monitoring
- Smoke alarm via buzzer alert
- Night mode logic using photoresistor
- Flutter app for login, controls, and monitoring
- Human presence detection using YOLOv8
- MQTT communication for detection events

## System Architecture

The system is organized into three core parts:

1. **Embedded control layer**
   - ESP32 firmware in `Project/Project.ino`
   - Reads sensors and controls relays/servo
   - Sends data to Firebase

2. **Mobile application layer**
   - Flutter app inside `mobileapplication/`
   - Allows user sign-in and device control
   - Reads live sensor and device state

3. **Vision detection layer**
   - Python scripts in `Human_detection/`
   - Detects people using YOLOv8
   - Publishes detection status over MQTT

```
Camera / YOLOv8 detection --> MQTT --> ESP32
                                       |
                                       v
ESP32 <--> Firebase Realtime Database <--> Flutter App
                |
                +--> Temperature, current, and device states
```

## Repository Structure

```
IOT-home-automation/
├── Human_detection/
│   ├── controller.py            # YOLOv8 detection and MQTT publishing
│   ├── test.py                 # YOLO detection test
│   ├── person_verify.py        # Human verification logic
│   ├── go.py                   # Helper script
│   ├── yolov8n.pt              # YOLO model weights
│   ├── datasets/               # Detection-related datasets
│   └── runs/                   # Inference output
│
├── Project/
│   └── Project.ino             # Main ESP32 firmware
│
├── Project_v1/
│   └── Project_v1.ino          # Earlier prototype version
│
├── mobileapplication/
│   ├── lib/                    # Flutter source code
│   ├── assets/                 # App icons and images
│   ├── android/                # Android project files
│   ├── ios/                    # iOS project files
│   ├── pubspec.yaml            # Dart dependencies
│   ├── firebase_options.dart   # Firebase config
│   └── README.md               # App-level documentation
│
├── runs/                       # Execution outputs
├── test/                       # Test resources
├── yolo-Weights/               # Additional model weights
├── Design_Home_Automation.ckt  # Circuit design
├── final_circuit.png           # Final circuit diagram
├── FINAL_REPORT_PROJECT.pdf    # Project report
├── FINAL_PRESENTATION.pptx     # Final presentation
├── firebase.txt                # Firebase notes
├── plan1.png / plan2.png       # Concept board diagrams
└── *.pdf / *.docx              # Documentation files
```

## Technology Stack

### Embedded firmware
- **Language**: C++
- **Platform**: ESP32
- **Connectivity**: WiFi
- **Backend**: Firebase Realtime Database
- **Libraries**: Dallas Temperature Sensor, OneWire, ESP32Servo

### Mobile app
- **Framework**: Flutter
- **Language**: Dart
- **Backend**: Firebase Authentication & Realtime Database
- **UI**: Material Design

### Computer vision
- **Language**: Python
- **Libraries**: OpenCV, Ultralytics YOLOv8, MQTT
- **Detection Model**: YOLOv8 nano

## Hardware Components

- ESP32 development board
- PIR motion sensor
- Photoresistor / LDR
- DS18B20 temperature sensor
- Smoke sensor
- Relay modules
- Servo motor (for door control)
- Buzzer
- LED indicators

## Setup Instructions

### 1) ESP32 Firmware Setup

1. Open `Project/Project.ino` in Arduino IDE
2. Install required libraries:
   - WiFi.h
   - ESP32Firebase.h
   - OneWire.h
   - DallasTemperature.h
   - ESP32Servo.h

3. Update the following credentials:
   ```cpp
   const char* ssid = "YOUR_WIFI_SSID";
   const char* password = "YOUR_WIFI_PASSWORD";
   const char* firebaseURL = "YOUR_FIREBASE_URL";
   ```

4. Connect your ESP32 and upload the firmware

**⚠️ Security Note**: The code currently contains hardcoded credentials. Replace with your own values before production use.

### 2) Flutter Mobile App Setup

```bash
cd mobileapplication
flutter pub get
flutter run
```

Ensure Firebase configuration is set up in:
- `lib/main.dart`
- `lib/firebase_options.dart`

The app manages these Firebase paths:
- `devices/light1`
- `devices/light2`
- `devices/fan`
- `devices/door`
- `sensors/temperature`
- `sensors/current`

### 3) Human Detection Module Setup

```bash
cd Human_detection
pip install ultralytics opencv-python paho-mqtt
python controller.py
```

The detection script:
- Captures frames from your webcam
- Runs YOLOv8 object detection
- Publishes detection status to MQTT topic: `check`

## App Usage

1. **Sign In**: Authenticate with Firebase credentials
2. **Dashboard**: View connected devices and sensor readings
3. **Device Control**:
   - Toggle Light 1 & Light 2
   - Toggle Fan
   - Control Door (open/close)
4. **Monitor**: View real-time temperature and current readings

## How It Works

### Automation Logic
- **Motion Detection**: PIR sensor triggers lighting/fan when motion detected
- **Light Sensing**: Photoresistor enables night mode for automatic LED control
- **Smoke Detection**: Buzzer activates on smoke sensor alert
- **Firebase Sync**: Real-time device state synchronization across all platforms
- **Human Detection**: YOLOv8 detects presence and publishes via MQTT

### Data Flow
```
Sensors (PIR, Temp, Current) 
        ↓
    ESP32 
        ↓
Firebase DB ←→ Flutter App
        ↑
   MQTT/YOLOv8 (Human Detection)
```

## Project Status

✅ Functional smart-home prototype combining:
- Embedded systems automation
- Cloud-based control
- Mobile app interface
- Computer vision integration
- MQTT-based messaging

Suitable for:
- IoT learning & education
- Smart-home prototyping
- IoT system demonstration
- Academic projects

## License

No explicit license. Please check with the repository owner before commercial use.

## Documentation

Additional resources:
- `FINAL_REPORT_PROJECT.pdf` - Detailed project report
- `FINAL_PRESENTATION.pptx` - Project presentation
- `Design_Home_Automation.ckt` - Circuit schematic
- `final_circuit.png` - Circuit diagram
- `mobileapplication/README.md` - App-specific documentation

---

**Repository**: https://github.com/Vivekananthan-dev/IOT-home-automation
