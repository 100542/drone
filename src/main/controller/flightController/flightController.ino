#include <Arduino.h>
#include <nRF24L01.h>
#include <RF24.h>

constexpr uint8_t RF_CE_PIN  = 8;
constexpr uint8_t RF_CSN_PIN = 9;

constexpr uint8_t ROTOR1_PIN = 5; 
constexpr uint8_t ROTOR2_PIN = 6;  
constexpr uint8_t ROTOR3_PIN = 9;  
constexpr uint8_t ROTOR4_PIN = 10; 

const byte RADIO_ADDRESS[6] = "00001";

struct ControlPacket {
  bool   armed;
  bool   failsafe;
  int16_t throttle;  
  int16_t yaw;      
  int16_t pitch;     
  int16_t roll;      
};

RF24 radio(RF_CE_PIN, RF_CSN_PIN);
ControlPacket rx{};
bool radioConnected = false;
unsigned long lastPacketMs = 0;

constexpr uint16_t FAILSAFE_TIMEOUT_MS = 500; 

void stopMotors() {
  analogWrite(ROTOR1_PIN, 0);
  analogWrite(ROTOR2_PIN, 0);
  analogWrite(ROTOR3_PIN, 0);
  analogWrite(ROTOR4_PIN, 0);
}

void applyControls() {
  if (!rx.armed || rx.failsafe || !radioConnected) {
    stopMotors();
    return;
  }
  uint8_t pwm = map(rx.throttle, 0, 1000, 0, 255);
  analogWrite(ROTOR1_PIN, pwm);
  analogWrite(ROTOR2_PIN, pwm);
  analogWrite(ROTOR3_PIN, pwm);
  analogWrite(ROTOR4_PIN, pwm);
}

void setup() {
  Serial.begin(115200);

  pinMode(ROTOR1_PIN, OUTPUT);
  pinMode(ROTOR2_PIN, OUTPUT);
  pinMode(ROTOR3_PIN, OUTPUT);
  pinMode(ROTOR4_PIN, OUTPUT);
  stopMotors();

  if (!radio.begin()) {
    Serial.println(F("NRF24 init failed!"));
  }
  radio.openReadingPipe(1, RADIO_ADDRESS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_1MBPS);
  radio.startListening();

  Serial.println(F("Flight controller ready"));
}

void loop() {
  if (radio.available()) {
    radio.read(&rx, sizeof(rx));
    lastPacketMs = millis();
    radioConnected = true;
  }

  if (millis() - lastPacketMs > FAILSAFE_TIMEOUT_MS) {
    radioConnected = false;
  }

  applyControls();
}