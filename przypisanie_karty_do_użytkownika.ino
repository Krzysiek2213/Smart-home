#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10

MFRC522 mfrc522(SS_PIN, RST_PIN);
int RfidNo = 0;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
}

void loop() {
  RfidScan();

}

void dump_byte_array(byte *buffer, byte bufferSize) {
  Serial.println("~");
  if (bufferSize == 4 && buffer[0] == 0x53 && buffer[1] == 0x7A && buffer[2] == 0x21 && buffer[3] == 0x14) {
    RfidNo = 1;
    Serial.print(RfidNo);
    Serial.println(" Zeskanowano kartę Krzysztofa!");
  delay(500);
  }
  Serial.print("!");
}

void RfidScan() {
  if (!mfrc522.PICC_IsNewCardPresent())
    return;

  if (!mfrc522.PICC_ReadCardSerial())
    return;

  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
}
