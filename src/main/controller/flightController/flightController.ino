#include <stdio.h>

// hier komt de concept code voor de flight controller (mainframe drone)

// Radio config
int radioChannel = 8;
bool radioConnected = false;

// PIN types 
char analogPins = 'A';
char digitalPins = 'D';

// Controller inputs en waardes
int throttle = 0;
int yaw = 0;
int pitch = 0;
int roll = 0;
double yawPosition = 0.0;
double pitchPosition = 0.0;
double rollPosition = 0.0;

// buttons & joysticks
bool ButtonA = digitalPins + 5;
bool ButtonB = digitalPins + 6;
bool ButtonX = digitalPins + 7;

int yawPin = analogPins + 0;
int pitchPin = analogPins + 1;
int rollPin = analogPins + 2;

