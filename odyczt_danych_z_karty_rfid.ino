#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600); // Inicjalizacja portu szeregowego
  SPI.begin();        // Inicjalizacja SPI
  mfrc522.PCD_Init(); // Inicjalizacja MFRC522
}

void loop() {
  RfidScan(); // Wywołanie funkcji RfidScan w pętli głównej
}

void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " "); // Wypisanie danych w formacie HEX
    Serial.print(buffer[i], HEX);
  }
  Serial.println(); // Nowa linia dla czytelności
}

void RfidScan() {
  if (!mfrc522.PICC_IsNewCardPresent())
    return;

  if (!mfrc522.PICC_ReadCardSerial())
    return;

  Serial.println("RFID Card Detected!"); // Wypisanie komunikatu o wykryciu karty RFID
  Serial.print("UID Value: "); // Wypisanie komunikatu o wartości UID
  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size); // Wypisanie numeru karty
}
