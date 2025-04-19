#include <WiFi.h>
#include <ESP32Firebase.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#include <ESP32Servo.h>

// WiFi and Firebase credentials
#define WIFI_SSID "Shrinivas 5G"
#define WIFI_PASSWORD "sEENU@1234"
#define API_KEY "AIzaSyDzL_-th-PXinHPlp6PapqB8cq_MTWTS04"
#define DATABASE_URL "https://iot-app-f5138-default-rtdb.asia-southeast1.firebasedatabase.app/"

Firebase firebase(DATABASE_URL);

Servo myServo;

// Pin definitions
#define ONE_WIRE_BUS 14
const int motionSensor = 12;
const int servo = 18;
const int photoresistor = 26;

const int light1_relay = 2;
const int light2_relay = 4;
const int fan_relay = 5;

const int led =23;
const int buzzer =32 ;
const int VIN = 25;

// Firebase and other objects

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Constants and variables
const float VCC = 5.0;
const float QOV = 0.5 * VCC;  // Quiescent Output voltage
float voltage;  // Internal variable for voltage
unsigned long lastTemperatureUpdate = 0;
unsigned long lastCurrentUpdate = 0;
const unsigned long temperatureInterval = 30000;  // Update temperature every 30 seconds
const unsigned long currentInterval = 30000;      // Update current every 30 seconds

void setup() {
  Serial.begin(115200);
  sensors.begin();

  // Set up pins
  pinMode(light1_relay, OUTPUT);
  pinMode(fan_relay, OUTPUT);
  myServo.attach(servo);

  pinMode(led,OUTPUT);
  pinMode(light2_relay,OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(photoresistor, INPUT);
  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
    // Print the IP address
  Serial.print("IP Address: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

  // Initialize Firebase
  //firebase.begin(API_KEY);

  // Attach Firebase data change listeners
  firebase.setString("devices/door/path", "devices/door");
  firebase.setString("devices/light1/path", "devices/light1");
  firebase.setString("devices/fan/path", "devices/fan");
  //firebase.setDataChangedCallback([](FirebaseData fbData) {
    //updateDeviceState(fbData);
  //}, "/devices");
}

void loop() {
  unsigned long currentMillis = millis();

  // Check for motion detection
  int motionDetected = digitalRead(motionSensor);
  if (motionDetected) {
    firebase.setInt("devices/light1", 1);
    firebase.setInt("devices/fan", 1);
  }

  // Update temperature and current values
  if (currentMillis - lastTemperatureUpdate >= temperatureInterval) {
    readTemperature();
    lastTemperatureUpdate = currentMillis;
  }

  if (currentMillis - lastCurrentUpdate >= currentInterval) {
    readCurrent();
    lastCurrentUpdate = currentMillis;
  }
}

void updateDeviceState(FirebaseData fbData) {
  if (fbData.dataType() == "int") {
    if (fbData.dataPath() == "/devices/door") {
      int state = fbData.intData();
      if (state == 1) {
        openDoor();
      } else if (state == 2) {
        closeDoor();
      }
    } else if (fbData.dataPath() == "/devices/light1") {
      int state = fbData.intData();
      digitalWrite(light1_relay, state == 0 ? HIGH : LOW);
    } else if (fbData.dataPath() == "/devices/fan") {
      int state = fbData.intData();
      digitalWrite(fan_relay, state == 0 ? HIGH : LOW);
    }
  }
}

void openDoor() {
  myServo.write(180);
}

void closeDoor() {
  myServo.write(60);
}

void readTemperature() {
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);
  if (temperatureC != DEVICE_DISCONNECTED_C) {
    firebase.pushFloat("sensors/temperature", temperatureC);
    Serial.print("Temperature: ");
    Serial.print(temperatureC);
    Serial.println(" °C");
  } else {
    Serial.println("Error: Could not read temperature data");
  }
}

void readCurrent() {
  float voltage_raw = (5.0 / 1023.0) * analogRead(VIN);  // Read the voltage from sensor
  voltage = voltage_raw - QOV + 0.012;  // Offset to make voltage zero when there is no current
  float current = voltage / 1;

  if (abs(current) > 0.01) {  // Assuming a cutoff limit of 0.01A
    firebase.pushFloat("sensors/current", current);
    Serial.print("V: ");
    Serial.print(voltage, 3);  // Print voltage with 3 decimal places
    Serial.print("V, I: ");
    Serial.print(current, 2);  // Print the current with 2 decimal places
    Serial.println("A");
  } else {
    Serial.println("No Current");
  }
}