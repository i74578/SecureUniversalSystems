#include "EspMQTTClient.h"
/*
list of categories
acceslist: server sends a list when there is a change. It sends suid and pin to the ESP32. Server --> ESP32
hello:     ESP32 starts by sending Hello to the server who send the acceslist. ESP32 --> Server
logentry:  Sends the log info.          ESP32 --> server

*/
EspMQTTClient client(
  "KelvinPelvin",
  "PelvinKelvin",
  "142.93.105.52",  // MQTT Broker server ip
  "entranceDoor",   // Can be omitted if not needed
  "VMN2rLa8aloFFQ3",   // Can be omitted if not needed
  "TestClient", 
  1883     // Client name that uniquely identify your device
);

struct employee{
  byte NUID[4];
  byte PIN[4];
};

//Access control list
//Stores a pointer to a list of employee structes, which includes NUID and PIN for all employees with access
employee *ptr_ACL;
byte ACL_count;

// Converts a hex char array to a byte array. Example {'0','0','F','F'} -> {0,255}
void hexStr2Bytes (char * HexCharArray, byte * byteArray){
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

// Sends a log message to the MQTT broker, with the 
// employee NUID and if authentification was successful
void sendLog(byte NUID[4],bool success){
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
      ptr_ACL[i].PIN[j] = PINBuffer[j];
    }
  }
}

void setup() {
  Serial.begin(115200);
}

void onConnectionEstablished() {
  client.subscribe("sus/accessList", onAccessListReceived);
  client.publish("sus/hello", "hello from entranceDoor");
  client.publish("sus/helloTest", "Initial");
}

void loop() {
  client.loop();
  if(client.isConnected()){
    Serial.println("Sending");
    client.publish("sus/logTest", "test");
    delay(30000);
  }
  delay(100);

}

