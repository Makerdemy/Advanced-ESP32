#define MDASH_APP_NAME "MinimalApp" //This will be shown on dashboard as the Running program's name
#include <mDash.h> 
#include <WiFi.h> //Unlike old mdash, the inlusion of this library makes integration of projects easy 
#define WIFI_NETWORK " " //Add SSID
#define WIFI_PASSWORD " " //Add Password
#define DEVICE_PASSWORD " " //Copy device token ID from the dashboard and paste it here 

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_NETWORK, WIFI_PASSWORD); 
  mDashBegin(DEVICE_PASSWORD); //Used instead of mDashStartWithWifi
  pinMode(5, OUTPUT);
}

void loop() {
  //delay(1000);
  digitalWrite(5, HIGH);
  delay(250);
  digitalWrite(5, LOW);
  delay(250);
}
