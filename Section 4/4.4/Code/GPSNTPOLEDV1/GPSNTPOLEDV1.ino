#include <TinyGPS++.h>
#include <HardwareSerial.h>

#include <WiFi.h>
#include "time.h"
#include "SSD1306.h"

TinyGPSPlus gps;
HardwareSerial SerialGPS(1);

SSD1306  display(0x3c, 21, 22);

const char* ssid       = "makerdemy1";
const char* password   = "india123";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 19800;
const int   daylightOffset_sec = 0;

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.print(timeinfo.tm_hour);
  Serial.println(timeinfo.tm_min);
 // Serial.println(timeinfo.tm_sec);
  int h=timeinfo.tm_hour;
  int m=timeinfo.tm_min;
   
  static char hr[15];
  static char minn[15];

  dtostrf(h,2, 0, hr);
  dtostrf(m,2, 0, minn);
  
  /*
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");*/
  display.clear();
  display.drawString(0,0,"TIME-");
  display.drawString(48,0,"HH :");
  display.drawString(72,0,"MM");
  display.drawString(48,12,hr);
  display.drawString(72,12,minn);


  
  display.display();


}

void setup() {

  Serial.begin(115200);
  SerialGPS.begin(9600, SERIAL_8N1, 16, 17);


  display.init();
  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");
  
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void loop() {

     
     while (SerialGPS.available() > 0) {
        gps.encode(SerialGPS.read());
        Serial.print("LAT=");  Serial.println(gps.location.lat(), 6);
        Serial.print("LONG="); Serial.println(gps.location.lng(), 6);
        printLocalTime();

        double la=12.905618;//gps.location.lat();
        double lo=80.227316;//gps.location.lng();
        
   
        static char laa[15];
        static char loo[15];


        dtostrf(la,7, 5, laa);
        dtostrf(lo,7, 5, loo);

        
      display.drawString(0,24,"LAT-");
      display.drawString(48,24,laa);

      display.drawString(0,36,"LON-");
      display.drawString(48,36,loo);


      display.display();
    }
     
}


        
