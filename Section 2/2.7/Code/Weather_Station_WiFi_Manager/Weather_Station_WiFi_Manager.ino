#include <FS.h>          
#include <WiFiManager.h>

#include "DHT.h"
#include "SSD1306.h" 
#include <Wire.h>


#define DHTPIN 27    
#define DHTTYPE DHT11   


SSD1306  display(0x3c, 21, 22);
DHT dht(DHTPIN, DHTTYPE);
//DHT dht(DHTPIN, DHTTYPE);


void setup() {
 
  Serial.begin(115200);
  display.init();
  dht.begin();

  
  WiFiManager wifiManager;
  wifiManager.autoConnect("ESPCONFIGAP");//wifiManager.autoConnect("Access point name","password")
  Serial.println("Connected to the configured WiFi");
  
  display.clear();
  display.drawString(0, 0, "Booting");
  display.display();
  delay(1000);

  

}

void loop() {


  float h = dht.readHumidity();
  float t = dht.readTemperature();
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) ) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  static char hum[15];
  static char temp[15];

  dtostrf(h,7, 0, hum);
  dtostrf(t,7, 0, temp);
  
  Serial.print(F("Humidity: "));
  Serial.print(hum);
  Serial.print(F("%  Temperature: "));
  Serial.print(temp);
  Serial.print(F("°C "));
  Serial.println();

  display.clear();
  display.drawString(30, 0, "Weather Station");
 
  
  display.drawString(0, 12,"Humidity: ");
  display.drawString(65,12, hum);
  display.drawString(95,12,"%");

  display.drawString(0, 24,"Temperature: ");
  display.drawString(65,24, temp);
  display.drawString(95, 24,"*C");
 
  display.display();
  delay(1000);
  display.clear();

  }
 
