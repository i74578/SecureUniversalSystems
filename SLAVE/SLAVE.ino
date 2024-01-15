/* RECEIVER ESP8266 */

// Create data buffer for messages up to 40 characters
char msgBuffer[40];
volatile byte indx;
#include <Servo.h>
#include <Nokia_LCD.h>

Servo servo;
int angle = 0;
Nokia_LCD lcd(D2, D3, D4, D5, D6);

const int ledPIN = D3;


// Set baud and switch Rx/Tx to D7/D8
void setup() {
  pinMode(ledPIN, OUTPUT);
  digitalWrite(ledPIN, LOW);
  Serial.begin(115200);
  Serial.swap();
  servo.attach(D1);
  servo.write(angle);
  lcd.begin();
  lcd.setContrast(60);
  lcd.clear();
  lcd.setCursor(0, 0);
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
void opendoor() {
  angle = 180;
  servo.write(angle);
}

void closedoor() {
  angle = 0;
  servo.write(angle);
}