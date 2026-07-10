#include "Motors.h"

// Private helper function (hidden from main file)
void setMotorRaw(int in1, int in2, int pwmPin, int speed) {
    // 1. Constrain speed to hardware limits
    int safeSpeed = constrain(speed, -MAX_PWM_LIMIT, MAX_PWM_LIMIT);
    
    // 2. Deadzone: Prevents motor stalling/humming at low power
    if (abs(safeSpeed) < 15) { 
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
        analogWrite(pwmPin, 0);
        return;
    }

    // 3. Determine direction
    bool forward = (safeSpeed >= 0);
    int mag = abs(safeSpeed);

    // 4. Write to Hardware
    digitalWrite(in1, forward ? HIGH : LOW);
    digitalWrite(in2, forward ? LOW  : HIGH);
    analogWrite(pwmPin, mag);
}

void initMotors() {
    pinMode(MOTOR_L_IN1, OUTPUT);
    pinMode(MOTOR_L_IN2, OUTPUT);
    pinMode(MOTOR_L_PWM, OUTPUT);

    pinMode(MOTOR_R_IN1, OUTPUT);
    pinMode(MOTOR_R_IN2, OUTPUT);
    pinMode(MOTOR_R_PWM, OUTPUT);

    stopMotors();
}

void setLeftMotor(int speed) {
    setMotorRaw(MOTOR_L_IN1, MOTOR_L_IN2, MOTOR_L_PWM, speed);
}

void setRightMotor(int speed) {
    setMotorRaw(MOTOR_R_IN1, MOTOR_R_IN2, MOTOR_R_PWM, speed);
}

void stopMotors() {
    setLeftMotor(0);
    setRightMotor(0);
}
