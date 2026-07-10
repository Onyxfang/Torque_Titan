#include "BlackBox.h"
#include <WebServer.h>

BlackBox::BlackBox() {
    _recording = false;
    memoryBank = new std::vector<TrainingData>(); 
}

void BlackBox::begin() {
    // Reserve space to prevent memory fragmentation (approx 1 minute of data)
    memoryBank->reserve(5000); 
    Serial.println("[BlackBox] System Ready. Memory Initialized.");
}

void BlackBox::startRecording() {
    _recording = true;
    _startTime = millis();
    memoryBank->clear(); 
    Serial.println("[BlackBox] RECORDING STARTED.");
}

void BlackBox::stopRecording() {
    _recording = false;
    Serial.print("[BlackBox] STOPPED. Samples: ");
    Serial.println(memoryBank->size());
}

bool BlackBox::isRecording() { return _recording; }
int BlackBox::getSampleCount() { return memoryBank->size(); }

// Added sideL and sideR parameters
void BlackBox::logPacket(float ax, float ay, float az, 
                         float gx, float gy, float gz,
                         int lineL, int lineR,
                         int sideL, int sideR, // <--- Added these
                         int motorL, int motorR,
                         int label,
                         float p, float i, float d) { 
    if (!_recording) return;

    TrainingData packet;
    packet.timestamp = millis() - _startTime;
    packet.ax = ax; packet.ay = ay; packet.az = az;
    packet.gx = gx; packet.gy = gy; packet.gz = gz;
    packet.lineLeft = lineL; packet.lineRight = lineR;
    
    // Assigning the new side sensor values
    packet.sideLeft = sideL;   // Ensure these fields exist in BlackBox.h struct
    packet.sideRight = sideR;

    packet.motorLeftCmd = motorL; packet.motorRightCmd = motorR;
    packet.label = label;
    
    packet.pGain = p; 
    packet.iGain = i; 
    packet.dGain = d;

    memoryBank->push_back(packet);
}

// Updated the CSV streamer to include side sensor columns
void BlackBox::streamToClient(WebServer *server) {
    if (memoryBank->empty()) {
        server->sendContent("No Data Recorded\n");
        return;
    }

    // Updated CSV Header for Edge Impulse / Analysis
    server->sendContent("timestamp,ax,ay,az,gx,gy,gz,lineL,lineR,sideL,sideR,motorL,motorR,label\n");

    char buffer[200]; // Increased buffer size for extra columns
    for (const auto& row : *memoryBank) {
        snprintf(buffer, sizeof(buffer), "%lu,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%d,%d,%d,%d,%d,%d,%d\n",
            row.timestamp, row.ax, row.ay, row.az, 
            row.gx, row.gy, row.gz,
            row.lineLeft, row.lineRight,
            row.sideLeft, row.sideRight, // <--- Added these
            row.motorLeftCmd, row.motorRightCmd, row.label
        );
        server->sendContent(buffer);
    }
}
