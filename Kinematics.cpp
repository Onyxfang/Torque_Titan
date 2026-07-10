#include "Kinematics.h"

// --- PID Implementation ---
PIDController::PIDController(float p, float i, float d) {
    Kp = p; Ki = i; Kd = d;
    prevError = 0; integral = 0; lastTime = millis();
}

float PIDController::compute(float error) {
    unsigned long now = millis();
    float dt = (now - lastTime) / 1000.0f;
    
    // Safety: If dt is too small, skip this iteration to avoid derivative spikes
    if (dt < 0.001f) return (Kp * error) + (Ki * integral); 

    // Integral with anti-windup (already handled by your constrain)
    integral = constrain(integral + (error * dt), -50.0f, 50.0f);
    
    // Derivative (rate of change of error)
    float derivative = (error - prevError) / dt;
    
    float output = (Kp * error) + (Ki * integral) + (Kd * derivative);

    prevError = error;
    lastTime = now;
    return output;
}

void PIDController::reset() {
    integral = 0; prevError = 0; lastTime = millis();
}

// --- Low-Pass Filter Implementation ---
LowPassFilter::LowPassFilter(float f_alpha) {
    alpha = f_alpha;
    prevOutput = 0;
}

float LowPassFilter::filter(float input) {
    // Formula: $y[n] = \alpha \cdot x[n] + (1 - \alpha) \cdot y[n-1]$
    float output = (alpha * input) + ((1.0f - alpha) * prevOutput);
    prevOutput = output;
    return output;
}