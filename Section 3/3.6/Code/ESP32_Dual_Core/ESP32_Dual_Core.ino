#include <math.h>

#define PIN_CORE0 13
#define PIN_CORE1 14

#define MSG_RESULT_SZE 20

xQueueHandle result_queue;


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

uint32_t loop_gpio(uint32_t count,uint8_t gpio_pin) {
  uint32_t pos;
  uint32_t start_millis,stop_millis;

  start_millis=millis();
  for (pos=0; pos<count; pos++) { 
    digitalWrite(gpio_pin, HIGH);
    digitalWrite(gpio_pin, LOW);
  }
  stop_millis=millis();
  return stop_millis-start_millis;
}


void do_messure() {
  uint32_t msg[MSG_RESULT_SZE];
  uint32_t total_millis=0;
  uint8_t pos=0;

  msg[0] = xPortGetCoreID();

  if (msg[0]==0) {
    msg[1] = loop_gpio(500000,PIN_CORE0);
  } else { 
    msg[1] = loop_gpio(500000,PIN_CORE1);
  }
  total_millis+=msg[1];
  msg[10] = double_pow(50000);
  total_millis+=msg[10];
  
  msg[11] = total_millis;

  xQueueSendToBack(result_queue,&msg,portMAX_DELAY);
  
}

void myCore0Task(void *pvParameters) {
  int taskno = (int)pvParameters;
  
  Serial.println(String("start Task on core ")+String(xPortGetCoreID()));

  while (1) {
    do_messure();
    delay(1000);
  }
}

void myCore1Task(void *pvParameters) {
  int taskno = (int)pvParameters;
  
  Serial.println(String("start Task on core ")+String(xPortGetCoreID()));

  while (1) {
    do_messure();
    delay(1000);
  }
}

void myResultTask(void *pvParameters) {
  int taskno = (int)pvParameters;
  uint32_t msg[MSG_RESULT_SZE];
  uint8_t pos=0;
  
  Serial.println(String("start result Task on core ")+String(xPortGetCoreID()));

  while (1) {
    if(xQueueReceive(result_queue,&msg,portMAX_DELAY)==pdTRUE) {  // max wait
      Serial.println(String("core ")+String(msg[0]));
      Serial.println(String("ESP32 ---- total execution time: ")+String((double)msg[11]/1000)+String(" s\n"));
    }
  }
}

void setup() {
    Serial.begin(115200);
    pinMode(PIN_CORE0, OUTPUT);
    pinMode(PIN_CORE1, OUTPUT);

    result_queue = xQueueCreate(10, sizeof(uint32_t)*MSG_RESULT_SZE);

    xTaskCreatePinnedToCore(myCore0Task, "myCore0Task", 1024, (void *)0, 1, NULL, 0); // run on core 0
    xTaskCreatePinnedToCore(myCore1Task, "myCore1Task", 1024, (void *)1, 1, NULL, 1); // run on core 1
    xTaskCreatePinnedToCore(myResultTask, "myResultTask", 1024, (void *)1, 1, NULL, 1); // run on core 1
}

void loop() {
  vTaskDelay(portMAX_DELAY); // wait as much as posible ...
}
