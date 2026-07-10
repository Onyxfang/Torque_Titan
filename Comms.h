#ifndef COMMS_H
#define COMMS_H

#include <Arduino.h>

/**
 * Torque Titan Communication & Logging System
 * * This module handles the WiFi Access Point (TorqueTitans_Data)
 * and the WebServer used for CSV data extraction.
 * * Note: This task is pinned to CORE 0 to prevent WiFi interrupts
 * from lagging the main Battle Logic (TaskPilotCode) on CORE 1.
 */

// This function will create the FreeRTOS task for WiFi & Logging
void startCommsTask(); 

#endif