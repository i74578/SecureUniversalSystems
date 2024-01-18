const String payload = "02ABAB12121234AB1FE24A5678";

struct employee{
  byte NUID[4];
  byte PIN[4];
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);


  int employeeCount = payload.substring(0,2).toInt();
  employee employees[employeeCount];


  for(int i=0 ; i<employeeCount;i++){
    String hexString = payload.substring(2,4);
    char hexCharArray[2];
    hexString.toCharArray(hexCharArray,2);
    Serial.println()
    Serial.println((int) strtol(hexCharArray,0,16));


    /*employees[i].NUID = payload.substring(2+12*i,10+12*i);
    employees[i].PIN = payload.substring(10+12*i,14+12*i);
    Serial.println("Employee:");
    Serial.print("NUID:");
    Serial.println(employees[i].NUID);
    Serial.print("PIN:");
    Serial.println(employees[i].PIN);*/

  }


}

void loop() {
  // put your main code here, to run repeatedly:

}
