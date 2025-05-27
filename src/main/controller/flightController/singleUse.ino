#include <Arduino.h>
#include <Servo.h>

Servo ESC;

// Servo library voor de ESC communication
// Hou de opstartfrequentie op de gehanteerde waarde. Dat is lager op hetzelfde als 1ms. 

constexpr uint8_t ROTOR1_PIN = 9; 

constexpr uint8_t THROTTLE_PWM = 75; 

void setup() {
  ESC.attach(9,1000,2000)
  Serial.begin(115200);

  pinMode(ROTOR1_PIN, OUTPUT);
  analogWrite(ROTOR1_PIN, 0);  

  Serial.println(F("Single rotor test starting..."));
}

void loop() {
  analogWrite(ROTOR1_PIN, THROTTLE_PWM);
}
