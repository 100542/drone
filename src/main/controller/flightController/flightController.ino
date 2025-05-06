#include <Arduino.h>
#include <nRF24L01.h>   
#include <RF24.h>

char DIGITAL_PINS = 'D';
char ANALOG_PINS = 'A';

int throttle = 0;
int yaw = 0;
int pitch = 0;
int roll = 0;
bool arm = false;

int rotor1 = ANALOG_PINS + 0;
int rotor2 = ANALOG_PINS + 1;
int rotor3 = ANALOG_PINS + 2;
int rotor4 = ANALOG_PINS + 3;

const radioChannel = RF24 radio(8);
bool radioConnected = false;

const byte address[6] = "00001";

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
    Serial.print("RADIO CHANNEL:"); Serial.println(radioChannel ? "CONNECTED" : "DISCONNECTED");
}

void readControllerInput() {
    if (Serial.available() >= 6) {
        // Lees inputs vanuit de controller
        arm      = Serial.read();
        failsafe = Serial.read(); 
        throttle = Serial.read();   
        yaw      = Serial.read();  
        pitch    = Serial.read();  
        roll     = Serial.read();   
        radioChannel = Serial.read();
        radioConnected = Serial.read();
    }
}

void setup() {
    // Setup function (signalen naar rotors)
    Serial.begin(9600);
    radio.begin();
    radio.openWritingPipe(address); 
    radio.setPALevel(RF24_PA_MIN);
    radio.startListening();

    Serial.println("Flight Controller Initialized");

    // Initialiseer de rotors
    pinMode(rotor1, OUTPUT);
    pinMode(rotor2, OUTPUT);
    pinMode(rotor3, OUTPUT);
    pinMode(rotor4, OUTPUT);

    // Zet de rotors aan
    digitalWrite(rotor1, HIGH);
    digitalWrite(rotor2, HIGH);
    digitalWrite(rotor3, HIGH);
    digitalWrite(rotor4, HIGH);

    // Zet de rotors uit
    digitalWrite(rotor1, LOW);
    digitalWrite(rotor2, LOW);
    digitalWrite(rotor3, LOW);
    digitalWrite(rotor4, LOW);
}

void loop() {
    if (radio.available()) {
        // Ontvang data van de controller
        char text[32] = "";
        radio.read(&text, sizeof(text));
        Serial.println(text);
    }

    readControllerInput();
    applyControls();
    delay(50);
}
