#include "Controller.h"

ControllerPtr myController;

void onConnectedController(ControllerPtr ctl) {
    if (myController == nullptr) {
        myController = ctl;
        Serial.println("Controller Connected");
    }
}

void onDisconnectedController(ControllerPtr ctl) {
    if (myController == ctl) {
        myController = nullptr;
        Serial.println("Controller Disconnected");
    }
}

void initController() {
    BP32.setup(&onConnectedController, &onDisconnectedController);
    // Clearing keys ensures a fresh handshake if you swap controllers at competition
    BP32.forgetBluetoothKeys(); 
}

void updateController() {
    BP32.update();
}

bool isControllerConnected() {
    return (myController && myController->isConnected());
}

int getThrottle() {
    // Negative axisY because Bluepad32 usually reports 'up' as negative
    if (isControllerConnected()) return -myController->axisY();
    return 0;
}

int getTurn() {
    if (isControllerConnected()) return myController->axisRX();
    return 0;
}

/**
 * Torque Titan Button Mapping Reference:
 * A: Force Mode 5 (Manual)
 * B: Label '2' in Logger (e.g. Escape)
 * X: Label '1' in Logger (e.g. Attack)
 * Y: Toggle Data Recording
 */
bool getButtonA() { return (isControllerConnected() && myController->a()); }
bool getButtonB() { return (isControllerConnected() && myController->b()); }
bool getButtonX() { return (isControllerConnected() && myController->x()); }
bool getButtonY() { return (isControllerConnected() && myController->y()); }
// Add to Controller.cpp
bool getButtonL1() { return (isControllerConnected() && myController->buttons() & 0x10); } 
bool getButtonR1() { return (isControllerConnected() && myController->buttons() & 0x20); }

// --- DPAD MAPPINGS (Used by Tuner.h for live PID adjustments) ---
bool getDpadUp()    { return (isControllerConnected() && (myController->dpad() & 0x01)); }
bool getDpadDown()  { return (isControllerConnected() && (myController->dpad() & 0x02)); }
bool getDpadLeft()  { return (isControllerConnected() && (myController->dpad() & 0x04)); }
bool getDpadRight() { return (isControllerConnected() && (myController->dpad() & 0x08)); }