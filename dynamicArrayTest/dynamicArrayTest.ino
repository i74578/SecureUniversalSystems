struct employee {
  byte NUID[4] = {0x00, 0x00, 0x00, 0x00};
  int PIN = 0000;
};

const int arrLength = 10;
int employeeCount = 0;
employee employees[arrLength];
employee currentlyLoggingIn; //this is set by startLogin() when a valid ID is scanned. It is then used by enterPIN() to see if the PIN matches.

void setup() {
  Serial.begin(9600);
  Serial.println("Hello world!");
  initArray(employees);
  resetCurrentlyLoggingIn();
  byte UID1[] = {0x01, 0x02, 0x23, 0xFF};
  byte UID2[] = {0x21, 0xF2, 0xAB, 0xDD};
  byte UID3[] = {0x21, 0xF2, 0xAB, 0xDF};
  addMember(employees, UID1, 1234);
  addMember(employees, UID2, 5678);

  startLogin()

  

}

void loop() { 

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

void addMember(employee e[], byte newNUID[], int newPIN) {
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

void removeMember(employee e[], byte NUID[]) {
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
// If it does, this function returns true and sets 

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



}

void resetCurrentlyLoggingIn() {
  //resets the value of currentlyLoggingIn to one of all zeroes.
  currentlyLoggingIn.NUID[0] = 0x00;
  currentlyLoggingIn.NUID[1] = 0x00;
  currentlyLoggingIn.NUID[2] = 0x00;
  currentlyLoggingIn.NUID[3] = 0x00;
  currentlyLoggingIn.PIN = 0;

}
