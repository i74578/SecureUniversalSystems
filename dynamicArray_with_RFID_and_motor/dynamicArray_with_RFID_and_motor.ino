struct employee {
  byte NUID[4];
  byte PIN[4];
};

#include <SPI.h>
#include <Servo.h>
#include <MFRC522.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include "EspMQTTClient.h"

#define SDA_PIN 5 //pin on ESP8266 connected to SDA on the RC522 RFID reader.
#define RST_PIN 21 //pin connected to RST on the RFID reader.
// SCK  <-> D5 (ESP8266), D18 (ESP32)
// MISO <-> D6 (ESP8266), D19 (ESP32)
// MOSI <-> D7 (ESP8266), D23 (ESP32)
#define echoPin 22 //pin for echo on ultrasonic sensor.
#define trigPin 2 //pin for trig on ultrasonic sensor.
#define servoPin 27 //pin for orange wire on servo motor
#define potPin 36 //pin for potentiometer
#define closeAngle 180
#define openAngle 90
#define triggerDistance 10

// const int arrLength = 10; //max number of employees
// employee employees[arrLength]; //array to hold all employees permitted entry
employee *ptr_ACL; //Access Control List, stores a pointer to a list of Employee structs.
byte ACL_count;
MFRC522 rfid(SDA_PIN, RST_PIN); //instance of class that interfaces with the RFID reader.
byte detectedNUID[4]; //byte array that will hold the NUID read by the RFID reader.
Servo servo; //instance of class that interfaces with servo motor
int angle = closeAngle; //current angle of the servo motor
Adafruit_PCD8544 display = Adafruit_PCD8544(14, 13, 4, 15, 26); //to interface with Nokia display
                                          //CLK DIN DC  CE  RST
employee currentlyLoggingIn; //this is set by startLogin() when a valid ID is scanned. It is then used by enterPIN() to see if the PIN matches.
EspMQTTClient client( //MQTT client to communicate with server
  "KelvinPelvin",
  "PelvinKelvin",
  "142.93.105.52",  // MQTT Broker server ip
  "entranceDoor",   // Can be omitted if not needed
  "VMN2rLa8aloFFQ3",   // Can be omitted if not needed
  "TestClient", 
  1883     // Client name that uniquely identify your device
);

void setup() {
  Serial.begin(9600);
  SPI.begin();      // Init SPI bus for RFID reader
  rfid.PCD_Init();  // Init MFRC522 RFID reader
  servo.attach(servoPin);
  servo.write(angle); //reset servo to close position
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(potPin, INPUT_PULLUP);
  Serial.println("Hello, world!");
  display.begin();
  display.clearDisplay();
  display.setContrast(60);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println("Hello, world!");
  display.display();
  // initArray();
  resetCurrentlyLoggingIn();
  byte myNuid[] = {0x91, 0x5D, 0xDE, 0x1D};
  byte myPIN[] = {1,2,3,4};
  // addEmployee(myNuid, myPIN);
  delay(2000);
  display.clearDisplay();
  display.display();
  Serial.println("setup() complete");
  
}

void loop() {
  client.loop(); //check if MQTT broker has anything new
  
  //proximity sensor checks:
  if (distance() < triggerDistance && angle != openAngle) {
    opendoor();
    delay(1000);
  } else if (distance() >= triggerDistance && angle != closeAngle) {
    closedoor();
  }

  //RFID reader checks:
  if (!rfid.PICC_IsNewCardPresent()) { //ignores rest of loop if no card is detected
    return;
  }
  if (!rfid.PICC_ReadCardSerial()) { //ignores rest of loop if the card isn't being read
    return;
  }
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
  if (startLogin(detectedNUID)) {
        Serial.println("Card accepted, enter PIN:");
      } else {
        Serial.println("Access denied");
        return;
      }
  
  //card accepted, now to enter PIN with the potentiometer:
  int waitFor = 4000; //time to adjust each digit before they're locked in 
  byte enteredPIN[4] = {0,0,0,0};
  for (int i = 0; i < 4; i++) {
    long startMillis = millis(); //when the loop for the current digit started
    while (millis() - startMillis < waitFor) {
      if (distance() < triggerDistance) { //interrupt the login process if the prox sensor is triggered from inside.
        return;
      }
      int potInput = analogRead(potPin);
      byte current_num = (byte) map(potInput,10,4095,9,0);
      enteredPIN[i] = current_num;
      int progress = map(millis() - startMillis, 0, waitFor, 0, 14); //progress bar
      displayEnteredPIN(enteredPIN, i, progress);
      delay(1);
    }
  }
  if (enterPIN(enteredPIN)) {
    Serial.println("Correct PIN");
    sendLog(currentlyLoggingIn.NUID, true);
    resetCurrentlyLoggingIn();
    opendoor();
    delay(5000);
  } else {
    sendLog(currentlyLoggingIn.NUID, false);
    resetCurrentlyLoggingIn();
    Serial.println("Incorrect PIN");
    display.clearDisplay();
    display.println("Incorrect PIN");
    display.display();
    delay(2000);
    display.clearDisplay();
    display.display();
  }

  // int waitFor = 8000;
  // for (int i = 0; i < waitFor; i++) {
  //   if (distance() < triggerDistance) {
  //     return;
  //   }
  //   if (Serial.available() > 0) {
  //     int enteredPIN = Serial.parseInt();
  //     if (enterPIN(enteredPIN)) {
  //       Serial.println("Access granted!");
  //       opendoor();
  //       delay(5000);
  //     } else {
  //       Serial.println("Wrong PIN");
  //     }
  //     return;
  //   }
  //   delay(1);
  // }
  // Serial.println("Timed out.");
}

// void initArray() {
//   for (int i = 0; i < arrLength; i++) {
//     employees[i].NUID[0] = 0x00;
//     employees[i].NUID[1] = 0x00;
//     employees[i].NUID[2] = 0x00;
//     employees[i].NUID[3] = 0x00;
//     employees[i].PIN[0] = 0;
//     employees[i].PIN[1] = 0;
//     employees[i].PIN[2] = 0;
//     employees[i].PIN[3] = 0;
//   }
// }

// void addEmployee(byte newNUID[], byte newPIN[]) {
//   for (int i = 0; i < arrLength; i++) {
//     if (employees[i].NUID[0] == 0x00 &&
//         employees[i].NUID[1] == 0x00 &&
//         employees[i].NUID[2] == 0x00 &&
//         employees[i].NUID[3] == 0x00) {
//       // empty entry found
//       employees[i].NUID[0] = newNUID[0];
//       employees[i].NUID[1] = newNUID[1];
//       employees[i].NUID[2] = newNUID[2];
//       employees[i].NUID[3] = newNUID[3];
//       employees[i].PIN[0] = newPIN[0];
//       employees[i].PIN[1] = newPIN[1];
//       employees[i].PIN[2] = newPIN[2];
//       employees[i].PIN[3] = newPIN[3];
//       return;
//     }
//   }
// }

// void removeEmployee(byte NUID[]) {
//   for (int i = 0; i < arrLength; i++) {
//     if (employees[i].NUID[0] == NUID[0] &&
//         employees[i].NUID[1] == NUID[1] &&
//         employees[i].NUID[2] == NUID[2] &&
//         employees[i].NUID[3] == NUID[3]) {
//       // matching entry found
//       employees[i].NUID[0] = 0x00;
//       employees[i].NUID[1] = 0x00;
//       employees[i].NUID[2] = 0x00;
//       employees[i].NUID[3] = 0x00;
//       employees[i].PIN[0] = 0;
//       employees[i].PIN[1] = 0;
//       employees[i].PIN[2] = 0;
//       employees[i].PIN[3] = 0;
//       return;
//     }
//   }
// }

bool startLogin(byte NUID[]) {
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

  for (int i = 0; i < ACL_count; i++) {
    if (ptr_ACL[i].NUID[0] == NUID[0] &&
        ptr_ACL[i].NUID[1] == NUID[1] &&
        ptr_ACL[i].NUID[2] == NUID[2] &&
        ptr_ACL[i].NUID[3] == NUID[3]) {
      currentlyLoggingIn = ptr_ACL[i];
      return true;
    }
  }
  resetCurrentlyLoggingIn();
  return false;
}

bool enterPIN(byte enteredPIN[]) {
  // for when the user enters their PIN after scanning their card.

  if (currentlyLoggingIn.NUID[0] == 0x00 &&
      currentlyLoggingIn.NUID[1] == 0x00 &&
      currentlyLoggingIn.NUID[2] == 0x00 &&
      currentlyLoggingIn.NUID[3] == 0x00) {
    // nobody is currently trying to log in.
    return false;
  }

  if (currentlyLoggingIn.PIN[0] == enteredPIN[0] &&
      currentlyLoggingIn.PIN[1] == enteredPIN[1] &&
      currentlyLoggingIn.PIN[2] == enteredPIN[2] &&
      currentlyLoggingIn.PIN[3] == enteredPIN[3]) {
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
  currentlyLoggingIn.PIN[0] = 0;
  currentlyLoggingIn.PIN[1] = 0;
  currentlyLoggingIn.PIN[2] = 0;
  currentlyLoggingIn.PIN[3] = 0;

}

void opendoor() {
  angle = openAngle;
  servo.write(angle);
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Welcome!");
  display.display();
}

void closedoor(){
  angle = closeAngle;
  servo.write(angle);
  display.clearDisplay();
  display.display();
}

// functions used by MQTT:

void sendLog(byte NUID[4], bool success){
  String logPayload = success ? "1" : "0";
  for (int i=0; i< 4; i++){
    logPayload += String(NUID[i], HEX);
  }
  logPayload.toUpperCase();
  Serial.print("Sending log entry: ");
  Serial.println(logPayload);
  client.publish("sus/logEntry", logPayload);
}

void onAccessListReceived(const String &payload){
  //When MQTT broker sends an updated access list
  Serial.print("ACL:");
  Serial.println(payload);

  int employeeCount = payload.substring(0,2).toInt();

  // Delete array pointed to by ptr_ACL and create a new struct array with each employee
  if (ptr_ACL != NULL) {
    delete[] ptr_ACL;
  }
  ptr_ACL = new employee[employeeCount];
  ACL_count = (byte) employeeCount;

  // Itterate for each employee in received message
  for(int i=0 ; i<employeeCount;i++){

    //Extract NUID, and save to ACL
    char NUIDBuffer[9];
    payload.substring(2+12*i,10+12*i).toCharArray(NUIDBuffer, 9);
    hexStr2Bytes(NUIDBuffer,ptr_ACL[i].NUID);

    //Extract PIN, and save to ACL
    char PINBuffer[5];
    payload.substring(10+12*i,14+12*i).toCharArray(PINBuffer, 5);
    for (int j=0;j<4;j++){
      ptr_ACL[i].PIN[j] = PINBuffer[j]-48; //quick conversion from char to decimal
    }
  }
}

void onConnectionEstablished() {
  client.subscribe("sus/accessList", onAccessListReceived);
  client.publish("sus/hello", "hello from entranceDoor");
}

// helper functions:

void displayEnteredPIN(byte digits[], byte currentByte, int progress) {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Enter PIN:");
  if (currentByte == 0) {
    display.print("[");
    display.print(digits[0]);
    display.print("]");
    display.print(digits[1]);
    display.print(" ");
    display.print(digits[2]);
    display.print(" ");
    display.print(digits[3]);
    display.println(" ");
  } else if (currentByte == 1) {
    display.print(" ");
    display.print(digits[0]);
    display.print("[");
    display.print(digits[1]);
    display.print("]");
    display.print(digits[2]);
    display.print(" ");
    display.print(digits[3]);
    display.println(" ");
  } else if (currentByte == 2) {
    display.print(" ");
    display.print(digits[0]);
    display.print(" ");
    display.print(digits[1]);
    display.print("[");
    display.print(digits[2]);
    display.print("]");
    display.print(digits[3]);
    display.println(" ");
  } else if (currentByte == 3) {
    display.print(" ");
    display.print(digits[0]);
    display.print(" ");
    display.print(digits[1]);
    display.print(" ");
    display.print(digits[2]);
    display.print("[");
    display.print(digits[3]);
    display.println("]");
  } else {
    display.print(" ");
    display.print(digits[0]);
    display.print(" ");
    display.print(digits[1]);
    display.print(" ");
    display.print(digits[2]);
    display.print(" ");
    display.print(digits[3]);
    display.println(" ");
  }
  for (int i = 0; i < progress; i++) {
    display.print("-");
  }
  display.display();
}

int distance() {
  // calculates distance from proximity sensor.
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  float duration = pulseIn(echoPin, HIGH);
  
  return (duration * .0343) / 2;
}

void printHex(byte *buffer, byte bufferSize) {
  //prints a byte array to Serial, in hex.
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? "0" : ""); //prepends a leading zero to each byte if necessary
    Serial.print(buffer[i], HEX);
  }
}


void hexStr2Bytes (char * HexCharArray, byte * byteArray){ 
  // Converts a hex char array to a byte array. Example {'0','0','F','F'} -> {0,255}
  for (int i=0; i< 8; i+=2){
    char tempstr[3] = {HexCharArray[i],HexCharArray[i+1]};  
    byteArray[i/2] = (byte) strtol(tempstr,0, 16);
  }
}

void printEmployeeStruct(int ACLnum){
  Serial.print("Employee:\NNUID:");
    for (int j = 0; j < 4; j++){
      Serial.print((int) ptr_ACL[ACLnum].NUID[j]);
      Serial.print(" ");
    }
    Serial.print("\nPIN:");
    for (int j = 0; j < 4; j++){
      Serial.print((char) ptr_ACL[ACLnum].PIN[j]);
      Serial.print(" ");
    }
    Serial.print("\n");
}

// void testAccessControl() {
//   // Tests:
//   byte UID1[] = {0x01, 0x02, 0x23, 0xFF};
//   byte UID2[] = {0x21, 0xF2, 0xAB, 0xDD};
//   byte UID3[] = {0x21, 0xF2, 0xAB, 0xDF};
//   byte PIN1[] = {1, 2, 3, 4};
//   byte PIN2[] = {5, 6, 7, 8};
//   addEmployee(UID1, PIN1);
//   addEmployee(UID2, PIN2);

//   if (startLogin(UID1)) {
//     Serial.println("(good) Employee succesfully added");
//   } else {
//     Serial.println("(bad) Something went wrong adding the employee");
//   }

//   if (enterPIN(PIN1)) {
//     Serial.println("(good) Employee entered PIN succesfully");
//   } else {
//     Serial.println("(bad) Employee entered wrong PIN");
//   }

//   if (startLogin(UID3)) {
//     Serial.println("(bad) Non-employee was allowed entry");
//   } else {
//     Serial.println("(good) Non-employee not allowed entry");
//   }

//   if (enterPIN(PIN1)) {
//     Serial.println("(bad) Entry permitted even though no card was scanned");
//   } else {
//     Serial.println("(good) No card, no entry");
//   }

//   startLogin(UID1);
//   if (enterPIN(PIN1)) {
//     Serial.println("(bad) Employee allowed access with wrong PIN ");
//   } else {
//     Serial.println("(good) Employee entered wrong PIN and not allowed access");
//   }
// }