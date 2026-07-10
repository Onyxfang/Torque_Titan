#ifndef BLACKBOX_H
#define BLACKBOX_H

#include <Arduino.h>
#include <vector>
#include <WebServer.h> // Needed for WiFi dumping

// The "Experience" Packet
struct TrainingData {
    unsigned long timestamp;
    float ax, ay, az;        // Physics (Accel)
    float gx, gy, gz;        // Physics (Gyro)
    int lineLeft, lineRight; // Vision (Line Sensors)
    int sideLeft, sideRight; // <--- ADDED: New Side IR Sensors
    int motorLeftCmd;        // Action
    int motorRightCmd;       // Action
    int label;               // Context (0=Cruise, 1=Attack, 2=Escape)
    
    // --- PID Tracking Fields ---
    float pGain; 
    float iGain; 
    float dGain; 
};

class BlackBox {
public:
    BlackBox();
    void begin();
    
    // Save a packet including current PID tuning values and new side sensors
    void logPacket(float ax, float ay, float az, 
                   float gx, float gy, float gz,
                   int lineL, int lineR,
                   int sideL, int sideR, // <--- ADDED: Args for new sensors
                   int motorL, int motorR,
                   int label,
                   float p, float i, float d); // Total 16 arguments now

    // WiFi Dump Helper
    void streamToClient(WebServer *server);

    // Control
    void startRecording();
    void stopRecording();
    bool isRecording();
    int getSampleCount();

private:
    std::vector<TrainingData> *memoryBank; 
    bool _recording;
    unsigned long _startTime;
};

#endif
