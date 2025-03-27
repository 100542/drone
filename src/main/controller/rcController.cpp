#include <stdio.h>

int throttle = 0;
int yaw = 0;
int pitch = 0;
int roll = 0;
double yawPosition = 0.0;
double pitchPosition = 0.0;
double rollPosition = 0.0;

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
};

ControllerInput controllerState;
bool arm = false;

void dronePowerToggle(bool buttonX) {
    static bool lastState = false; 

    if (buttonX && !lastState) {
        arm = !arm; 
        printf("Drone %s\n", arm ? "Armed" : "Disarmed");
    }
    
    lastState = buttonX; 
}

void droneThrottle(bool buttonA, bool buttonB) {
    if (buttonA) {
        throttle += (throttle < 40) ? 5 : 1;
    } else if (buttonB) {
        throttle -= (throttle > 40) ? 1 : 5;
    }

    if (throttle < 0) throttle = 0;
}

void droneYaw(double yawPosition) {
    yaw = (yawPosition > 0) ? 1 : (yawPosition < 0) ? -1 : 0;
}

void dronePitch(double pitchPosition) {
    pitch = (pitchPosition > 0) ? 1 : (pitchPosition < 0) ? -1 : 0;
}

void droneRoll(double rollPosition) {
    roll = (rollPosition > 0) ? 1 : (rollPosition < 0) ? -1 : 0;
}

void updateController() {
    dronePowerToggle(controllerState.armToggle);
    droneThrottle(controllerState.throttleUp, controllerState.throttleDown);
    droneYaw(yawPosition);
    dronePitch(pitchPosition);
    droneRoll(rollPosition);
}

int main() {
    printf("Controller Initialized\n");

    controllerState.armToggle = true; 
    updateController();
    
    controllerState.throttleUp = true; 
    updateController();
    
    controllerState.yawRight = true;
    yawPosition = 0.5; 
    updateController();

    return 0;
}
