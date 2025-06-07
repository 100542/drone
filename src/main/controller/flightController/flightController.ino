#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Servo.h>

const char* ssid = "SSID_HIER";
const char* password = "PW_HIER";
WiFiUDP Udp;
const unsigned int localUdpPort = 4210;
char incomingPacket[255];

Servo esc1;
Servo esc2;
Servo esc3;
Servo esc4;

const int escPin1 = D4;
const int escPin2 = D3;
const int escPin3 = D2; 
const int escPin4 = D1; 

int speedM1 = 1000;
int speedM2 = 1000;
int speedM3 = 1000;
int speedM4 = 1000;

bool controllerConnected = false;

void setup() {
  Serial.begin(115200);
  
  esc1.attach(escPin1);
  esc2.attach(escPin2);
  esc3.attach(escPin3);
  esc4.attach(escPin4);

  for (int i = 0; i < 3; i++) {
    esc1.writeMicroseconds(1000);
    esc2.writeMicroseconds(1000);
    esc3.writeMicroseconds(1000);
    esc4.writeMicroseconds(1000);
    delay(500);
  }

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Udp.begin(localUdpPort);
  Serial.println("\nWiFi connected");
  Serial.print("IP: "); Serial.println(WiFi.localIP());
  Serial.print("Listening on UDP port "); Serial.println(localUdpPort);
}

void loop() {
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    int len = Udp.read(incomingPacket, 255);
    if (len > 0) incomingPacket[len] = '\0';
    
    controllerConnected = true;
    processCommand(String(incomingPacket));
  } else {
    controllerConnected = false;
    applyFailsafe();
  }

  esc1.writeMicroseconds(speedM1);
  esc2.writeMicroseconds(speedM2);
  esc3.writeMicroseconds(speedM3);
  esc4.writeMicroseconds(speedM4);

  delay(20);
}

void processCommand(String cmd) {
  cmd.trim();

  if (cmd == "THROTTLE_UP") {
    adjustThrottle(50);
  } else if (cmd == "THROTTLE_DOWN") {
    adjustThrottle(-50);
  } else if (cmd == "MOVE_LEFT") {
    speedM1 += 20;
    speedM4 += 20;
    speedM2 -= 20;
    speedM3 -= 20;
  } else if (cmd == "MOVE_RIGHT") {
    speedM1 -= 20;
    speedM4 -= 20;
    speedM2 += 20;
    speedM3 += 20;
  } else if (cmd == "TURN_LEFT") {
    speedM1 -= 20;
    speedM3 -= 20;
    speedM2 += 20;
    speedM4 += 20;
  } else if (cmd == "TURN_RIGHT") {
    speedM1 += 20;
    speedM3 += 20;
    speedM2 -= 20;
    speedM4 -= 20;
  } else if (cmd == "LAND") {
    performLanding();
  } else if (cmd == "PING") {
    controllerConnected = true;
  }

  clampSpeeds();
}

void adjustThrottle(int delta) {
  speedM1 += delta;
  speedM2 += delta;
  speedM3 += delta;
  speedM4 += delta;
}

void clampSpeeds() {
  speedM1 = constrain(speedM1, 1000, 2000);
  speedM2 = constrain(speedM2, 1000, 2000);
  speedM3 = constrain(speedM3, 1000, 2000);
  speedM4 = constrain(speedM4, 1000, 2000);
}

void applyFailsafe() {
  speedM1 = 1000;
  speedM2 = 1000;
  speedM3 = 1000;
  speedM4 = 1000;
}

void performLanding() {
  while (speedM1 > 1000 || speedM2 > 1000 || speedM3 > 1000 || speedM4 > 1000) {
    adjustThrottle(-10);
    clampSpeeds();
    esc1.writeMicroseconds(speedM1);
    esc2.writeMicroseconds(speedM2);
    esc3.writeMicroseconds(speedM3);
    esc4.writeMicroseconds(speedM4);
    delay(100);
  }
}
