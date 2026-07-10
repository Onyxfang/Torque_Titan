#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_BNO08x.h>
#include <Adafruit_NeoPixel.h>
#include <math.h>

#include "Motors.h"
#include "Controller.h"
#include "Comms.h"
#include "BlackBox.h" 
#include "Kinematics.h"
#include "Tuner.h"

// Pins and Constants
const int BOOT_BTN_PIN = 0; 
const int LINE_L = A0; const int LINE_R = A1;
const int OBJ_L = A2;  const int OBJ_C = A3; const int OBJ_R = A4;
const int OBJ_SideL = 4; // GPIO 4 (SCL)
const int OBJ_SideR = 5; // GPIO 5 (A5)

const int OBJ_ACTIVE_STATE = LOW;
const int IMU_RST = 5; const int IMU_CS = 3; const int IMU_INT = 38;
#define NEOPIXEL_PIN 33

const int LINE_THRESHOLD = 900;
const int SEARCH_PWM = 180;
const int EDGE_BACKUP_SPEED = -200;
const int EDGE_TURN_SPEED = 200;
const int CONFIDENCE_THRESHOLD = 4;

// Shared Memory
volatile float shared_Yaw = 0.0;
volatile int   shared_LineL_ADC = 0;
volatile int   shared_LineR_ADC = 0;
volatile bool  shared_EnemyDetected = false;
volatile int   shared_BotMode = 0;

// Global Objects
Adafruit_BNO08x imu(IMU_RST);
Adafruit_NeoPixel statusPixel(1, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
BlackBox logger;

// Kinematics & Tuning
PIDController steerPID(380.0, 12.0, 8.0);
LowPassFilter yawFilter(0.15);
LowPassFilter lineLFilter(0.2);
LowPassFilter lineRFilter(0.2);

float currentKp = 380.0; float currentKi = 12.0; float currentKd = 8.0;

float wrapAngle(float a) {
    while (a > PI)  a -= 2.0f * PI;
    while (a < -PI) a += 2.0f * PI;
    return a;
}

/**
 * STEP RESPONSE TEST FUNCTION
 * Integrated to work within the existing Pilot Task flow.
 */
void executeStepTurn(float relativeDeg, float ax, float ay, float az, float gx, float gy, float gz, int detSL, int detSR) {
    bool wasAlreadyRecording = logger.isRecording(); //
    if (!wasAlreadyRecording) logger.startRecording();

    float targetYaw = wrapAngle(shared_Yaw + (relativeDeg * 0.01745f));
    unsigned long stepStart = millis();
    steerPID.reset(); //

    while (millis() - stepStart < 2000) {
        float yErr = wrapAngle(targetYaw - shared_Yaw);
        float pCorr = steerPID.compute(yErr); //

        int tL = constrain((int)pCorr, -255, 255);
        int tR = constrain(-(int)pCorr, -255, 255);

        setLeftMotor(tL); setRightMotor(tR);
        
        // Log with Label 1 (Step Test)
        logger.logPacket(ax, ay, az, gx, gy, gz, shared_LineL_ADC, shared_LineR_ADC, detSL, detSR, tL, tR, 1, currentKp, currentKi, currentKd);
        delay(10);
    }
    stopMotors();
    if (!wasAlreadyRecording) logger.stopRecording(); //
}

void TaskPilotCode(void * pvParameters) {
    unsigned long stateStart = 0;
    int subState = 0;
    float targetYaw = 0.0;
    static int leftConfidence = 0, rightConfidence = 0;
    static bool lastButtonStateY = false, isRecordingActive = false;
    float ax=0, ay=0, az=0, gx=0, gy=0, gz=0;
    int targetL = 0, targetR = 0;

    for(;;) { 
        updateController(); //
        
        // Mode Switch to Manual Tuning
        if (getButtonA()) { 
            shared_BotMode = 5; 
            statusPixel.setPixelColor(0, 128, 0, 128); 
            statusPixel.show(); 
        }

        // Sensor Processing
        shared_LineL_ADC = (int)lineLFilter.filter(analogRead(LINE_L));
        shared_LineR_ADC = (int)lineRFilter.filter(analogRead(LINE_R));

        sh2_SensorValue_t sensorValue;
        if (imu.getSensorEvent(&sensorValue)) {
            if (sensorValue.sensorId == SH2_GAME_ROTATION_VECTOR) {
                float qw = sensorValue.un.gameRotationVector.real;
                float qx = sensorValue.un.gameRotationVector.i;
                float qy = sensorValue.un.gameRotationVector.j;
                float qz = sensorValue.un.gameRotationVector.k;
                shared_Yaw = yawFilter.filter(wrapAngle(atan2f(2.0f*(qw*qz + qx*qy), 1.0f-2.0f*(qy*qy + qz*qz))));
            } else if (sensorValue.sensorId == SH2_LINEAR_ACCELERATION) {
                ax = sensorValue.un.linearAcceleration.x; ay = sensorValue.un.linearAcceleration.y; az = sensorValue.un.linearAcceleration.z;
            } else if (sensorValue.sensorId == SH2_GYROSCOPE_CALIBRATED) {
                gx = sensorValue.un.gyroscope.x; gy = sensorValue.un.gyroscope.y; gz = sensorValue.un.gyroscope.z;
            }
        }

        bool detSL = (digitalRead(OBJ_SideL) == OBJ_ACTIVE_STATE);
        bool detL  = (digitalRead(OBJ_L)     == OBJ_ACTIVE_STATE);
        bool detC  = (digitalRead(OBJ_C)     == OBJ_ACTIVE_STATE);
        bool detR  = (digitalRead(OBJ_R)     == OBJ_ACTIVE_STATE);
        bool detSR = (digitalRead(OBJ_SideR) == OBJ_ACTIVE_STATE);
        shared_EnemyDetected = (detSL || detL || detC || detR || detSR);

        // State Machine
        switch(shared_BotMode) {
            case 0: // WAIT
                targetL = 0; targetR = 0;
                if (digitalRead(BOOT_BTN_PIN) == LOW) { shared_BotMode = 1; stateStart = millis(); }
                break;

            case 1: // COUNTDOWN
                targetL = 0; targetR = 0;
                if (millis() - stateStart > 5000) shared_BotMode = 2;
                break;

            case 2: // SEARCH
                if (shared_LineL_ADC < LINE_THRESHOLD) leftConfidence++; else leftConfidence = 0;
                if (shared_LineR_ADC < LINE_THRESHOLD) rightConfidence++; else rightConfidence = 0;
                if (leftConfidence >= CONFIDENCE_THRESHOLD || rightConfidence >= CONFIDENCE_THRESHOLD) {
                   shared_BotMode = 4; subState = 0; stateStart = millis();
                   float turnDir = (leftConfidence >= rightConfidence) ? -1.0 : 1.0;
                   targetYaw = wrapAngle(shared_Yaw + (130.0 * 0.01745 * turnDir));
                } else if (shared_EnemyDetected) shared_BotMode = 3;
                else { targetL = SEARCH_PWM; targetR = SEARCH_PWM; }
                break;

            case 3: { // ATTACK
                if (shared_LineL_ADC < LINE_THRESHOLD || shared_LineR_ADC < LINE_THRESHOLD) { 
                    shared_BotMode = 4; subState = 0; stateStart = millis(); break; 
                }
                if (!shared_EnemyDetected) { shared_BotMode = 2; break; }
                float sX = 0, sY = 0; 
                if (detSL) { sX += cosf(-1.20f); sY += sinf(-1.20f); }
                if (detL)  { sX += cosf(-0.61f); sY += sinf(-0.61f); }
                if (detC)  { sX += 1.0f; }
                if (detR)  { sX += cosf(0.61f);  sY += sinf(0.61f);  }
                if (detSR) { sX += cosf(1.20f);  sY += sinf(1.20f);  }
                float err = atan2f(sY, sX);
                float corr = steerPID.compute(err); //
                int attackSpeed = (detC) ? 255 : 180; 
                targetL = constrain(attackSpeed + (int)corr, -255, 255);
                targetR = constrain(attackSpeed - (int)corr, -255, 255);
                break;
            }

            case 4: { // ESCAPE
                unsigned long elapsed = millis() - stateStart;
                if (subState == 0) {
                   targetL = EDGE_BACKUP_SPEED; targetR = EDGE_BACKUP_SPEED;
                   if (elapsed > 200) { subState = 1; stateStart = millis(); steerPID.reset(); }
                } else {
                   float yErr = wrapAngle(targetYaw - shared_Yaw);
                   float pCorr = steerPID.compute(yErr); //
                   targetL = constrain(EDGE_TURN_SPEED + (int)pCorr, -255, 255);
                   targetR = constrain(-EDGE_TURN_SPEED + (int)pCorr, -255, 255);
                   if (abs(yErr) < 0.15 || (millis() - stateStart > 1500)) shared_BotMode = 2;
                   if (detC) shared_BotMode = 3;
                }
                break;
            }

            case 5: // MANUAL & STEP TESTING
                if (isControllerConnected()) {
                    // Standard Movement
                    int throtPWM = map(getThrottle(), -512, 512, -255, 255);
                    int turnPWM  = map(getTurn(), -512, 512, -150, 150);
                    targetL = throtPWM - turnPWM; targetR = throtPWM + turnPWM;

                    updateLiveTuning(); //

                    // Step Test Triggers
                    if (getButtonX()) executeStepTurn(-90.0, ax, ay, az, gx, gy, gz, detSL, detSR);
                    if (getButtonB()) executeStepTurn(90.0, ax, ay, az, gx, gy, gz, detSL, detSR);

                    // Continuous Log Toggle
                    if (getButtonY() && !lastButtonStateY) {
                        if (!isRecordingActive) { logger.startRecording(); isRecordingActive = true; }
                        else { logger.stopRecording(); isRecordingActive = false; }
                    }
                    lastButtonStateY = getButtonY();
                } else { targetL = 0; targetR = 0; }
                break;
        }
        setLeftMotor(targetL); setRightMotor(targetR);
        
        // Standard Background Logging
        logger.logPacket(ax, ay, az, gx, gy, gz, shared_LineL_ADC, shared_LineR_ADC, (detSL?1:0), (detSR?1:0), targetL, targetR, 0, currentKp, currentKi, currentKd);
        delay(10);
    }
}

void setup() {
    Serial.begin(115200); initMotors(); initController();
    pinMode(OBJ_L, INPUT); pinMode(OBJ_C, INPUT); pinMode(OBJ_R, INPUT);
    pinMode(OBJ_SideL, INPUT); pinMode(OBJ_SideR, INPUT);
    pinMode(BOOT_BTN_PIN, INPUT_PULLUP);
    
    statusPixel.begin(); logger.begin(); SPI.begin();
    
    if (imu.begin_SPI(IMU_CS, IMU_INT, &SPI)) {
        imu.enableReport(SH2_GAME_ROTATION_VECTOR, 5000);
        imu.enableReport(SH2_LINEAR_ACCELERATION, 5000);
        imu.enableReport(SH2_GYROSCOPE_CALIBRATED, 5000);
    }
    
    xTaskCreatePinnedToCore(TaskPilotCode, "Pilot", 10000, NULL, 1, NULL, 1); //
    startCommsTask(); //
}

void loop() { vTaskDelete(NULL); }