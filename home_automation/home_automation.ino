#include "OneWire.h"
#include "DallasTemperature.h"
#include "WiFiS3.h"
#include "arduino_secrets.h" 
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <TimeLib.h>

// WiFi credentials
char ssid[] = SECRET_SSID;        
char pass[] = SECRET_PASS;    

// LED and button pins
const char KITCHEN_LED_PIN = 3;
const char FIRE_LED_PIN = 6;
#define FLAME 4 
#define ALARM 5
bool fireStatus1 = false;

const char BEDROOM_LED_PIN = 7;
const char OFFICE_LED_PIN = 2;
const char LIVINGROOM_LED_PIN = 10;
bool kitchenStatus = false;
bool bedroomStatus = false;
bool officeStatus = false;
bool livingroomStatus = false;

// Heater and temperature sensor pins
const int oneWireBus = 11;
const int heaterPin = 13;

// External lighting
int photoPin = A0;
int outsideledPin = 12;
int val = 0;
int light = 0;

int czujnik = 8;    
int swiatloczujnikruchu = 9;  
String readString;
int status = WL_IDLE_STATUS;

// Temperature and heater control
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);
float setTemperature = 25.0;
bool heaterState = false;

// Display
LiquidCrystal_I2C lcd(0x27, 16, 2);  

WiFiServer server(80);

unsigned long lastMotionTime = 0;
const unsigned long motionInterval = 5000;
unsigned long currentTime = 0;
unsigned long lastFireCheckTime = 0;
const unsigned long fireCheckInterval = 100;
unsigned long lastClientCheckTime = 0;
const unsigned long clientCheckInterval = 100; 

// Display refresh timer
unsigned long lastDisplayUpdateTime = 0;
const unsigned long displayUpdateInterval = 30000; 

void setup() {
  Serial.begin(9600);
  pinMode(KITCHEN_LED_PIN, OUTPUT);
  pinMode(BEDROOM_LED_PIN, OUTPUT);
  pinMode(OFFICE_LED_PIN, OUTPUT);
  pinMode(LIVINGROOM_LED_PIN, OUTPUT);
  pinMode(FLAME, INPUT);
  pinMode(ALARM, OUTPUT);
  pinMode(FIRE_LED_PIN, OUTPUT);
  pinMode(outsideledPin, OUTPUT);
  pinMode(czujnik, INPUT);   
  pinMode(swiatloczujnikruchu, OUTPUT); 
  pinMode(heaterPin, OUTPUT);
  digitalWrite(heaterPin, LOW); 

  sensors.begin();
  lcd.init();
  lcd.backlight();

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }
  server.begin();
  printWifiStatus();
}

void Fire() {
  int fire = digitalRead(FLAME);
  if (fire == HIGH) {
    digitalWrite(ALARM, HIGH);
    digitalWrite(FIRE_LED_PIN, HIGH);
    Serial.println("Fire! Fire!");
    lcd.setCursor(0, 1);
    lcd.print("Fire detected! ");
    fireStatus1 = true;
  } else {
    digitalWrite(ALARM, LOW);
    digitalWrite(FIRE_LED_PIN, LOW);
    fireStatus1 = false;
    if (millis() - lastMotionTime > motionInterval) {
      lcd.setCursor(0, 1);
      lcd.print("                "); 
    }
  }
}

void Czujnikruchu() {
  int ruch = digitalRead(czujnik);  
  if (ruch == HIGH && (millis() - lastMotionTime > motionInterval)) {  
    digitalWrite(swiatloczujnikruchu, HIGH);
    Serial.println("Motion detected");
    lcd.setCursor(0, 1);
    lcd.print("Motion detected");
    lastMotionTime = millis();
  } 
  if (millis() - lastMotionTime > motionInterval) {
    digitalWrite(swiatloczujnikruchu, LOW);
    if (!fireStatus1) {
      lcd.setCursor(0, 1);
      lcd.print("                "); 
    }
  }
}

void handleClient(WiFiClient client) {
  String currentLine = "";
  while (client.connected()) {
    if (client.available()) {
      char c = client.read();
      if (readString.length() < 100) {
        readString += c;
      }
      if (c == '\n') {
        if (currentLine.length() == 0) {
          // Handle the request
          if (readString.indexOf("GET /?kitchenon") >= 0) {
            digitalWrite(KITCHEN_LED_PIN, HIGH);
            kitchenStatus = true;
          } else if (readString.indexOf("GET /?kitchenoff") >= 0) {
            digitalWrite(KITCHEN_LED_PIN, LOW);
            kitchenStatus = false;
          } else if (readString.indexOf("GET /?bedroomon") >= 0) {
            digitalWrite(BEDROOM_LED_PIN, HIGH);
            bedroomStatus = true;
          } else if (readString.indexOf("GET /?bedroomoff") >= 0) {
            digitalWrite(BEDROOM_LED_PIN, LOW);
            bedroomStatus = false;
          } else if (readString.indexOf("GET /?officeon") >= 0) {
            digitalWrite(OFFICE_LED_PIN, HIGH);
            officeStatus = true;
          } else if (readString.indexOf("GET /?officeoff") >= 0) {
            digitalWrite(OFFICE_LED_PIN, LOW);
            officeStatus = false;
          } else if (readString.indexOf("GET /?livingroomon") >= 0) {
            digitalWrite(LIVINGROOM_LED_PIN, HIGH);
            livingroomStatus = true;
          } else if (readString.indexOf("GET /?livingroomoff") >= 0) {
            digitalWrite(LIVINGROOM_LED_PIN, LOW);
            livingroomStatus = false;
          } else if (readString.indexOf("GET /temperature?set=") >= 0) {
            int pos = readString.indexOf("set=") + 4;
            setTemperature = readString.substring(pos).toFloat();
          } else if (readString.indexOf("GET /motion_status") >= 0) {
            client.print(digitalRead(czujnik));
            client.stop();
          } else if (readString.indexOf("GET /fire_status") >= 0) {
            client.print(digitalRead(FLAME));
            String response = "{\"fire\":";
            response += (fireStatus1 ? "true" : "false");
            response += "}";
            client.stop();
          } else if (readString.indexOf("GET /temperature_status") >= 0) {
            float temperature = getCurrentTemperature();
            String response = "{\"currentTemperature\":";
            response += temperature;
            response += ",\"setTemperature\":";
            response += setTemperature;
            response += ",\"heaterState\":";
            response += (heaterState ? "true" : "false");
            response += "}";
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: application/json");
            client.println("Connection: close");
            client.println();
            client.print(response);
            client.stop();
          } else if (readString.indexOf("GET /heater_status") >= 0) {
            client.print(heaterState ? "1" : "0");
            client.stop();
          } else if (readString.indexOf("GET /light_status") >= 0) {
            String response = "{\"kitchen\":";
            response += (kitchenStatus ? "true" : "false");
            response += ",\"bedroom\":";
            response += (bedroomStatus ? "true" : "false");
            response += ",\"office\":";
            response += (officeStatus ? "true" : "false");
            response += ",\"livingroom\":";
            response += (livingroomStatus ? "true" : "false");
            response += "}";
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: application/json");
            client.println("Connection: close");
            client.println();
            client.print(response);
            client.stop();
          }

          readString = "";
          client.stop();
          Serial.println("client disconnected");
        } else {
          currentLine = "";
        }
      } else if (c != '\r') {
        currentLine += c;
      }
    }
  }
}

void loop() {
  currentTime = millis();

  if (currentTime - lastFireCheckTime >= fireCheckInterval) {
    Fire();
    lastFireCheckTime = currentTime;
  }

  Czujnikruchu();

  val = analogRead(photoPin);
  light = map(val, 0, 1023, 0, 10);

  if (light < 3) {
    digitalWrite(outsideledPin, HIGH);
  } else {
    digitalWrite(outsideledPin, LOW);
  }

  if (millis() - lastClientCheckTime >= clientCheckInterval) {
    WiFiClient client = server.available();
    if (client) {
      Serial.println("new client");
      handleClient(client);
    }
    lastClientCheckTime = millis();
  }

  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);
  if (temperatureC != DEVICE_DISCONNECTED_C) {
    if (temperatureC < setTemperature) {
      digitalWrite(heaterPin, HIGH); 
      heaterState = true;
    } else {
      digitalWrite(heaterPin, LOW); 
      heaterState = false;
    }
  }

    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temperatureC);
    lcd.print(" C ");
    lcd.setCursor(0, 1);
}

float getCurrentTemperature() {
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(0);
}

void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
