#include <Arduino.h>

constexpr uint8_t ROTOR1_PIN = 5; 

constexpr uint8_t THROTTLE_PWM = 75; 

void setup() {
  Serial.begin(115200);

  pinMode(ROTOR1_PIN, OUTPUT);
  analogWrite(ROTOR1_PIN, 0);  

  Serial.println(F("Single rotor test starting..."));
}

void loop() {
  analogWrite(ROTOR1_PIN, THROTTLE_PWM);
}
