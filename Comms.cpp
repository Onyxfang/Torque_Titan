#include "Comms.h"
#include <WiFi.h>
#include <WebServer.h>
#include "BlackBox.h" 
#include <Adafruit_NeoPixel.h>

void TaskCommsCode(void * pvParameters);

// Link to the global objects in Main
extern BlackBox logger; 
extern Adafruit_NeoPixel statusPixel;

WebServer server(80);

void setupWiFi() {
    // 1. Create the Network
    WiFi.softAP("TorqueTitans_Data", "seniorDesign"); // PW: seniorDesign
    
    Serial.print("WiFi IP: ");
    Serial.println(WiFi.softAPIP()); // Usually 192.168.4.1

    // 2. The Home Page - Updated to reflect the 5-sensor array
    server.on("/", HTTP_GET, []() {
        String msg = "Torque Titans Battle Logger v2.0\n";
        msg += "Status: 5-IR Array + IMU Logging Active\n";
        msg += "Go to /dump to download CSV.";
        server.send(200, "text/plain", msg);
    });

    // 3. The Download Link
    server.on("/dump", HTTP_GET, []() {
        server.sendHeader("Content-Disposition", "attachment; filename=training_data.csv");
        server.setContentLength(CONTENT_LENGTH_UNKNOWN);
        server.send(200, "text/csv", "");
        
        // Stream data from BlackBox to Laptop
        // This will now include sideLeft and sideRight automatically
        logger.streamToClient(&server);
        
        server.sendContent(""); // End
    });

    server.begin();
}

void startCommsTask() {
    // Comms runs on Core 0 to keep the battle logic on Core 1 uninterrupted
    xTaskCreatePinnedToCore(TaskCommsCode, "Comms", 5000, NULL, 1, NULL, 0);
}

void TaskCommsCode(void * pvParameters) {
    setupWiFi(); 

    for(;;) {
        // Check if laptop is requesting data
        server.handleClient(); 
        
        // Simple heartbeat 
        delay(10); 
    }
}