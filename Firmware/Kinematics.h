#ifndef KINEMATICS_H
#define KINEMATICS_H

#include <Arduino.h>

/**
 * Torque Titan Control Systems
 * - PIDController: Manages the steering vector and target yaw.
 * - LowPassFilter: Smooths noisy IR and IMU data.
 */

class PIDController {
  private:
    float Kp, Ki, Kd;
    float prevError;
    float integral;
    unsigned long lastTime;

  public:
    PIDController(float p, float i, float d);
    float compute(float error);
    void reset();

    // Helper functions for Tuner integration
    void setGains(float p, float i, float d) { Kp = p; Ki = i; Kd = d; }
    float getKp() { return Kp; }
    float getKi() { return Ki; }
    float getKd() { return Kd; }
};

class LowPassFilter {
  private:
    float alpha; // Smoothing factor (0.0 to 1.0)
    float prevOutput;

  public:
    // alpha closer to 1.0 = more raw data, closer to 0.0 = more smoothing
    LowPassFilter(float f_alpha);
    float filter(float input);
};

#endif
