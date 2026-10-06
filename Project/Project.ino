#include <WiFi.h>
#include <ESP32Firebase.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#include <ESP32Servo.h>
#define WIFI_SSID ""
#define WIFI_PASSWORD ""
#define API_KEY ""
#define DATABASE_URL ""


#define _SSID ""          // Your WiFi SSID
#define _PASSWORD ""      // Your WiFi Password
#define REFERENCE_URL ""  // Your Firebase project reference url

Firebase firebase(REFERENCE_URL);

Servo myServo;

//Pins
#define ONE_WIRE_BUS 14 
const int motionSensor = 12;
const int smoke =34;//change
const int photoresistor = 26;
const int servo = 18;
const int light1_relay = 2;
const int light2_relay = 4;
const int fan_relay = 5;  //check
const int buzzer =32 ;//change 
const int led =23;
int sensor;
const int thre =300;
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

void setup()
{
  pinMode(led,OUTPUT);
  Serial.begin(115200);
  sensors.begin();
  //setup pinMode
  pinMode(light1_relay, OUTPUT);
  pinMode(light2_relay, OUTPUT);
  pinMode(fan_relay, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(photoresistor, INPUT);//define
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

  //initialize firebase


}

void loop()
{
  readTemperature();
  isNight();

  int motionDetected = digitalRead(motionSensor);

  if (motionDetected){
    //if(camera){

    //Database update light1 an fan

    firebase.setInt("devices/fan", 1);
    firebase.setInt("devices/light1", 1);

    //}
  }
  
  int smokeDetected =  digitalRead(smoke); // add read value

  if(smokeDetected){
    // activate buzzer
    digitalWrite(buzzer,HIGH);
    delay(8000); //check //check
    digitalWrite(buzzer,LOW);
    delay(100);
    //push notification if possible
  }
  

   // check before run
  if (firebase.getInt("devices/door") == 1) {
    openDoor();
  } else if (firebase.getInt("devices/door") == 2) {
    closeDoor();
  }

  if(firebase.getInt("devices/light1") == 1)//Firabase: light1 on
  {
    digitalWrite(light2_relay,LOW);
    
  }else if(firebase.getInt("devices/light2") == 0){
    digitalWrite(light2_relay, HIGH);
    
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
  

  
  
}

void openDoor() {
    myServo.write(180);
}

void closeDoor() {
    myServo.write(60);
}

void isNight(){
  sensor = analogRead(photoresistor);//photo
  Serial.println(sensor);
  if(sensor<thre){
    digitalWrite(led,HIGH);
  }
  else{
    digitalWrite(led,LOW);
  }
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
    firebase.setFloat("sensors/current",Voltage);
  }else{
    Serial.println("No Current");
  }
  delay(500);
}

