#include <stdio.h>
#include <time.h>

// Matrix voor de LEDs in het bord
int ledMatrix[5][5] = {
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0}
};

// Radio config
int radioChannel = 8;
bool radioConnected = false;

// Controller inputs en waardes
int throttle = 0;
int yaw = 0;
int pitch = 0;
int roll = 0;
double yawPosition = 0.0;
double pitchPosition = 0.0;
double rollPosition = 0.0;

// buttons & joysticks
bool ButtonA = 5;
bool ButtonB = 6;
bool ButtonX = 7;

int yawPin = A0;
int pitchPin = A1;
int rollPin = A2;

// controller inputs struct, om mogelijk te maken dat meerdere buttons tegelijk ingedrukt kunnen worden
struct ControllerInput {
    bool armToggle = false;
    bool throttleUp = false;
    bool throttleDown = false;
    bool yawLeft = false;
    bool yawRight = false;
    bool pitchUp = false;
    bool pitchDown = false;
    bool rollLeft = false;
    bool rollRight = false;
    bool failsafe;
};

// standaard controller op unarmed
struct ControllerInput controllerState = {};
bool arm = false;
unsigned long lastSignalTime = 0;
    
void checkFailsafe() { // verifieert of de drone nog controle heeft en override onmiddelijk alle acties.
    unsigned long currentTime = millis(); 

    if (radioConnected) {
        lastSignalTime = currentTime; // update de tijd van het laatste signaal
    }

    if (((currentTime - lastSignalTime)) > 5) { // als er meer dan 5 seconden zijn verstreken
        radioConnected = !controllerState.failsafe;
    } else {
        radioConnected = true;
    }

    if (radioConnected == false || controllerState.failsafe == true) { 
        // de radioConnected == false is hier vreemd, maar functioneerd als een relay voor de failsafe.
        // Want uiteraard al werkt de failsafe niet maar is de radio niet meer connected, is de drone nog steeds in de lucht.
        arm = false;
        throttle = 0;
        yaw = 0;
        pitch = 0;
        roll = 0;
    }
}



// functie voor het aan en uitzetten van de drone.
void dronePowerToggle(int buttonX) {

    if (controllerState.failsafe == true ) {
        arm = false; // extra beveiliging, mocht de drone throttle op hol slaan na een failsafe.
    }

    static bool lastState = false; // slaat de status van de knop op

    if (buttonX && !lastState) {
        arm = !arm; // veranderd de status van de drone
    }
    
    lastState = buttonX; 
}

void droneThrottle(int buttonA, int buttonB) {
    if (controllerState.failsafe == true) {
        throttle = 0;
        return;
    }

    if (buttonA) {
        if (throttle < 40) { // als de throttle onder de 40 is, verhoog met 5, anders met 1
            throttle += 5;
        } else {
            throttle += 1;
        }
    } else if (buttonB) {
        if (throttle >= 40) {  // als de throttle boven de 40 is, verlaag met 1, anders met 5
            throttle -= 5;
        } else {
            throttle -= 1;
        }
    }

    if (throttle < 0) throttle = 0; // throttle kan niet onder de 0
}

void droneYaw (int yawPosition) { // yawPosition is de positie van de yaw stick
    if (controllerState.failsafe == true) {
        yawPosition = 0;
        return;
    }

    if (yawPosition > 0) {
        yaw += 1;
    } else if (yawPosition < 0) {
        yaw -= 1;
    } else {
        yaw = 0;
    }
}

void dronePitch(int pitchPosition) { // pitchPosition is de positie van de pitch stick
    if (controllerState.failsafe == true) {
        pitchPosition = 0;
        return;
    }

    if (pitchPosition > 0) {
        pitch += 1;
    } else if (pitchPosition < 0) {
        pitch -= 1;
    } else {
        pitch = 0;
    }
}

void droneRoll(int rollPosition) { // rollPosition is de positie van de roll stick
    if (controllerState.failsafe == true) {
        rollPosition = 0;
        return;
    }

    if (rollPosition > 0) {
        roll += 1;
    } else if (rollPosition < 0) {
        roll -= 1;
    } else {
        roll = 0;
    }
}

void updateController() { // update de informatie naar de controller
    checkFailsafe(); // nogmaals de failsafe checken, want dit kan anders niet werken en de drone kan een ongeluk veroorzaken.
    droneYaw(yawPosition);
    dronePowerToggle(controllerState.armToggle);
    droneThrottle(controllerState.throttleUp, controllerState.throttleDown);
    dronePitch(pitchPosition);
    droneRoll(rollPosition);
}


void setup() {
    Serial.begin(9600);

    pinMode(ButtonX, INPUT_PULLUP);
    pinMode(ButtonB, INPUT_PULLUP);
    pinMode(ButtonA, INPUT_PULLUP);

    int InputBtnX = digitalRead(ButtonX);
    int InputBtnB = digitalRead(ButtonB);
    int InputBtnA = digitalRead(ButtonA);

    yaw = analogRead(yawPin);
    pitch = analogRead(pitchPin);
    roll = analogRead(rollPin);
}

int main() { // main functie om de controller te gebruiken

    controllerState.armToggle = true; 
    updateController();
    
    controllerState.throttleUp = true; 
    updateController();
    
    controllerState.yawRight = true;
    yawPosition = 0.5; 
    updateController();

    return 0;
}
