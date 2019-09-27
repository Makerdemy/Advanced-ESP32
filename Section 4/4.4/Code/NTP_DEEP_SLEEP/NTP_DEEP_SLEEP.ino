
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  58


#include <WiFi.h>
#include "time.h"
#include "SSD1306.h"


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
  
  int h=timeinfo.tm_hour;
  int m=timeinfo.tm_min;
  int d=timeinfo.tm_mday;
  int mn=timeinfo.tm_mon;
  int y=timeinfo.tm_year;
  mn=mn+1;
  y=1900+y;

   
  static char hr[15];
  static char minn[15];
  static char dayy[15];
  static char mont[15];
  static char yr[15];
 

  dtostrf(h,2, 0, hr);
  dtostrf(m,2, 0, minn);
  dtostrf(d,2, 0, dayy);
  dtostrf(mn,2, 0, mont); 
  dtostrf(y,2, 0, yr);
  /*
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");*/
  display.clear();
  display.drawString(0,0,"TIME-");
  display.drawString(48,0,"HH :");
  display.drawString(72,0,"MM");
  display.drawString(48,12,hr);
  display.drawString(72,12,minn);

  display.drawString(0,24,"DATE-");
  display.drawString(48,24,"DD");
  display.drawString(72,24,"MM");
  display.drawString(96,24,"YYYY");
  
  display.drawString(48,36,dayy);
  display.drawString(72,36,mont);
  display.drawString(96,36,yr);


  display.display();


}

void setup() {

  Serial.begin(115200);
   
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  display.init();
  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      display.clear();
      display.drawString(0,0,"Connecting to the Internet.");
      display.display();
      display.clear();
  }
  Serial.println(" CONNECTED");
  display.clear();
  display.drawString(0,24,"Synchronizing the clock");
  display.drawString(0,48,"using NTP");
  display.display();
  delay(1500);
  
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  Serial.println("Going to sleep now");
  Serial.flush(); 
  esp_deep_sleep_start();
  
}


void loop() {

     
        
}


        
