#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>

/**
 * Torque Titan Motor Controller Hardware Map
 * Using standard H-Bridge (IN1, IN2, PWM) logic.
 * Note: Pins 6, 9-13 are safe from I2C/SPI conflicts on ESP32-S3.
 */

// ==========================================
// MOTOR PIN DEFINITIONS
// ==========================================
// Left Motor (Motor A)
#define MOTOR_L_IN1  13
#define MOTOR_L_IN2  12
#define MOTOR_L_PWM  11 

// Right Motor (Motor B)
#define MOTOR_R_IN1  10
#define MOTOR_R_IN2  9
#define MOTOR_R_PWM  6

// ==========================================
// TUNING CONSTANTS
// ==========================================
#define MAX_PWM_LIMIT 255  // Top hardware speed (8-bit resolution)

// ==========================================
// FUNCTIONS
// ==========================================
void initMotors();

/** * Sets Left Motor Speed
 * @param speed Range -255 (Reverse) to 255 (Forward)
 */
void setLeftMotor(int speed); 

/** * Sets Right Motor Speed
 * @param speed Range -255 (Reverse) to 255 (Forward)
 */
void setRightMotor(int speed);

void stopMotors();

#endif
