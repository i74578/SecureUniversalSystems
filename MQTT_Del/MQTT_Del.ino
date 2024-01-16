#include "EspMQTTClient.h"
/*
liste af categorier
acceslist: server sender liste ved ændringer. Sender suid og pin server -> arduino
hello:     arduino starter op og sender hello til server som sender acceslist via acceslist           arduino ->server
logentry:  sender log info          arduino -> server

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

employee *ptr_ACL;
byte ACL_count;


void hexStr2Bytes (char * c_ar, byte b_ar[]){
  for (int i=0; i< 8; i+=2){
    char tempstr[3] = {c_ar[i],c_ar[i+1]};  
    b_ar[i/2] = (byte) strtol(tempstr,0, 16);
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

  //sendLog(ACL[0].NUID,true);
}

void setup() {
  Serial.begin(115200);
}

void onConnectionEstablished() {
  client.subscribe("sus/accessList", onAccessListReceived);
  client.publish("sus/hello", "hello from entranceDoor");
}

void loop() {



  for (int i=0;i<50;i++){
    client.loop();
    if(client.isConnected()){
      //Do something
    }
    delay(100);
  }
}

