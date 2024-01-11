struct employee {
  byte NUID[4] = {0x00, 0x00, 0x00, 0x00};
  int PIN = 0000;
};

#include <SPI.h>
#include <MFRC522.h>

#define SDA_PIN D8 //pin on ESP8266 connected to SDA on the RC522 RFID reader.
#define RST_PIN D0 //pin connected to RST on the RFID reader.

const int arrLength = 10;
employee employees[arrLength];
employee currentlyLoggingIn; //this is set by startLogin() when a valid ID is scanned. It is then used by enterPIN() to see if the PIN matches.
MFRC522 rfid(SDA_PIN, RST_PIN); //instance of class that interfaces with the RFID reader.
byte detectedNUID[4]; //byte array that will hold the NUID read by the RFID reader.
MFRC522::MIFARE_Key key;


void setup() {
  Serial.begin(9600);
  SPI.begin();      // Init SPI bus
  rfid.PCD_Init();  // Init MFRC522
  Serial.println("Hello world!");
  initArray(employees);
  resetCurrentlyLoggingIn();
  byte myNuid[] = {0x91, 0x5D, 0xDE, 0x1D};
  addEmployee(employees, myNuid, 1234);

  //testSystem();
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent()) //ignores rest of loop if no card is detected
    return;

  if (!rfid.PICC_ReadCardSerial()) //ignores rest of loop if the card isn't being read
    return;

  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI && piccType != MFRC522::PICC_TYPE_MIFARE_1K && piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

  for (byte i = 0; i < 4; i++) {
    detectedNUID[i] = rfid.uid.uidByte[i];
  }
  Serial.print("Detected NUID: ");
  printHex(detectedNUID, 4);
  Serial.println();
  if (startLogin(employees, detectedNUID)) {
        Serial.println("Card accepted, enter PIN:");
      } else {
        Serial.println("Access denied");
        return;
      }
  
  int waitFor = 8000;
  for (int i = 0; i < waitFor; i++) {
    if (Serial.available() > 0) {
      int enteredPIN = Serial.parseInt();
      if (enterPIN(enteredPIN)) {
        Serial.println("Access granted!");
      } else {
        Serial.println("Wrong PIN");
      }
      return;
    }
    delay(1);
  }
  Serial.println("Timed out.");
}

void initArray(employee e[]) {
  for (int i = 0; i < arrLength; i++) {
    e[i].NUID[0] = 0x00;
    e[i].NUID[1] = 0x00;
    e[i].NUID[2] = 0x00;
    e[i].NUID[3] = 0x00;
    e[i].PIN = 0;
  }
}

void addEmployee(employee e[], byte newNUID[], int newPIN) {
  for (int i = 0; i < arrLength; i++) {
    if (e[i].NUID[0] == 0x00 &&
        e[i].NUID[1] == 0x00 &&
        e[i].NUID[2] == 0x00 &&
        e[i].NUID[3] == 0x00) {
      // empty entry found
      e[i].NUID[0] = newNUID[0];
      e[i].NUID[1] = newNUID[1];
      e[i].NUID[2] = newNUID[2];
      e[i].NUID[3] = newNUID[3];
      e[i].PIN = newPIN;
      return;
    }
  }
}

void removeEmployee(employee e[], byte NUID[]) {
  for (int i = 0; i < arrLength; i++) {
    if (e[i].NUID[0] == NUID[0] &&
        e[i].NUID[1] == NUID[1] &&
        e[i].NUID[2] == NUID[2] &&
        e[i].NUID[3] == NUID[3]) {
      // matching entry found
      e[i].NUID[0] = 0x00;
      e[i].NUID[1] = 0x00;
      e[i].NUID[2] = 0x00;
      e[i].NUID[3] = 0x00;
      e[i].PIN = 0;
      return;
    }
  }
}

bool startLogin(employee e[], byte NUID[]) {
// When user has scanned their card and entered the NUID, the system checks whether it exists in the list of people with access.
// If it does, this function returns true and sets currentlyLoggingIn to the matching employee, so that enterPIN() knows which PIN to 

  if (NUID[0] == 0x00 &&
      NUID[1] == 0x00 &&
      NUID[2] == 0x00 &&
      NUID[3] == 0x00) {
    // 00 00 00 00 is used for empty entries in the list, so it's not a valid NUID in our case.
    resetCurrentlyLoggingIn();
    return false;
  }

  for (int i = 0; i < arrLength; i++) {
    if (e[i].NUID[0] == NUID[0] &&
        e[i].NUID[1] == NUID[1] &&
        e[i].NUID[2] == NUID[2] &&
        e[i].NUID[3] == NUID[3]) {
      currentlyLoggingIn = e[i];
      return true;
    }
  }
  resetCurrentlyLoggingIn();
  return false;
}

bool enterPIN(int PIN) {
  // for when the user enters their PIN after scanning their card.

  if (currentlyLoggingIn.NUID[0] == 0x00 &&
      currentlyLoggingIn.NUID[1] == 0x00 &&
      currentlyLoggingIn.NUID[2] == 0x00 &&
      currentlyLoggingIn.NUID[3] == 0x00) {
    // nobody is currently trying to log in.
    return false;
  }

  if (currentlyLoggingIn.PIN == PIN) {
    // entered PIN matches expectations.
    resetCurrentlyLoggingIn();
    return true;
  } else {
    resetCurrentlyLoggingIn();
    return false;
  }

}

void resetCurrentlyLoggingIn() {
  // resets the value of currentlyLoggingIn to one of all zeroes.
  // this prevents enterPIN() from allowing access before a card has been scanned.
  currentlyLoggingIn.NUID[0] = 0x00;
  currentlyLoggingIn.NUID[1] = 0x00;
  currentlyLoggingIn.NUID[2] = 0x00;
  currentlyLoggingIn.NUID[3] = 0x00;
  currentlyLoggingIn.PIN = 0;

}



void printHex(byte *buffer, byte bufferSize) {
  //prints a byte array to Serial, in hex.
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? "0" : ""); //prepends a leading zero to each byte if necessary
    Serial.print(buffer[i], HEX);
  }
}

void testSystem() {
  // Tests:
  byte UID1[] = {0x01, 0x02, 0x23, 0xFF};
  byte UID2[] = {0x21, 0xF2, 0xAB, 0xDD};
  byte UID3[] = {0x21, 0xF2, 0xAB, 0xDF};
  addEmployee(employees, UID1, 1234);
  addEmployee(employees, UID2, 5678);

  if (startLogin(employees, UID1)) {
    Serial.println("(good) Employee succesfully added");
  } else {
    Serial.println("(bad) Something went wrong adding the employee");
  }

  if (enterPIN(1234)) {
    Serial.println("(good) Employee entered PIN succesfully");
  } else {
    Serial.println("(bad) Employee entered wrong PIN");
  }

  if (startLogin(employees, UID3)) {
    Serial.println("(bad) Non-employee was allowed entry");
  } else {
    Serial.println("(good) Non-employee not allowed entry");
  }

  if (enterPIN(1234)) {
    Serial.println("(bad) Entry permitted even though no card was scanned");
  } else {
    Serial.println("(good) No card, no entry");
  }

  startLogin(employees, UID1);
  if (enterPIN(4321)) {
    Serial.println("(bad) Employee allowed access with wrong PIN ");
  } else {
    Serial.println("(good) Employee entered wrong PIN and not allowed access");
  }
}