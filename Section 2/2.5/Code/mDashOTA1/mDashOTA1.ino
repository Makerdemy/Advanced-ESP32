#include <mDash.h>

#define WIFI_NAME "ESPTEST"
#define WIFI_PASS "chooka123"
#define DEVICE_ID "d4"
#define DEVICE_TOKEN "PJRDlLZV1xsYsBvLHVNL0g"

void setup() {
  Serial.begin(115200);
  mDashStartWithWifi(WIFI_NAME, WIFI_PASS, DEVICE_ID, DEVICE_TOKEN);
  pinMode(5, OUTPUT);
}

void loop() {
  //delay(1000);
  digitalWrite(5, HIGH);
  delay(250);
  digitalWrite(5, LOW);
  delay(250);
}
