#include <WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>


void setup() {
   
    Serial.begin(115200);
    WiFiManager wifiManager;
    wifiManager.autoConnect("ESPCONFIGAP");//wifiManager.autoConnect("Access point name","password")
    Serial.println("Connected to the configured WiFi");
}

void loop() {
    // put your main code here, to run repeatedly:
    
}
