#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

const char* ssid = "ESPTEST";
const char* password = "chooka123";

void setup() 
  {
      Serial.begin(115200);
      Serial.println("Booting");
      WiFi.mode(WIFI_STA);
      WiFi.begin(ssid, password);
      while (WiFi.waitForConnectResult() != WL_CONNECTED) 
        {
          Serial.println("Connection Failed! Rebooting...");
          delay(5000);
          ESP.restart();
         }

    Serial.println("Ready");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    
    ArduinoOTA.setHostname("myesp32");
    ArduinoOTA.setPassword("admin");
    ArduinoOTA.begin();
       
    ArduinoOTA
      .onStart([]() {
        Serial.println("Started updating");
      })
      .onEnd([]() {
        Serial.println("\nEnd");
      })
      .onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
      })
      .onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
      });


    pinMode(5,OUTPUT);
  }

void loop() 
 {
   ArduinoOTA.handle();
  
  digitalWrite(5, HIGH);
  delay(2500);
  digitalWrite(5, LOW);
  delay(2500);
 }
