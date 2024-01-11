/*
 * Code to interface with an RC522 RFID-reader using an ESP8266 MCU.
 * Retrieved from https://www.aranacorp.com/en/using-an-rfid-module-with-an-esp8266/
 */
#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN D8
#define RST_PIN D0
#define echoPin D2
#define trigPin D4

float duration;
int L;
Servo servo;
int angle = 0;
const byte correctCard[4] = { 0x91, 0x5D, 0xDE, 0x1D };

MFRC522 rfid(SS_PIN, RST_PIN);  // Instance of the class

MFRC522::MIFARE_Key key;

// Init array that will store new NUID
byte nuidPICC[4];

void setup() {
  Serial.begin(115200);
  SPI.begin();      // Init SPI bus
  rfid.PCD_Init();  // Init MFRC522
  Serial.println();
  Serial.print(F("Reader :"));
  rfid.PCD_DumpVersionToSerial();
  servo.attach(D1);
  servo.write(angle);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  Serial.println();
  Serial.println(F("This code scan the MIFARE Classic NUID."));
  Serial.print(F("Using the following key:"));
  printHex(key.keyByte, MFRC522::MF_KEY_SIZE);
  Serial.println();
}

void loop() {

  if (distance() < 10) {
    opendoor();
    delay(1000);
  }
  if (distance() > 10) {
    closedoor();
  }
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if (!rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if (!rfid.PICC_ReadCardSerial())
    return;

  //Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  //Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI && piccType != MFRC522::PICC_TYPE_MIFARE_1K && piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

  // Store NUID into nuidPICC array
  // for (byte i = 0; i < 4; i++) {
  //   nuidPICC[i] = rfid.uid.uidByte[i];
  // }

  // Serial.println(F("The NUID tag is:"));
  // Serial.print(F("In hex: "));
  printHex(rfid.uid.uidByte, rfid.uid.size);
  Serial.println();
  if (rfid.uid.uidByte[0] == correctCard[0] && rfid.uid.uidByte[1] == correctCard[1] && rfid.uid.uidByte[2] == correctCard[2] && rfid.uid.uidByte[3] == correctCard[3]) {
    
    opendoor();
    Serial.println("Access granted.");
    delay(5000);
  } else {
    Serial.println("Access denied.");
  }


  // Serial.println();
  // Serial.print(F("In dec: "));
  // printDec(rfid.uid.uidByte, rfid.uid.size);

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}


/**
 		Helper routine to dump a byte array as hex values to Serial.
*/
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? "0" : "");  //prepends a leading zero to each byte if necessary
    Serial.print(buffer[i], HEX);
  }
}

/**
 		Helper routine to dump a byte array as dec values to Serial.
*/
void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}



void opendoor() {
  angle = 180;
  servo.write(angle);
  }


void closedoor(){
    angle = 0;
    servo.write(angle);
}

int distance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  L = (duration * .0343) / 2;
  return L;
}
