#include <Arduino.h>
#include <nRF24L01.h>
#include <RF24.h>

constexpr uint8_t RF_CE_PIN  = 8;
constexpr uint8_t RF_CSN_PIN = 9;
const byte RADIO_ADDRESS[6] = "00001";

constexpr uint8_t BTN_ARM_PIN      = 5; 
constexpr uint8_t BTN_THROTTLE_UP  = 6;
constexpr uint8_t BTN_THROTTLE_DN  = 7;

constexpr uint8_t YAW_PIN   = A0;
constexpr uint8_t PITCH_PIN = A1;
constexpr uint8_t ROLL_PIN  = A2;

struct ControlPacket {
  bool   armed;
  bool   failsafe;
  int16_t throttle;  
  int16_t yaw;       
  int16_t pitch;    
  int16_t roll;      
};

RF24 radio(RF_CE_PIN, RF_CSN_PIN);
ControlPacket tx{};

unsigned long lastSendMs = 0;
constexpr uint16_t SEND_INTERVAL_MS = 50;

bool debounceRead(uint8_t pin) {
  static uint32_t lastChange[20] = {0};
  static bool lastState[20] = {false};
  bool state = !digitalRead(pin); 
  if (state != lastState[pin] && millis() - lastChange[pin] > 25) {
    lastChange[pin] = millis();
    lastState[pin] = state;
  }
  return lastState[pin];
}

int16_t readAxis(uint8_t pin) {
  int raw = analogRead(pin);    
  return map(raw, 0, 1023, -500, 500); 
}

void setup() {
  Serial.begin(115200);

  pinMode(BTN_ARM_PIN, INPUT_PULLUP);
  pinMode(BTN_THROTTLE_UP, INPUT_PULLUP);
  pinMode(BTN_THROTTLE_DN, INPUT_PULLUP);

  if (!radio.begin()) {
    Serial.println(F("NRF24 init failed!"));
  }
  radio.openWritingPipe(RADIO_ADDRESS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_1MBPS);
  radio.stopListening();

  Serial.println(F("RC controller ready"));
}

void loop() {
  static bool armLatched = false;
  if (debounceRead(BTN_ARM_PIN)) {
    if (!armLatched) {  
      tx.armed = !tx.armed;
      armLatched = true;
    }
  } else {
    armLatched = false;
  }

  bool throttleUp   = debounceRead(BTN_THROTTLE_UP);
  bool throttleDown = debounceRead(BTN_THROTTLE_DN);

  if (throttleUp && tx.throttle < 1000)  tx.throttle += (tx.throttle < 400 ? 5 : 1);
  if (throttleDown && tx.throttle > 0)   tx.throttle -= (tx.throttle > 400 ? 5 : 1);

  tx.yaw   = readAxis(YAW_PIN);
  tx.pitch = readAxis(PITCH_PIN);
  tx.roll  = readAxis(ROLL_PIN);

  if (millis() - lastSendMs >= SEND_INTERVAL_MS) {
    radio.write(&tx, sizeof(tx));
    lastSendMs = millis();
  }
}