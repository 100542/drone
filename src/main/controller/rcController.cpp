#include <stdio.h>

// NOTE: dit is voor nu nog conceptueel, hoe het zou kunnen werken.

// functies
int8_t throttle = 0;
int8_t yaw = 0; // rotaties op z-axis
int8_t pitch = 0;
int8_t roll = 0;
double yawPosition = 0.0;
double pitchPosition = 0.0;
double rollPosition = 0.0;

// comms & failsafes
char buttonPressed = 'o'; // o = nothing, x = sluit alles onmiddelijk af, zonder te wachten.
int8_t senderGroup = 8; // verplicht matchen met de drone ontvanger

bool arm = false; // false = disarmed, true = armed

void dronePowerToggle () {
    if (buttonPressed == 'x' && arm == false) {
        arm = true; // arm drone
    } else if (buttonPressed == 'x' && arm == true) {
        arm = false; // disarm drone || alleen als al aan staat
    }
};

void droneThrottle () {
// deze actie sluit de drone af, zoals eerder beschreven. ook dronePowerToggle doet hetzelfde.
    if (buttonPressed == 'x') {
        throttle = 0;

// acceleratie van de drone
    } else if (buttonPressed == 'a') {
        if (throttle < 40) {
            throttle += 5;
        } else if (throttle >= 40) {
            throttle += 1;
        }
    }

// omgekeerde logica bij afdalen in snelheid
    if (buttonPressed == 'b') {
        if (throttle > 40) {
            throttle -= 1;
        } else if (throttle <= 40) {
            throttle -= 5;
        }
    }
}

void droneYaw () {
// reguleert de yaw, rotatie op z-axis. voor nu is dit dmv een joystick, maar kan ook met een knop.
    if (yawPosition > 0) {
        yaw = 1;
    } else if (yawPosition < 0) {
        yaw = -1;
    } else {
        yaw = 0;
    }
}

void dronePitch () {
// reguleert de pitch, vooruit en achteruit beweging. voor nu is dit dmv een joystick, maar kan ook met een knop.
    if (yawPosition > 0 || yawPosition < 0) {
        pitch = 0;
    }

    if (pitchPosition > 0) {
        pitch = 1;
    } else if (pitchPosition < 0) {
        pitch = -1;
    } else {
        pitch = 0;
    }
}

void droneRoll () {
// reguleert de roll, links en rechts beweging. voor nu is dit dmv een joystick, maar kan ook met een knop.
    if (yawPosition > 0 || yawPosition < 0 ) {
        roll = 0;
    }

    if (rollPosition > 0) {
        roll = 1;
    } else if (rollPosition < 0) {
        roll = -1;
    } else {
        roll = 0;
    }
}