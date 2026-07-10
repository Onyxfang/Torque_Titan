#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Arduino.h>
#include <Bluepad32.h>

/**
 * Torque Titan Bluetooth Controller Interface
 * Handles Xbox/PS4 controller inputs via Bluepad32.
 * Used for Manual Mode (5) and tagging data for Edge Impulse.
 */

void initController();
void updateController();
bool isControllerConnected();
int getThrottle(); 
int getTurn();


// State & Label Mapping
bool getButtonA(); // Switch to Manual
bool getButtonY(); // Toggle BlackBox Recording
bool getButtonX(); // Tag: Attack (Label 1)
bool getButtonB(); // Tag: Escape (Label 2)
bool getButtonL1();
bool getButtonR1();

// Live PID Tuning (D-Pad)
bool getDpadUp();
bool getDpadDown();
bool getDpadLeft();
bool getDpadRight();

#endif