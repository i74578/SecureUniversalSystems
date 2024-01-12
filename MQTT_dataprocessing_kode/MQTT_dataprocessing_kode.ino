const String payload = "ABAB1212,1234;AB1FE24A,5678;";
byte fieldNum = 0;
String currentField = "NUID";
int currentPIN;
byte currentNUID[2];



struct employee{ //employee struct
  int PIN;
  byte NUID[2];
};



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);


  employee employees[payload.charAt(0)]; // the first char of the payload array determines the amount of employees there are.
  int employee_num = 0;

  for(int i = 1 ; i< payload.length();i++){
    //this if statement finds out wether a "," or a  ";" has been found, if not the else will add the currentchar at index i to either PIN or NUID
    
    if(payload.charAt(i) == ','){ // "," found
      Serial.print("found a ',' at index");
      Serial.println(i);
      currentField = "PIN";
      fieldNum = 0;
      currentPIN = 0;

    } 
    else if (payload.charAt(i) == ';'){ // ";" found, meaning the start of a new employee has begun
      Serial.print("found a ';' at index");
      Serial.println(i);
      currentField = "NUID";
      fieldNum = 0;
      employee temp;
      temp.PIN = currentPIN;

      for (int indx = 0; indx < 4; indx++){
        temp.NUID[indx] = currentNUID[indx];
      }
      employees[employee_num] = temp;
      Serial.print("PIN IS: ");
      Serial.println(temp.PIN);
      employee_num ++;
      
    }


    else //if neither a "," or a ";" has been found this code runs:
    {
      if(currentField == "NUID")
      {
        string tmpstr =  
        currentNUID[fieldNum] = strtol({payload.charAt(i), payload.charAt(i+1)});
        fieldNum++;
        i++;
        Serial.println("a char has been added to the NUID");
        Serial.println(payload.charAt(i));
      }

      else if (currentField == "PIN")
      {
        currentPIN += (payload.charAt(i)-48) * (int)power(10,fieldNum);
        fieldNum++;
        Serial.println("a char has been added to the PIN, current PIN is:");
        Serial.println(currentPIN);

        
      }
      else{
        Serial.println("if you see this something is wrong with NUID/PIN assignments");
      }
    }


  } // end of for loop

  for (int i = 0; i< payload.charAt(0);i++)
  {
    for (int x = 0; x < 4; x++){
      Serial.println( (employees[i].NUID[x]),HEX);
    }
  }

} // end of setup

void loop() {
  // put your main code here, to run repeatedly:



}


int power(int base, int power){
  int result = 1;
    for(int i = 0; i< power;i++)
    {
      result *= base;
    }
    Serial.print("result of power is: ");
    return result;

}

