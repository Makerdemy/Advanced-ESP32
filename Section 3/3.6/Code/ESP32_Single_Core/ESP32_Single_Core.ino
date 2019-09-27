#include <math.h>

uint32_t double_pow(uint32_t count) {
  uint32_t pos;
  double t = 0;
  uint32_t start_millis,stop_millis;
  
  start_millis=millis();
  for (pos=0; pos<count; pos++) {
    t = pow(pos,2);
 }
  stop_millis=millis();
  return stop_millis-start_millis;
}

uint32_t loop_gpio(uint32_t count) {
  uint32_t pos;
  uint32_t start_millis,stop_millis;

  start_millis=millis();
  for (pos=0; pos<count; pos++) { 
    digitalWrite(13, HIGH);
    digitalWrite(13, LOW);
  }
  stop_millis=millis();

  return stop_millis-start_millis;
}


void do_messure() {
  uint32_t total_millis=0;
  total_millis += loop_gpio(1000000);
  total_millis += double_pow(100000);
  Serial.print("Speed Test is running on core ");
  Serial.println(xPortGetCoreID());
  Serial.println(String("ESP32 ---- total execution time ")+String((double)total_millis/1000)+String(" s\n"));
    
}


void setup() {
    Serial.begin(115200);
}

void loop() {
  do_messure();
  delay(1000);
}
