#include <WiFi.h>
#include <FirebaseESP32.h>
#include <Wire.h>
//#include <Adafruit_Sensor.h>
#include <DHT.h>

#define FIREBASE_HOST "https://iot-app-f5138-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "AIzaSyDzL_-th-PXinHPlp6PapqB8cq_MTWTS04"
#define WIFI_SSID "yourwifissid"
#define WIFI_PASSWORD "yourwifipassword"

#define LIGHT1_PIN 5  // Pin connected to relay for light 1
#define FAN_PIN 4     // Pin connected to relay for fan
#define DOOR_PIN 2    // Pin connected to servo for door control

#define DHT_PIN 13    // Pin connected to DHT sensor
#define DHT_TYPE DHT11

#define MOTION_SENSOR_PIN 12  // Pin connected to motion sensor

FirebaseData firebaseData;
DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(115200);
  pinMode(LIGHT1_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(DOOR_PIN, OUTPUT);
  pinMode(MOTION_SENSOR_PIN, INPUT);

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
  // Initialize DHT sensor
  dht.begin();
}

void loop() {
  // Check for motion
  int motionDetected = digitalRead(MOTION_SENSOR_PIN);

  if (motionDetected) {
    // Turn on light and fan
    digitalWrite(LIGHT1_PIN, HIGH);
    digitalWrite(FAN_PIN, HIGH);
    delay(500); // Add delay to prevent flickering

    // Update Firebase to indicate light and fan are on
    Firebase.setInt(firebaseData, "/appliances/light1", 1);
    Firebase.setInt(firebaseData, "/appliances/fan", 1);
  }

  // Check Firebase for commands
  if (Firebase.getInt(firebaseData, "/appliances/light1") == 1) {
    digitalWrite(LIGHT1_PIN, HIGH);
  } else {
    digitalWrite(LIGHT1_PIN, LOW);
  }

  if (Firebase.getInt(firebaseData, "/appliances/fan") == 1) {
    digitalWrite(FAN_PIN, HIGH);
  } else {
    digitalWrite(FAN_PIN, LOW);
  }

  // Read temperature from DHT sensor
  float temperature = dht.readTemperature();

  // Send temperature data to Firebase
  Firebase.setFloat(firebaseData, "/sensors/temperature", temperature);

  // Check for door control command
  int doorCommand = Firebase.getInt(firebaseData, "/door/control");
  if (doorCommand == 1) {
    openDoor();
  } else if (doorCommand == 0) {
    closeDoor();
  }

  delay(1000); // Adjust delay according to your requirements
}

void openDoor() {
  // Code to open the door using a servo
}

void closeDoor() {
  // Code to close the door using a servo
}
