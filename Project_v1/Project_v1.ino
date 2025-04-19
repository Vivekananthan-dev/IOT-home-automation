#include <WiFi.h>
#include <ESP32Firebase.h>
#include <PubSubClient.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#include <ESP32Servo.h>

#define _SSID "Poda eruma"          // Your WiFi SSID
#define _PASSWORD "VSweta0078" // Your WiFi Password
#define mqtt_server "91.121.93.94"
#define REFERENCE_URL "https://iot-app-f5138-default-rtdb.asia-southeast1.firebasedatabase.app/"  // Your Firebase project reference url
//#define API_KEY "AIzaSyDzL_-th-PXinHPlp6PapqB8cq_MTWTS04"

Firebase firebase(REFERENCE_URL);

#define topic "check"

WiFiClient espClient;
PubSubClient client(espClient);

Servo myServo;
#define ONE_WIRE_BUS 14
const int motion = 33;
const int light1_relay = 15;
const int light2_relay = 21;
const int fan_relay = 5;
const int servo = 18;
#define VIN 25 
const float VCC   = 5.0;
const int model = 1;   

float cutOffLimit = 1.01;

float sensitivity[] ={
          0.185,
          0.100,
          0.066
     
         }; 
const float QOV =   0.5 * VCC;// set quiescent Output voltage of 0.5V
float voltage;// internal variable for voltage

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);         

void setup() {
  Serial.begin(115200);
  pinMode(light1_relay, OUTPUT);
  pinMode(light2_relay, OUTPUT);
  pinMode(fan_relay, OUTPUT);
  pinMode(motion, INPUT);

  myServo.attach(servo);

    //connect wifi
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(1000);

  
  // Connect to WiFi
  Serial.println();
  Serial.println();
  Serial.print("Connecting to: ");
  Serial.println(_SSID);
  WiFi.begin(_SSID, _PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("-");
  }

  Serial.println("");
  Serial.println("WiFi Connected");

  // Print the IP address
  Serial.print("IP Address: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

  client.setServer(mqtt_server,1883);

  digitalWrite(light1_relay, HIGH);
  digitalWrite(light2_relay, HIGH);
  digitalWrite(fan_relay, HIGH);
}

void loop() {

  if (firebase.getInt("devices/door") == 1) {
    Serial.print(firebase.getInt("devices/door"));
    openDoor();
  } else if (firebase.getInt("devices/door") == 2) {
    closeDoor();
  }

  if(firebase.getInt("devices/light1") == 1)//Firabase: light1 on
  {
    digitalWrite(light1_relay,LOW);
    
  }else if(firebase.getInt("devices/light1") == 0){
    digitalWrite(light1_relay, HIGH);
    
  }
 
  if(firebase.getInt("devices/light2") == 1)//Firabase: light2 on
  {
    digitalWrite(light2_relay,LOW);
    
  }else if(firebase.getInt("devices/light2") == 0){
    digitalWrite(light2_relay, HIGH);
    
  }
 
  if(firebase.getInt("devices/fan") == 1 )//Firabase: fan on
  {
    digitalWrite(fan_relay,LOW);
    
  }else if(firebase.getInt("devices/fan") == 0){
    digitalWrite(fan_relay,HIGH);
    
  }
  int motionState = digitalRead(motion);
  if(motionState == HIGH){
    firebase.setInt("devices/light1",1);
    firebase.setInt("devices/fan",1);
    digitalWrite(light1_relay, LOW);
    digitalWrite(fan_relay, LOW);
  }

  readTemperature();
  readcurrent();
  
} 

void openDoor() {
    myServo.write(180);
}

void closeDoor() {
    myServo.write(60);
}

void readTemperature(){
    sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);
  if (temperatureC != DEVICE_DISCONNECTED_C) {
    Serial.print("Temperature: ");
    Serial.print(temperatureC);
    Serial.println(" °C");

    firebase.setFloat("sensors/temperature",temperatureC);
  } else {
    Serial.println("Error: Could not read temperature data");
    Serial.print(temperatureC);
  }
}

void readcurrent(){
    float voltage_raw =   (5.0 / 1023.0)* analogRead(VIN);// Read the voltage from sensor
  voltage =  voltage_raw - QOV + 0.012 ;// 0.000 is a value to make voltage zero when there is no current
  float current = voltage / 1;
 
  if(abs(current) > cutOffLimit ){
    Serial.print("V: ");
    Serial.print(voltage,3);// print voltage with 3 decimal places
    Serial.print("V, I: ");
    Serial.print(current,2); // print the current with 2 decimal places
    Serial.println("A");
    firebase.setFloat("sensors/current",voltage);
  }else{
    Serial.println("No Current");
    firebase.setFloat("sensors/current",0.00);
  }
  delay(500);
}
