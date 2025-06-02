#include <Servo.h>

Servo esc;           
const int escPin = 9;

void setup() {
  esc.attach(escPin);

  esc.writeMicroseconds(1000);
  delay(2000);

  esc.writeMicroseconds(1200);
  delay(5000);

  esc.writeMicroseconds(1000);
}

void loop() {
}
