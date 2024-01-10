#include <Servo.h>

Servo servo;
int angle = 180;
#define buttonPin D3
bool ongoingClick = false;

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
  pinMode(buttonPin, INPUT_PULLUP);
  unsigned long current_millis;
  unsigned long prev_millis;
}


void loop() {
  // put your main code here, to run repeatedly:

  if (distance() < 30) {
    open();
    delay(5000);
    close();

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
  Serial.print("This is the distance: ");
  Serial.println(L);
  return L;
}
  

void open() {
    angle = 10;
    servo.write(angle);
  }

void close() {
    angle = 180;
    servo.write(angle);
  }

  
