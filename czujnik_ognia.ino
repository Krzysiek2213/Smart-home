
  
/*
 * This is code for  Flame Sensor module for Arduino
* watch the video https://youtu.be/OZkZIP3KlHQ
  * Other Arduino library and videos http://robojax.com/learn/arduino/
 *  * 
	Flame Sensor module for Arduino
	Written by Ahmad Shamshiri for Robojax.com
	on Jan 18, 2018 at 22:21 in Ajax, Ontario, Canada
 * Permission granted to share this code given that this
 * note is kept with the code.
 * Disclaimer: this code is "AS IS" and for educational purpose only.
 * 
 * 
 */
/*


What it does?
When the Infrared diode sees the flame, it sends the digital out DO to
also the level of flame is represented with Analog output

 */
  // 4 Infrared Obstacle code for Robojax.com
#define FLAME 2 // connect DO pin of sensor to this pin
#define ALARM 8 // pin 8 for Alarm 


void setup() {
  Serial.begin(9600);
  pinMode(FLAME, INPUT);//define FLAME input pin
  pinMode(ALARM, OUTPUT);//define ALARM output pin
  // Flame sensor code for Robojax.com

}

void loop() {
    // Flame sensor code for Robojax.com

  int fire = digitalRead(FLAME);// read FLAME sensor


  if( fire == HIGH)
  {
    digitalWrite(ALARM,HIGH);// set the buzzer ON
    Serial.println("Fire! Fire!");
  }else{
    digitalWrite(ALARM,LOW); // Set the buzzer OFF
    Serial.println("Peace");
    // Flame sensor code for Robojax.com

  }



  delay(200);
}