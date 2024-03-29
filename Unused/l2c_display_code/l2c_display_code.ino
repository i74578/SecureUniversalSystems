#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#define pot_in A0
int input;
byte current_num;
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
const long interval = 2000; //hvor lang tid man skal være på det samme tal indeen den godtager.
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;
int place = 0; // place in the lcd display
byte password[4] = {1,2,3,4};
byte enteredPassword[4]; //user entered
byte temp_num = 20;
bool allowed_acces = true;
bool locked = true; 

void setup() {
  // Serial setup
  Serial.begin(15200);
  
  // setup af potentiometer:
  pinMode(pot_in,INPUT_PULLUP); //initialiser potentiometer

  // setup af lcd display
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Indtast kode:");
  lcd.setCursor(0,1); // sæt cursor på anden række
}

void loop() {


  if(locked){//while (locked){
  // processering af input fra potentiometer
  input=analogRead(pot_in);
  current_num = (int) map(input,10,1023,0,9);

  //LCD display:
  
  lcd.setCursor(place,1);
  lcd.print(current_num);
  if (current_num != temp_num){
  temp_num = current_num; // new number
  currentMillis = millis();
  previousMillis = currentMillis; //reset timer
  } 
  else
  {
  currentMillis = millis();
  }

  // timer
  
  if (currentMillis - previousMillis >= interval && current_num == temp_num) {
    // gem hvad  tiden var sidst gang den tjekkede
    previousMillis = currentMillis;
    lcd.setCursor(place,1);
    lcd.print(current_num);
    enteredPassword[place] = current_num;
    place++;


    if (place >=4){
      Serial.print(place);
      for (int i = 0;i<place;i++)
      {
        if(password[i]!=enteredPassword[i]){
          allowed_acces = false;
        }
        lcd.setCursor(place,1);
      }

      if (allowed_acces){
        lcd.setCursor(10,1);
        lcd.print("KLIK!"); //åben døren
        locked = false;
      }
      else
      {
        lcd.setCursor(10,1);
        lcd.print("NEJ!");
        delay(2000);

        allowed_acces = true;
        place = 0;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Indtast kode1:");
        temp_num = 15;
      }
    }
    
  }

  }

}
