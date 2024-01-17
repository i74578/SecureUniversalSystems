/* RECEIVER ESP8266 */

// Create data buffer for messages up to 40 characters
char msgBuffer[40];
volatile byte indx;
#include <Servo.h>
#include <Nokia_LCD.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

Servo servo;
int angle = 0;
//Nokia_LCD lcd(D2, D3, D4, D5, D6);

const int ledPIN = D3;


#define pot_in A0
int input;
byte current_num;
LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
int place = 0;                       // place in the lcd display
byte password[4] = { 1, 2, 3, 4 };
byte enteredPassword[4];  //user entered
byte temp_num = 20;
bool allowed_acces = true;
bool locked = true;

// Set baud and switch Rx/Tx to D7/D8
void setup() {
  pinMode(ledPIN, OUTPUT);
  digitalWrite(ledPIN, LOW);
  Serial.begin(115200);
  Serial.swap();
  servo.attach(D5);
  servo.write(angle);
  // setup af lcd display
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Indtast kode:");
  lcd.setCursor(0, 1);  // sæt cursor på anden række
}
void opendoor() {
  angle = 180;
  servo.write(angle);
}

void closedoor() {
  angle = 0;
  servo.write(angle);
}
void loop() {
  // Listen for received UART data on Rx/Tx (D7/D8)
  if (Serial.available() > 0) {
    byte currentByte = Serial.read();
    if (indx < sizeof msgBuffer) {
      msgBuffer[indx++] = currentByte;

      // If end of message, send to dev computer
      if (currentByte == '\n') {
        if (msgBuffer[0] == 48) {
          opendoor();
          delay(2000);
          closedoor();
        } else if (msgBuffer[0] == 49) {
          digitalWrite(ledPIN, HIGH);
          lcd.setCursor(place, 1);
          indx = 0;
          if (Serial.available() > 0) {
            byte currentByte = Serial.read();
            if (indx < sizeof msgBuffer) {
              msgBuffer[indx++] = currentByte;
              if (currentByte == '\n') {

                lcd.print(msgBuffer);
                place++;

                delay(2000);
                digitalWrite(ledPIN, LOW);
              }

              //String message = String(msgBuffer);
              //lcd.print(message);

              // Serial.swap();
              // delay(200);
              // Serial.println(message);
              // delay(200);
              // Serial.swap();
              // delay(200);
              indx = 0;
            }
          }
        }
      }
    }
  }
}