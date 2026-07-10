#ifndef TUNER_H
#define TUNER_H

#include <Arduino.h>
#include "Kinematics.h"
#include "Controller.h"

extern PIDController steerPID;
extern float currentKp, currentKi, currentKd;

const float KP_STEP = 1.0; 
const float KI_STEP = 0.1;
const float KD_STEP = 0.5;

void updateLiveTuning() {
    bool changed = false;

    // Kp: Snappiness (D-pad Up/Down)
    if (getDpadUp())    { currentKp += KP_STEP; changed = true; delay(150); }
    if (getDpadDown())  { currentKp -= KP_STEP; changed = true; delay(150); }
    
    // Ki: Steady State (D-pad Left/Right)
    if (getDpadLeft())  { currentKi += KI_STEP; changed = true; delay(150); }
    if (getDpadRight()) { currentKi -= KI_STEP; changed = true; delay(150); }

    // Kd: Damping/Wiggle (Bumpers L1/R1)
    if (getButtonL1())  { currentKd -= KD_STEP; changed = true; delay(150); }
    if (getButtonR1())  { currentKd += KD_STEP; changed = true; delay(150); }
    
    if (changed) {
        currentKp = max(0.0f, currentKp);
        currentKi = max(0.0f, currentKi);
        currentKd = max(0.0f, currentKd);
        
        steerPID.setGains(currentKp, currentKi, currentKd);
        Serial.printf("[TUNER] New Gains -> P:%.1f I:%.2f D:%.1f\n", currentKp, currentKi, currentKd);
    }
}
#endif