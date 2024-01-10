struct member {
  byte NUID[4] = {0x00, 0x00, 0x00, 0x00};
  int PIN = 0000;
};

const int arrLength = 10;
member arr[arrLength];

void setup() {
  Serial.begin(9600);
  Serial.println("Hello world!");
  initArray(arr);
  byte temp[] = {0x01, 0x02, 0x23, 0xFF};
  byte temp2[] = {0x21, 0xF2, 0xAB, 0xDD};
  addMember(arr, temp, 1234);
  addMember(arr, temp2, 5678);
  if (checkAccess(arr, temp)) {
    Serial.println("Yes!!");
  } else {
    Serial.println("defeat...");
  }

}

void loop() {

}

void initArray(member m[]) {
  for (int i = 0; i < arrLength; i++) {
    m[i].NUID[0] = 0x00;
    m[i].NUID[1] = 0x00;
    m[i].NUID[2] = 0x00;
    m[i].NUID[3] = 0x00;
    m[i].PIN = 0;
  }
}

void addMember(member m[], byte newNUID[], int newPIN) {
  for (int i = 0; i < arrLength; i++) {
    if (m[i].NUID[0] == 0x00 &&
        m[i].NUID[1] == 0x00 &&
        m[i].NUID[2] == 0x00 &&
        m[i].NUID[3] == 0x00) {
      // empty entry found
      m[i].NUID[0] = newNUID[0];
      m[i].NUID[1] = newNUID[1];
      m[i].NUID[2] = newNUID[2];
      m[i].NUID[3] = newNUID[3];
      m[i].PIN = newPIN;
      return;
    }
  }
}
bool checkAccess(member m[], byte NUID[]) {

  if (m[i].NUID[0] == 0x00 &&
      m[i].NUID[1] == 0x00 &&
      m[i].NUID[2] == 0x00 &&
      m[i].NUID[3] == 0x00) {
    // 00 00 00 00 is used for empty entries in the list, so it's not a valid NUID in our case.
    return false;
  }

  for (int i = 0; i < arrLength; i++) {
    if (m[i].NUID[0] == NUID[0] &&
        m[i].NUID[1] == NUID[1] &&
        m[i].NUID[2] == NUID[2] &&
        m[i].NUID[3] == NUID[3]) {
      return true;
    }
  }
  return false;
}