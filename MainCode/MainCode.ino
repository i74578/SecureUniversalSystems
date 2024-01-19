#include <SPI.h>
#include <Servo.h>
#include <MFRC522.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include "EspMQTTClient.h"

#define SDA_PIN 5   // pin on ESP8266 connected to SDA on the RC522 RFID reader
#define RST_PIN 21  // pin connected to RST on the RFID reader
// SCK  <-> D18 (ESP32)
// MISO <-> D19 (ESP32)
// MOSI <-> D23 (ESP32)
#define echoPin 22  // pin for echo on ultrasonic sensor
#define trigPin 2   // pin for trig on ultrasonic sensor
#define servoPin 27 // pin for orange wire on servo motor
#define potPin 36   // pin for potentiometer

#define closeAngle 180      // servo angle where the door is closed
#define openAngle 90        // servo angle where the door is open
#define triggerDistance 7   // distance at which proximity sensor will trigger 
#define openDelay 5000      // milliseconds to keep the door open before closing it again
#define digitDelay 4000     // milliseconds to enter each digit in the PIN

struct employee { // struct for each card that has access, stores the card's NUID and the user's chosen PIN
  byte NUID[4];
  byte PIN[4];
};

employee *ptr_ACL; // Access Control List, stores a pointer to a list of Employee structs
byte ACL_count;
MFRC522 rfid(SDA_PIN, RST_PIN); // instance of class that interfaces with the RFID reader
byte detectedNUID[4]; // byte array that will hold the NUID read by the RFID reader
Servo servo; // instance of class that interfaces with servo motor
int angle = closeAngle; // current angle of the servo motor
Adafruit_PCD8544 display = Adafruit_PCD8544(14, 13, 4, 15, 26); // to interface with Nokia display (CLK, DIN, DC, CE, RST)
employee currentlyLoggingIn; // this is set by startLogin() when a valid ID is scanned. It is then used by enterPIN() to see if the PIN matches
EspMQTTClient client( // MQTT client to communicate with server
  "KelvinPelvin",     // WiFi name
  "PelvinKelvin",     // and password
  "142.93.105.52",    // MQTT Broker server ip
  "entranceDoor",     // Can be omitted if not needed
  "VMN2rLa8aloFFQ3",  // Can be omitted if not needed
  "TestClient", 
  1883                // Client name that uniquely identify your device
);

void setup() {
  Serial.begin(9600);
  SPI.begin();      // Init SPI bus for RFID reader
  rfid.PCD_Init();  // Init MFRC522 RFID reader
  servo.attach(servoPin);
  servo.write(angle); // reset servo to close position
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(potPin, INPUT_PULLUP);
  Serial.println("Hello, world!");
  // initialize display:
  display.begin();
  display.clearDisplay();
  display.setContrast(60);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println("Hello, world!");
  display.display();
  delay(2000);
  display.clearDisplay();
  display.display();

  client.setKeepAlive(30); // MQTT expects a keepalive message with a specified interval, the default value of 15 is too low for our program
  resetCurrentlyLoggingIn(); 
  Serial.println("setup() complete");
}

void loop() {
  
  // MQTT checks:
  client.loop(); // check if MQTT broker has anything new

  if (!client.isMqttConnected()) {
    // program can still run with the latest received info if connection is lost, so this is mostly a debug tool/warning
    Serial.println("MQTT disconnected!"); 
  }
  if (!client.isWifiConnected()) {
    Serial.println("WiFi disconnected!");
  }

  // proximity sensor checks:
  if (distance() < triggerDistance && angle != openAngle) {
    opendoor();
    delay(openDelay);
  } else if (distance() >= triggerDistance && angle != closeAngle) {
    closedoor();
  }

  // RFID reader checks:
  if (!rfid.PICC_IsNewCardPresent()) { // resets to beginning of the loop if no card is detected
    return;
  }
  if (!rfid.PICC_ReadCardSerial()) { // resets to beginning of the loop if the card isn't being read
    return;
  }
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak); // resets to beginning of the loop if the card isn't of the correct type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI && piccType != MFRC522::PICC_TYPE_MIFARE_1K && piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

  for (byte i = 0; i < 4; i++) {
    // load the first four bytes of the card NUID into the program
    // some cards can have more than 4 bytes, so this step saves our program from imploding in that scenario
    detectedNUID[i] = rfid.uid.uidByte[i];  
  }
  Serial.print("Detected NUID: ");
  printHex(detectedNUID, 4);
  Serial.println();
  if (startLogin(detectedNUID)) {
        Serial.println("Card accepted, enter PIN:");
      } else {
        // card was not in the database, reset login process
        Serial.println("Access denied");
        display.clearDisplay();
        display.println("Access denied");
        display.display();
        delay(2000);
        display.clearDisplay();
        display.display();
        return;
      }
  
  // card accepted, now to enter PIN with the potentiometer:

  byte enteredPIN[4] = {0,0,0,0};
  for (int i = 0; i < 4; i++) {
    long startMillis = millis(); // when the loop for the current digit started
    while (millis() - startMillis < digitDelay) { 
      if (distance() < triggerDistance) { // interrupt the login process if the prox sensor is triggered from inside
        return;
      }
      int potInput = analogRead(potPin);
      byte current_num = (byte) map(potInput, 0, 4095, 9, 0); // maps the  potentiometer input from 0-4095 to 0-9
      enteredPIN[i] = current_num; 
      int progress = map(millis() - startMillis, 0, digitDelay, 0, 14); //progress bar, the display is around 14 characters wide
      displayEnteredPIN(enteredPIN, i, progress);
      delay(1);
    }
  }
  if (enterPIN(enteredPIN)) {
    // correct pin, open the door and then reset
    Serial.println("Correct PIN");
    sendLog(currentlyLoggingIn.NUID, true);
    opendoor();
    delay(openDelay);
    resetCurrentlyLoggingIn();
  } else {
    // wrong pin, reset everything
    sendLog(currentlyLoggingIn.NUID, false);
    Serial.println("Incorrect PIN");
    display.clearDisplay();
    display.println("Incorrect PIN");
    display.display();
    delay(2000);
    resetCurrentlyLoggingIn();
    display.clearDisplay();
    display.display();
  }

}

bool startLogin(byte NUID[]) {
// When user has scanned their card and entered the NUID, the system checks whether it exists in the list of people with access.
// If it does, this function returns true and sets currentlyLoggingIn to the matching employee, so that enterPIN() knows which PIN to check against. 

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

  if (currentlyLoggingIn.PIN[0] == enteredPIN[0] &&
      currentlyLoggingIn.PIN[1] == enteredPIN[1] &&
      currentlyLoggingIn.PIN[2] == enteredPIN[2] &&
      currentlyLoggingIn.PIN[3] == enteredPIN[3]) {
    // entered PIN matches expectations.
    return true;
  } else {
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

void onConnectionEstablished() {
  Serial.println("Hello to MQTT");
  client.subscribe("sus/accessList", onAccessListReceived);
  client.publish("sus/hello", "hello from entranceDoor");
}

void sendLog(byte NUID[4], bool success){
  // when an unlock attempt (successful or not) is made, the success status and NUID are published to the sus/logEntry MQTT channel.
  String logPayload = success ? "1" : "0";
  for (int i=0; i< 4; i++){
    logPayload += NUID[i] < 0x10 ? "0" : ""; // prepend leading zero if necessary, so every byte is two characters.
    logPayload += String(NUID[i], HEX);
  }

  if (!client.isConnected()) {
    Serial.println("Client disconnected while trying to send log!");
    return;
  }
  logPayload.toUpperCase();
  Serial.print("Sending log entry: ");
  Serial.println(logPayload);
  client.publish("sus/logEntry", logPayload);
}

void onAccessListReceived(const String &payload){
  // called by MQTT when a new access list is received from the sus/accessList channel
  // this function parses the ACL string and a new array of employee structs.
  
  Serial.print("ACL:");
  Serial.println(payload);

  int employeeCount = payload.substring(0,2).toInt(); // first two characters in the payload is always the number of employees included

  // delete array pointed to by ptr_ACL and create a new struct array with each employee
  if (ptr_ACL != NULL) {
    delete[] ptr_ACL;
  }
  ptr_ACL = new employee[employeeCount];
  ACL_count = (byte) employeeCount;

  // iterate for each employee in received message
  for(int i=0; i<employeeCount; i++){

    //Extract NUID, and save to ACL
    char NUIDBuffer[9];
    payload.substring(2+12*i, 10+12*i).toCharArray(NUIDBuffer, 9); // character 2 to 9 of each employee is the 4-byte NUID
    hexStr2Bytes(NUIDBuffer,ptr_ACL[i].NUID);

    //Extract PIN, and save to ACL
    char PINBuffer[5];
    payload.substring(10+12*i, 14+12*i).toCharArray(PINBuffer, 5); // character 10 to 13 of each employee is the 4-digit PIN
    for (int j=0; j<4; j++){
      ptr_ACL[i].PIN[j] = PINBuffer[j]-48; //quick conversion from char to decimal
    }
  }
}

// helper functions:

void displayEnteredPIN(byte digits[], byte currentByte, int progress) {
  // used to show text and numbers on the display while user is entering their PIN with potentiometer.
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Enter PIN:");
  if (currentByte == 0) {
    display.print("[");
    display.print(digits[0]);
    display.print("] ");
    display.print(digits[1]);
    display.print("  ");
    display.print(digits[2]);
    display.print("  ");
    display.print(digits[3]);
    display.println(" ");
  } else if (currentByte == 1) {
    display.print(" ");
    display.print(digits[0]);
    display.print(" [");
    display.print(digits[1]);
    display.print("] ");
    display.print(digits[2]);
    display.print("  ");
    display.print(digits[3]);
    display.println(" ");
  } else if (currentByte == 2) {
    display.print(" ");
    display.print(digits[0]);
    display.print("  ");
    display.print(digits[1]);
    display.print(" [");
    display.print(digits[2]);
    display.print("] ");
    display.print(digits[3]);
    display.println(" ");
  } else if (currentByte == 3) {
    display.print(" ");
    display.print(digits[0]);
    display.print("  ");
    display.print(digits[1]);
    display.print("  ");
    display.print(digits[2]);
    display.print(" [");
    display.print(digits[3]);
    display.println("]");
  } else {
    display.print(" ");
    display.print(digits[0]);
    display.print("  ");
    display.print(digits[1]);
    display.print("  ");
    display.print(digits[2]);
    display.print("  ");
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
  // prints a byte array to Serial, in hex.
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? "0" : ""); // prepends a leading zero to each byte if necessary
    Serial.print(buffer[i], HEX);
  }
}

void hexStr2Bytes (char * HexCharArray, byte * byteArray){ 
  // converts a hex char array to a byte array. Example {'0','0','F','F'} -> {0,255}
  for (int i=0; i< 8; i+=2){
    char tempstr[3] = {HexCharArray[i],HexCharArray[i+1]};  
    byteArray[i/2] = (byte) strtol(tempstr,0, 16);
  }
}

void printEmployeeStruct(int ACLnum){
  // used for debugging
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