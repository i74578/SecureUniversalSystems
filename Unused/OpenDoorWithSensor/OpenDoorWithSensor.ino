#include <Servo.h>

Servo servo;
int angle = 0;


float duration;
int L;

#define echoPin D2
#define trigPin D4

void setup() {
  // put your setup code here, to run once:
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(115200);
  servo.attach(D1);
  servo.write(angle);
  unsigned long current_millis;
  unsigned long prev_millis;
}


void loop() {
  if (distance() < 30) {
    door();
  }
}

int distance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  L = (duration * .0343) / 2;
  return L;
}


void door() {
  angle = 180;
  servo.write(angle);
  delay(5000);
  angle = 0;
  servo.write(angle);
}
