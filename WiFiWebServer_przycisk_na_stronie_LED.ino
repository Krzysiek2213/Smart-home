/*
  WiFi Web Server

 A simple web server that shows the value of the analog input pins.

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the WiFi.begin() call accordingly.

 Circuit:
 * Analog inputs attached to pins A0 through A5 (optional)

 created 13 July 2010
 by dlf (Metodo2 srl)
 modified 31 May 2012
 by Tom Igoe


  Find the full UNO R4 WiFi Network documentation here:
  https://docs.arduino.cc/tutorials/uno-r4-wifi/wifi-examples#wi-fi-web-server
 */

#include "WiFiS3.h"



#include "arduino_secrets.h" 
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                 // your network key index number (needed only for WEP)
//dioda i przycisk
const char BUTTON_PIN = 2;
const char LED_PIN = 3;
bool pressed = false;
bool ledState = false;
String readString;
int status = WL_IDLE_STATUS;

WiFiServer server(80);
/*void handleButton() {
  bool currentState = digitalRead(BUTTON_PIN);

  if (currentState != pressed) {
    // Przełącz stan diody LED tylko w momencie, gdy przycisk zostanie zwolniony
    if (!ledState) {
      Serial.println("Button pressed. LED turned on."); // Wiadomość, gdy przycisk jest naciśnięty i dioda LED jest włączona
      digitalWrite(LED_PIN, HIGH); // Włącz diodę LED
      ledState = true; // Ustaw stan diody LED na włączony
    } else {
      Serial.println("Button pressed. LED turned off."); // Wiadomość, gdy przycisk jest naciśnięty i dioda LED jest wyłączona
      digitalWrite(LED_PIN, LOW); // Wyłącz diodę LED
      ledState = false; // Ustaw stan diody LED na wyłączony
    }
    
    // Odczekaj, aż przycisk zostanie zwolniony
    while(digitalRead(BUTTON_PIN) != pressed) {
      delay(10);
    }
    delay(100); // Opóźnienie dla stabilności
  }
}*/

void setup() {

  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  server.begin();
  // you're connected now, so print out the status:
  printWifiStatus();
}


void loop() {
  WiFiClient client = server.available();
  if (client)
  {
    Serial.println("new client");

    while (client.connected())
    {
      if (client.available())
      {
        char c = client.read();
        if (readString.length() < 100)
        {
          readString += c;
          Serial.write(c);
          
          if (c == '\n') {
            client.println("<a href=\"/?lighton\"\">Turn On Light</a>");
            client.println("<br />");
            client.println("<br />");
            client.println("<a href=\"/?lightoff\"\">Turn Off Light</a><br />");     

            delay(1);
            
            if(readString.indexOf("?lighton") > 0)
            {
              digitalWrite(LED_PIN, HIGH);
              delay(1);
            }
            else{
              if(readString.indexOf("?lightoff") > 0)
              {
                digitalWrite(LED_PIN, LOW);    
                delay(1);
              }
            }           
            readString="";

            delay(1);
            client.stop();
            Serial.println("client disonnected");
          }
        }
      }
    }
  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
