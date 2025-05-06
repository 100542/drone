#include <Arduino.h>

int throttle = 0;
int yaw = 0;
int pitch = 0;
int roll = 0;
bool arm = false;

bool failsafe = false;

void applyControls() {
    if (!arm || failsafe) {
        // Sluit motoren af bij failsafe staat
        throttle = 0;
        yaw = 0;
        pitch = 0;
        roll = 0;
    }

    // Prints van de waarden
    Serial.print("Throttle: "); Serial.println(throttle);
    Serial.print("Yaw: "); Serial.println(yaw);
    Serial.print("Pitch: "); Serial.println(pitch);
    Serial.print("Roll: "); Serial.println(roll);
    Serial.print("ARMED: "); Serial.println(arm ? "ARMED" : "DISARMED");
    Serial.print("FAILSAFE: "); Serial.println(failsafe ? "AAN" : "UIT");
}

void readControllerInput() {
    if (Serial.available() >= 6) {
        arm      = Serial.read();
        failsafe = Serial.read(); 
        throttle = Serial.read();   
        yaw      = Serial.read();  
        pitch    = Serial.read();  
        roll     = Serial.read();   
    }
}

void setup() {
    Serial.begin(9600);
}

void loop() {
    readControllerInput();
    applyControls();

    delay(50);
}
